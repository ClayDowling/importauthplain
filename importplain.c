#include "importplain.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char global_groups[MAX_GLOBAL_GROUPS][FIELD_LENGTH];
static int ur_initialized = 0;

void ur_add_global_group(const char*);
void ur_initialize();

void strip_trailing_whitespace(char *string)
{
    char *end;

    // Trim trailing space
    end = string + strlen(string) - 1;
    while(end > string && isspace(*end)) end--;

    // Write new null terminator
    *(end+1) = 0;

}

struct userrecord* ur_parse(const char* src)
{
	struct userrecord *ur;
	char *line;
	char *group;
	int i=0;

        line = strdup(src);
	ur = calloc(1, sizeof(struct userrecord));
	strncpy(ur->login, strtok(line, ":"), FIELD_LENGTH);
	strncpy(ur->password, strtok(NULL, ":"), FIELD_LENGTH);
	strncpy(ur->name, strtok(NULL, ":"), FIELD_LENGTH);
	strncpy(ur->email, strtok(NULL, ":"), FIELD_LENGTH);
	
    while((group = strtok(NULL, ",")) && i < MAX_GROUPS) {
        strip_trailing_whitespace(group);
        strncpy(ur->groups[i], group, FIELD_LENGTH);
        ur_add_global_group(group);
		++i;
	}

	free(line);
	return ur;
}

void ur_delete(struct userrecord* ur)
{
	if (ur) {
		free(ur);
	}
}

int ur_group_seen(const char *search)
{
    int i;
    ur_initialize();
    for(i=0; i < MAX_GLOBAL_GROUPS; ++i) {
        if (strncmp(global_groups[i], search, FIELD_LENGTH) == 0) {
            return 1;
        }
    }
    return 0;
}

void ur_add_global_group(const char* group)
{
    int i;
    ur_initialize();
    for(i=0; i < MAX_GLOBAL_GROUPS; ++i) {
        if (strlen(global_groups[i]) == 0) {
            break;
        }
        if (strncmp(global_groups[i], group, FIELD_LENGTH) == 0) {
            return;
        }
    }
    if (i < MAX_GLOBAL_GROUPS) {
        strncpy(global_groups[i], group, FIELD_LENGTH);
    }
}

void ur_initialize()
{
    if (ur_initialized) {
        return;
    }
    memset(global_groups, 0, sizeof(global_groups));
    ur_initialized = 1;
}
