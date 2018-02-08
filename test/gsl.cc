/*-------------------------------------------------------------------
GSL Inclusion Check

Author: Branden Allen
Date  : 2018.05.07
-------------------------------------------------------------------*/
#include <check.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_sf_bessel.h>

using namespace std;

START_TEST(g4_check) {
	double x= 5.0; 
	double y= gsl_sf_bessel_J0(x);
	double z= gsl_sf_bessel_J0(x);
	ck_assert(z== y);
} END_TEST

START_TEST(g4_fail) {
	double k= 0;
	double g= 1;
	ck_abort();
	ck_assert(k== g);
	ck_assert_uint_eq(1,2);
} END_TEST

START_TEST(g4_mega_fail) {
	ck_assert(strcmp("hi","bye")== 0);
} END_TEST

Suite *attempt(void) {
	Suite *s;
	TCase *tc_core, *tc_limits;

	s= suite_create("g4");
	tc_core= tcase_create("Core");
	tcase_add_test(tc_core, g4_check);
	tcase_add_test(tc_core, g4_fail);
	tcase_add_test(tc_core, g4_mega_fail);
	suite_add_tcase(s, tc_core);

	return s;
}

int main(void) {
	int nfail;
	Suite *s; 
	SRunner *sr;

	s= attempt();
	sr= srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	nfail= srunner_ntests_failed(sr);
	srunner_free(sr);
	return (nfail== 2) ? EXIT_SUCCESS : EXIT_FAILURE;
}
