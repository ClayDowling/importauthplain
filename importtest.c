#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest.h"
#include "importplain.h"

const char *TEST_RECORD = "clay:$1$7qW3giu7$gf5SRGyb:Clay Dowling:bogus@lazarusid.com:admin,user,facebook,google";
const char *TEST_RECORD_2 = "adelfa:$1$8qW4hit7$gf3SREif:Adelfa Dowling:fake@lazarusid.com:user,yahoo";
const char *TEST_RECORD_BAD_GROUPS = "badgroups:$1$8qW4hit7$gf3SREif:User With Bad Groups:noteven@lazarusid.com:user,user";

void setup()
{
    bzero(global_groups, sizeof(global_groups));
}

void test_parse_record_returns_user_record_with_valid_record(CuTest *tc)
{
	struct userrecord *ur;

    setup();
	ur = ur_parse(TEST_RECORD);
	CuAssertPtrNotNull(tc, ur);

	ur_delete(ur);
}

void test_parse_record_returns_filled_user_record(CuTest *tc)
{
	struct userrecord *ur;

    setup();
	ur = ur_parse(TEST_RECORD);
	CuAssertStrEquals(tc, "clay", ur->login);
	CuAssertStrEquals(tc, "$1$7qW3giu7$gf5SRGyb", ur->password);
	CuAssertStrEquals(tc, "Clay Dowling", ur->name);
	CuAssertStrEquals(tc, "bogus@lazarusid.com", ur->email);
	ur_delete(ur);
}

void test_parse_record_returns_correct_groups(CuTest *tc)
{
	struct userrecord *ur;

    setup();
    ur = ur_parse(TEST_RECORD);
	CuAssertStrEquals(tc, "admin", ur->groups[0]);
	CuAssertStrEquals(tc, "user", ur->groups[1]);
	CuAssertStrEquals(tc, "facebook", ur->groups[2]);
	CuAssertStrEquals(tc, "google", ur->groups[3]);
	CuAssertStrEquals(tc, "", ur->groups[4]);
	ur_delete(ur);
}

void test_parse_record_populates_global_groups_from_one_record(CuTest *tc)
{
    struct userrecord *ur;

    setup();
    ur = ur_parse(TEST_RECORD);
    ur_delete(ur);

    CuAssertIntEquals_Msg(tc, "Group admin not found", 1, ur_group_seen("admin"));
    CuAssertIntEquals_Msg(tc, "Group facebook not found", 1, ur_group_seen("facebook"));
    CuAssertIntEquals_Msg(tc, "Group google not found", 1, ur_group_seen("google"));
    CuAssertIntEquals_Msg(tc, "Group user not found", 1, ur_group_seen("user"));

}

void test_parse_record_populates_global_groups_from_two_records(CuTest *tc)
{
    struct userrecord *ur;

    setup();
    ur = ur_parse(TEST_RECORD);
    ur_delete(ur);
    ur = ur_parse(TEST_RECORD_2);
    ur_delete(ur);

    CuAssertIntEquals_Msg(tc, "Group admin not found", 1, ur_group_seen("admin"));
    CuAssertIntEquals_Msg(tc, "Group facebook not found", 1, ur_group_seen("facebook"));
    CuAssertIntEquals_Msg(tc, "Group google not found", 1, ur_group_seen("google"));
    CuAssertIntEquals_Msg(tc, "Group user not found", 1, ur_group_seen("user"));
    CuAssertIntEquals_Msg(tc, "Group yahoo not found", 1, ur_group_seen("yahoo"));

}

void test_parse_record_populates_only_one_copy_of_a_group(CuTest *tc)
{
    struct userrecord *ur;

    setup();
    ur = ur_parse(TEST_RECORD_BAD_GROUPS);
    ur_delete(ur);

    CuAssertStrEquals(tc, "user", global_groups[0]);
    CuAssertStrEquals_Msg(tc, "Group found, should be empty.", "", global_groups[1]);
}

int main(void)
{
	CuString *output = CuStringNew();
	CuSuite *suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, test_parse_record_returns_user_record_with_valid_record);
	SUITE_ADD_TEST(suite, test_parse_record_returns_filled_user_record);
	SUITE_ADD_TEST(suite, test_parse_record_returns_correct_groups);
    SUITE_ADD_TEST(suite, test_parse_record_populates_global_groups_from_one_record);
    SUITE_ADD_TEST(suite, test_parse_record_populates_global_groups_from_two_records);
    SUITE_ADD_TEST(suite, test_parse_record_populates_only_one_copy_of_a_group);

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);

	return EXIT_SUCCESS;
}
