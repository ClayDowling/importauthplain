/**
 * Reads user and group records from an authplain text file and converts
 * them to an authsqlite database.
 *
 * TODO
 *
 * 1. Read records from the file.
 * 2. Read user groups.
 * 3. Write user records to database.
 * 4. Create any groups in the database that are missing.
 * 5. Associate user record with group records via usergroup table.
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "ImportAuthPlainConfig.h"
#include "importplain.h"

int group_gid[MAX_GLOBAL_GROUPS];

sqlite3 *db = NULL;

int currate_groups(FILE*);
int currate_users(FILE*, const char*);
int get_group_gid(const char*);

void help(const char *program) {

    fprintf(stderr, "DokuWiki auth_plain importer for auth_sqlite version %d.%d\n\n",
            ImportAuthPlain_VERSION_MAJOR, ImportAuthPlain_VERSION_MINOR);
    fprintf(stderr, "usage: %s -s source -d database [-a animal]\n"
                    "\n"
                    "-s auth_plain users file, with comments and blank lines removed.\n"
                    "-d database file with schema matching auth.sql\n"
                    "-a animal to associate these users with on the farm.\n",
            program);
}

int main(int argc, char** argv)
{
    FILE    *src = NULL;
    int     ch;
    char    *databasefile = NULL;
    char    *srcfilename = NULL;
    char    *animal = "";

    while((ch = getopt(argc, argv, "s:d:a:")) != -1) {
        switch(ch) {
        case 's':
            srcfilename = optarg;
            break;
        case 'd':
            databasefile = optarg;
            break;
        case 'a':
            animal = optarg;
            break;
        default:
            help(argv[0]);
            break;
        }
    }

    if (srcfilename) {
        struct stat sb;
        if (stat(srcfilename, &sb)) {
            fprintf(stderr, "%s not found.\n", srcfilename);
            return EXIT_FAILURE;
        }
        src = fopen(srcfilename, "r");
        if (NULL == src) {
            perror(srcfilename);
            return EXIT_FAILURE;
        }
    } else {
        help(argv[0]);
        fprintf(stderr, "\n*** -s sourcefile is mandatory.\n");
        return EXIT_FAILURE;
    }

    if (databasefile) {
        if (sqlite3_open(databasefile, &db) != SQLITE_OK) {
            fprintf(stderr, "%s", sqlite3_errmsg(db));
            if (NULL != db) {
                sqlite3_close(db);
            }
            return EXIT_FAILURE;
        }
    } else {
        help(argv[0]);
        fprintf(stderr, "\n*** -d database file is madatory.\n");
        return EXIT_FAILURE;
    }

    if (currate_groups(src) == 0)
        goto main_oops;

    if (currate_users(src, animal) == 0)
        goto main_oops;

    sqlite3_close(db);
    return EXIT_SUCCESS;

main_oops:
    sqlite3_close(db);
    return EXIT_FAILURE;
}

int currate_groups(FILE* src)
{
    sqlite3_stmt        *stmt;
    int                 i;
    struct userrecord   *ur;
    char                line[2048];

    rewind(src);

    if (sqlite3_prepare_v2(db, "INSERT INTO groups (name) VALUES (?)", -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "sqlite prepare: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    while(!feof(src)) {
        fgets(line, sizeof(line), src);
        ur = ur_parse(line);
        ur_delete(ur);
    }

    for(i=0; 0 != global_groups[i][0]; ++i) {
        if (sqlite3_bind_text(stmt, 1, global_groups[i], -1, SQLITE_STATIC) != SQLITE_OK) {
            goto currate_groups_oops;
        }
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            goto currate_groups_oops;
        }
        sqlite3_reset(stmt);
        group_gid[i] = sqlite3_last_insert_rowid(db);
    }

    sqlite3_finalize(stmt);
    return 1;


currate_groups_oops:
    fprintf(stderr, "%s: %s", __func__, sqlite3_errmsg(db));
    sqlite3_finalize(stmt);
    return 0;
}

int get_group_gid(const char *group)
{
    int i;

    for(i=0; i < MAX_GLOBAL_GROUPS && global_groups[i][0] != 0; ++i) {
        if (strcmp(global_groups[i], group) == 0) {
            return group_gid[i];
        }
    }

    return -1;
}


int currate_users(FILE *src, const char *animal)
{
    struct userrecord   *ur;
    sqlite3_stmt        *user_stmt = NULL;
    sqlite3_stmt        *user_group_stmt = NULL;
    char                line[2048];
    int                 i;
    int                 uid;
    int                 gid;
    int                 result = 0;

    if (sqlite3_prepare_v2(db, "INSERT INTO user (login, pass, fullname, email) VALUES (?, ?, ?, ?)", -1, &user_stmt, NULL) != SQLITE_OK) {
        goto currate_users_oops;
    }
    if (sqlite3_prepare_v2(db, "INSERT INTO usergroup (uid, gid, animal) VALUES (?, ?, ?)", -1, &user_group_stmt, NULL) != SQLITE_OK) {
        goto currate_users_oops;
    }
    sqlite3_exec(db, "BEGIN", NULL, NULL, NULL);
    rewind(src);

    while(!feof(src)) {
        fgets(line, sizeof(line), src);
        if (feof(src)) break;

        ur = ur_parse(line);
        if (NULL == ur)
            continue;

        if (sqlite3_bind_text(user_stmt, 1, ur->login, -1, SQLITE_STATIC) != SQLITE_OK)
            goto currate_users_oops;
        if (sqlite3_bind_text(user_stmt, 2, ur->password, -1, SQLITE_STATIC) != SQLITE_OK)
            goto currate_users_oops;
        if (sqlite3_bind_text(user_stmt, 3, ur->name, -1, SQLITE_STATIC) != SQLITE_OK)
            goto currate_users_oops;
        if (sqlite3_bind_text(user_stmt, 4, ur->email, -1, SQLITE_STATIC) != SQLITE_OK)
            goto currate_users_oops;
        if (sqlite3_step(user_stmt) != SQLITE_DONE)
            goto currate_users_oops;
        uid = sqlite3_last_insert_rowid(db);
        sqlite3_reset(user_stmt);
        ur_delete(ur);

        for(i=0; i < MAX_GROUPS && ur->groups[i][0] != 0; ++i) {
            gid = get_group_gid(ur->groups[i]);
            if (gid != -1) {
                if (sqlite3_bind_int(user_group_stmt, 1, uid) != SQLITE_OK)
                    goto currate_users_oops;
                if (sqlite3_bind_int(user_group_stmt, 2, gid) != SQLITE_OK)
                    goto currate_users_oops;
                if (sqlite3_bind_text(user_group_stmt, 3, animal, -1, SQLITE_STATIC) != SQLITE_OK)
                    goto currate_users_oops;
                if (sqlite3_step(user_group_stmt) != SQLITE_DONE)
                    goto currate_users_oops;
                if (sqlite3_reset(user_group_stmt) != SQLITE_OK)
                    goto currate_users_oops;
            }
        }
    }
    sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);
    result = 1;

currate_users_oops:
    if (user_stmt != NULL) {
        sqlite3_finalize(user_stmt);
    }
    if (user_group_stmt != NULL) {
        sqlite3_finalize(user_group_stmt);
    }
    if (result == 0) {
        fprintf(stderr, "%s: %s\n", __func__, sqlite3_errmsg(db));
    }
    return 0;
}
