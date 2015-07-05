#include "importplain.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct userrecord* ur_parse(const char* src)
{
	struct userrecord *ur;
	char *line;

	line = strdup(src);
	ur = calloc(1, sizeof(struct userrecord));
	strncpy(ur->login, strtok(line, ":"), FIELD_LENGTH);
	strncpy(ur->password, strtok(NULL, ":"), FIELD_LENGTH);
	strncpy(ur->name, strtok(NULL, ":"), FIELD_LENGTH);
	strncpy(ur->email, strtok(NULL, ":"), FIELD_LENGTH);

	free(line);
	return ur;
}

void ur_delete(struct userrecord* ur)
{
	if (ur) {
		free(ur);
	}
}
