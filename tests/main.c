/**
 * Test program to validate the functionality of the jep_utils library.
 *
 * Author: John Powell
 */

#include "jep_utils_tests.h"

int main(int argc, char** argv)
{
	int i;

	/* test data */
	unsigned char test_bytes[JEP_TEST_BUFFER_SIZE];
	char test_chars[JEP_TEST_BUFFER_SIZE];

	/* test results */
	int passes = 0;
	int create_byte_buffer_passes = 0;
	int append_byte_passes = 0;
	int append_bytes_passes = 0;
	int create_char_buffer_passes = 0;
	int append_char_passes = 0;
	int append_chars_passes = 0;

	jep_byte_buffer* bb;
	jep_char_buffer* cb;

	for (i = 0; i < JEP_TEST_BUFFER_SIZE; i++)
	{
		test_bytes[i] = (unsigned char)'a';
		test_chars[i] = 'a';
	}

	bb = jep_create_byte_buffer();
	cb = jep_create_char_buffer();


	/* ------------------------------------------------- */
	/*             jep_byte_buffer tests                 */
	/* ------------------------------------------------- */

	/* assert byte buffer creation */
	create_byte_buffer_passes = jep_create_byte_buffer_tests(bb);

	/* append a single byte */
	jep_append_byte(bb, (unsigned char)'a');
	append_byte_passes = jep_append_byte_tests(bb);

	/* append multiple bytes */
	jep_append_bytes(bb, test_bytes, JEP_TEST_BUFFER_SIZE);
	append_bytes_passes = jep_append_bytes_tests(bb);


	/* ------------------------------------------------- */
	/*             jep_char_buffer tests                 */
	/* ------------------------------------------------- */

	/* assert character buffer creation */
	create_char_buffer_passes = jep_create_char_buffer_tests(cb);

	/* append a single character */
	jep_append_char(cb, 'a');
	append_char_passes = jep_append_char_tests(cb);

	/* append multiple characters */
	jep_append_chars(cb, test_chars, JEP_TEST_BUFFER_SIZE);
	append_chars_passes = jep_append_chars_tests(cb);


	/* ------------------------------------------------- */
	/*                     cleanup                       */
	/* ------------------------------------------------- */
	jep_destroy_char_buffer(cb);
	jep_destroy_byte_buffer(bb);

	/* ------------------------------------------------- */
	/*         tally and report the final results        */
	/* ------------------------------------------------- */
	passes += create_char_buffer_passes;
	passes += append_char_passes;
	passes += append_chars_passes;
	passes += create_byte_buffer_passes;
	passes += append_byte_passes;
	passes += append_bytes_passes;

	printf(JEP_TEST_REPORT_SEPARATOR);
	printf("Final Results: %d/%d tests passed\n", passes, JEP_TEST_EXPECTED);
	printf(JEP_TEST_REPORT_SEPARATOR);

	return 0;
}
