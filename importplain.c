#include "importplain.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
		strncpy(ur->groups[i], group, FIELD_LENGTH);
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
