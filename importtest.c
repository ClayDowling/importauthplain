#include <stdio.h>
#include <stdlib.h>
#include "CuTest.h"
#include "importplain.h"

const char *TEST_RECORD = "clay:$1$7qW3giu7$gf5SRGyb:Clay Dowling:bogus@lazarusid.com:admin,user,facebook,google";

void test_parse_record_returns_user_record_with_valid_record(CuTest *tc)
{
	struct userrecord *ur;

	ur = ur_parse(TEST_RECORD);
	CuAssertPtrNotNull(tc, ur);

	ur_delete(ur);
}

void test_parse_record_returns_filled_user_record(CuTest *tc)
{
	struct userrecord *ur;

	ur = ur_parse(TEST_RECORD);
	CuAssertStrEquals(tc, "clay", ur->login);
	CuAssertStrEquals(tc, "$1$7qW3giu7$gf5SRGyb", ur->password);
	CuAssertStrEquals(tc, "Clay Dowling", ur->name);
	CuAssertStrEquals(tc, "bogus@lazarusid.com", ur->email);
	ur_delete(ur);
}


int main(void)
{
	CuString *output = CuStringNew();
	CuSuite *suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, test_parse_record_returns_user_record_with_valid_record);
	SUITE_ADD_TEST(suite, test_parse_record_returns_filled_user_record);

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);

	return EXIT_SUCCESS;
}
