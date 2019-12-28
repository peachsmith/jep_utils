#include "character_tests.h"

int char_compare_less_test()
{
	jep_char a = 0x00;
	jep_char b = 0x01;

	return jep_char_cmp(a, b) == -1 ? 1 : 0;
}

int char_compare_greater_test()
{
	jep_char a = 0x01;
	jep_char b = 0x00;

	return jep_char_cmp(a, b) == 1 ? 1 : 0;
}

int char_compare_equal_test()
{
	jep_char a = 0x00;
	jep_char b = 0x00;

	return jep_char_cmp(a, b) == 0 ? 1 : 0;
}
