/**
 * Test cases for the jep_utils library.
 *
 * Author: John Powell
 */

#include "jep_utils_tests.h"

int jep_create_byte_buffer_tests(jep_byte_buffer* bb)
{
	int passed = 0;

	int bb_not_null = 0;
	int bb_buffer_not_null = 0;
	int bb_size_0 = 0;
	int bb_cap_50 = 0;

	if (bb != NULL)
	{
		passed++;
		bb_not_null = 1;
	}

	if (bb != NULL && bb->buffer != NULL)
	{
		passed++;
		bb_buffer_not_null = 1;
	}

	if (bb != NULL && bb->size == 0)
	{
		passed++;
		bb_size_0 = 1;
	}

	if (bb != NULL && bb->cap == 50)
	{
		passed++;
		bb_cap_50 = 1;
	}

	printf(JEP_TEST_REPORT_SEPARATOR);
	printf("jep_create_byte_buffer tests\n");
	printf(JEP_TEST_REPORT_SEPARATOR);
	printf(JEP_TEST_REPORT_RESULT("bb != NULL", bb_not_null));
	printf(JEP_TEST_REPORT_RESULT("bb->buffer != NULL", bb_buffer_not_null));
	printf(JEP_TEST_REPORT_RESULT("bb->size == 0", bb_size_0));
	printf(JEP_TEST_REPORT_RESULT("bb->cap == 50", bb_cap_50));


	return passed;
}

int jep_append_byte_tests(jep_byte_buffer* bb)
{
	int passed = 0;

	int bb_size_1 = 0;
	int bb_cap_50 = 0;

	if (bb->size == 1)
	{
		passed++;
		bb_size_1 = 1;
	}

	if (bb->cap == 50)
	{
		passed++;
		bb_cap_50 = 1;
	}

	printf(JEP_TEST_REPORT_SEPARATOR);
	printf("jep_append_bytes tests\n");
	printf(JEP_TEST_REPORT_SEPARATOR);
	printf(JEP_TEST_REPORT_RESULT("bb->size == 1", bb_size_1));
	printf(JEP_TEST_REPORT_RESULT("bb->cap == 50", bb_cap_50));

	return passed;
}

int jep_append_bytes_tests(jep_byte_buffer* bb)
{
	int passed = 0;

	int bb_size_51 = 0;
	int bb_cap_75 = 0;

	if (bb->size == 51)
	{
		passed++;
		bb_size_51 = 1;
	}

	if (bb->cap == 75)
	{
		passed++;
		bb_cap_75 = 1;
	}

	printf(JEP_TEST_REPORT_SEPARATOR);
	printf("jep_append_bytes tests\n");
	printf(JEP_TEST_REPORT_SEPARATOR);
	printf(JEP_TEST_REPORT_RESULT("bb->size == 51", bb_size_51));
	printf(JEP_TEST_REPORT_RESULT("bb->cap == 75", bb_cap_75));

	return passed;
}

int jep_create_char_buffer_tests(jep_char_buffer* cb)
{
	int passed = 0;

	int cb_not_null = 0;
	int cb_buffer_not_null = 0;
	int cb_size_0 = 0;
	int cb_cap_50 = 0;

	if (cb != NULL)
	{
		passed++;
		cb_not_null = 1;
	}

	if (cb != NULL && cb->buffer != NULL)
	{
		passed++;
		cb_buffer_not_null = 1;
	}

	if (cb != NULL && cb->size == 0)
	{
		passed++;
		cb_size_0 = 1;
	}

	if (cb != NULL && cb->cap == 50)
	{
		passed++;
		cb_cap_50 = 1;
	}

	printf(JEP_TEST_REPORT_SEPARATOR);
	printf("jep_create_char_buffer tests\n");
	printf(JEP_TEST_REPORT_SEPARATOR);
	printf(JEP_TEST_REPORT_RESULT("cb != NULL", cb_not_null));
	printf(JEP_TEST_REPORT_RESULT("cb->buffer != NULL", cb_buffer_not_null));
	printf(JEP_TEST_REPORT_RESULT("cb->size == 0", cb_size_0));
	printf(JEP_TEST_REPORT_RESULT("cb->cap == 50", cb_cap_50));

	return passed;
}

int jep_append_char_tests(jep_char_buffer* cb)
{
	int passed = 0;

	int cb_size_1 = 0;
	int cb_cap_50 = 0;

	if (cb->size == 1)
	{
		passed++;
		cb_size_1 = 1;
	}

	if (cb->cap == 50)
	{
		passed++;
		cb_cap_50 = 1;
	}

	printf(JEP_TEST_REPORT_SEPARATOR);
	printf("jep_append_char tests\n");
	printf(JEP_TEST_REPORT_SEPARATOR);
	printf(JEP_TEST_REPORT_RESULT("cb->size == 1", cb_size_1));
	printf(JEP_TEST_REPORT_RESULT("cb->cap == 50", cb_cap_50));

	return passed;
}

int jep_append_chars_tests(jep_char_buffer* cb)
{
	int passed = 0;

	int cb_size_51 = 0;
	int cb_cap_75 = 0;

	if (cb->size == 51)
	{
		passed++;
		cb_size_51 = 1;
	}

	if (cb->cap == 75)
	{
		passed++;
		cb_cap_75 = 1;
	}

	printf(JEP_TEST_REPORT_SEPARATOR);
	printf("jep_append_chars tests\n");
	printf(JEP_TEST_REPORT_SEPARATOR);
	printf(JEP_TEST_REPORT_RESULT("cb->size == 51", cb_size_51));
	printf(JEP_TEST_REPORT_RESULT("cb->cap == 75", cb_cap_75));

	return passed;
}

int jep_create_bitstring_test(jep_bitstring* bs)
{
	int passed = 0;

	int bs_not_null = 0;
	int bs_bytes_not_null = 0;
	int bs_byte_count_0 = 0;
	int bs_bit_count = 0;
	int bs_current_bits = 0;

	if (bs != NULL)
	{
		passed++;
		bs_not_null = 1;
	}

	if (bs != NULL && bs->bytes != NULL)
	{
		passed++;
		bs_bytes_not_null = 1;
	}

	if (bs != NULL && bs->bit_count == 0)
	{
		passed++;
		bs_bit_count = 1;
	}

	if (bs != NULL && bs->byte_count == 1)
	{
		passed++;
		bs_byte_count_0 = 1;
	}

	if (bs != NULL && bs->current_bits == 0)
	{
		passed++;
		bs_current_bits = 1;
	}

	printf(JEP_TEST_REPORT_SEPARATOR);
	printf("jep_create_bitstring tests\n");
	printf(JEP_TEST_REPORT_SEPARATOR);
	printf(JEP_TEST_REPORT_RESULT("bs != NULL", bs_not_null));
	printf(JEP_TEST_REPORT_RESULT("bs->bytes != NULL", bs_bytes_not_null));
	printf(JEP_TEST_REPORT_RESULT("bs->bit_count == 0", bs_byte_count_0));
	printf(JEP_TEST_REPORT_RESULT("bs->byte_count == 1", bs_bit_count));
	printf(JEP_TEST_REPORT_RESULT("bs->current_bits == 0", bs_current_bits));

	return passed;
}
