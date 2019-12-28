#include "char_buffer_tests.h"

int char_buffer_create_test()
{
	jep_char_buffer* cb;

	cb = jep_create_char_buffer();

	if (cb == NULL)
		return 0;

	jep_destroy_char_buffer(cb);

	return 1;
}

int char_buffer_append_char_test()
{
	int res;
	jep_char_buffer* cb;
	jep_char c = 0x65;

	cb = jep_create_char_buffer();

	if (cb == NULL)
		return 0;

	res = jep_append_char(cb, c);

	if (cb->buffer[0] != 0x65)
		res = 0;

	jep_destroy_char_buffer(cb);

	return res;
}

int char_buffer_append_chars_test()
{
	int res;
	jep_char_buffer* cb;
	jep_char c[3] = { 0x65, 0x66, 0x67 };

	cb = jep_create_char_buffer();

	if (cb == NULL)
		return 0;

	res = jep_append_chars(cb, c, 3);

	jep_destroy_char_buffer(cb);

	return res == 3 ? 1 : 0;
}