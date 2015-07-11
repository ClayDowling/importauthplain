#ifndef _IMPORTPLAIN_H_
#define _IMPORTPLAIN_H_

#define FIELD_LENGTH 80
#define MAX_GROUPS 10
#define MAX_GLOBAL_GROUPS 50

extern char global_groups[MAX_GLOBAL_GROUPS][FIELD_LENGTH];

struct userrecord {
	char login[FIELD_LENGTH];
	char password[FIELD_LENGTH];
	char name[FIELD_LENGTH];
	char email[FIELD_LENGTH];
	char groups[MAX_GROUPS][FIELD_LENGTH];
};

struct userrecord* ur_parse(const char*);
void ur_delete(struct userrecord*);

/* Global group functions */
int ur_group_seen(const char*);

#endif
