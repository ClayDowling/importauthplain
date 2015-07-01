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
#include <sqlite3.h>

#define STRING_LENGTH 80

struct userec {
	char login[STRING_LENGTH];
	char password[STRING_LENGTH];
	char name[STRING_LENGTH];
	char email[STRING_LENGTH];
};

typedef struct userec user_t;

void read_record(FILE* src, user_t* record);
