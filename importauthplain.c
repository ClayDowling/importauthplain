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

#include "importplain.h"

int group_gid[MAX_GLOBAL_GROUPS];

sqlite3 *db = NULL;

int currate_groups(FILE*);

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
        default:
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
        fprintf(stderr, "-s sourcefile is mandatory.\n");
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
        fprintf(stderr, "-d database file is madatory.\n");
        return EXIT_FAILURE;
    }

    if (currate_groups(src) == 0) {
        sqlite3_close(db);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
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
        fprintf(stderr, "Group: %s\n", global_groups[i]);
        if (sqlite3_bind_text(stmt, 1, global_groups[i], -1, SQLITE_STATIC) != SQLITE_OK) {
            fprintf(stderr, "sqlite_bind: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return 0;
        }
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "sqlite step: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return 0;
        }
        sqlite3_reset(stmt);
        group_gid[i] = sqlite3_last_insert_rowid(db);
    }

    sqlite3_finalize(stmt);
    return 1;
}
