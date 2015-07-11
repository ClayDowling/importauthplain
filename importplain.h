#ifndef _IMPORTPLAIN_H_
#define _IMPORTPLAIN_H_

#define FIELD_LENGTH 80
#define MAX_GROUPS 10

struct userrecord {
	char login[FIELD_LENGTH];
	char password[FIELD_LENGTH];
	char name[FIELD_LENGTH];
	char email[FIELD_LENGTH];
	char groups[MAX_GROUPS][FIELD_LENGTH];
};

struct userrecord* ur_parse(const char*);
void ur_delete(struct userrecord*);

#endif
