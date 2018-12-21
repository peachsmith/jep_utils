#include "jep_utils.h"

/**********************
* byte buffer         *
**********************/

JEP_UTILS_API jep_byte_buffer* JEP_UTILS_CALL
jep_create_byte_buffer()
{

	jep_byte_buffer* bb = NULL;

	bb = (jep_byte_buffer*)malloc(sizeof(jep_byte_buffer));

	if (bb == NULL)
		return NULL;

	size_t cap = 50;

	bb->cap = cap;
	bb->size = 0;
	bb->buffer = (unsigned char*)malloc(sizeof(unsigned char) * cap);

	if (bb->buffer == NULL)
	{
		free(bb);
		return NULL;
	}

	return bb;
}

JEP_UTILS_API void JEP_UTILS_CALL
jep_destroy_byte_buffer(jep_byte_buffer* bb)
{
	if (bb == NULL)
		return;

	if (bb->buffer != NULL)
		free(bb->buffer);

	free(bb);
}

JEP_UTILS_API int JEP_UTILS_CALL
jep_append_byte(jep_byte_buffer* bb, unsigned char b)
{
	if (bb == NULL || bb->buffer == NULL)
		return 0;

	if (bb->size >= bb->cap)
	{
		size_t new_size = bb->size + bb->size / 2;
		bb->buffer = (unsigned char*)realloc(bb->buffer, new_size);
	}

	if (bb->buffer == NULL)
		return 0;

	bb->buffer[bb->size++] = b;

	return 1;
}

JEP_UTILS_API int JEP_UTILS_CALL
jep_append_bytes(jep_byte_buffer* bb, unsigned char* b, int n)
{
	int i, result = 0;

	for (i = 0; i < n; i++)
		result += jep_append_byte(bb, b[i]);

	return result;
}


/**********************
* character buffer   *
**********************/

JEP_UTILS_API jep_char_buffer* JEP_UTILS_CALL
jep_create_char_buffer()
{

	jep_char_buffer* cb = NULL;

	cb = (jep_char_buffer*)malloc(sizeof(jep_char_buffer));

	if (cb == NULL)
		return NULL;

	size_t cap = 50;

	cb->cap = cap;
	cb->size = 0;
	cb->buffer = (char*)malloc(sizeof(char) * cap);

	if (cb->buffer == NULL)
	{
		free(cb);
		return NULL;
	}

	return cb;
}

JEP_UTILS_API void JEP_UTILS_CALL
jep_destroy_char_buffer(jep_char_buffer* cb)
{
	if (cb == NULL)
		return;

	if (cb->buffer != NULL)
		free(cb->buffer);

	free(cb);
}

JEP_UTILS_API int JEP_UTILS_CALL
jep_append_char(jep_char_buffer* cb, char c)
{
	if (cb == NULL || cb->buffer == NULL)
		return 0;

	if (cb->size >= cb->cap)
	{
		size_t new_size = cb->size + cb->size / 2;
		cb->buffer = (char*)realloc(cb->buffer, new_size);
	}

	if (cb->buffer == NULL)
		return 0;

	cb->buffer[cb->size++] = c;

	return 1;
}

JEP_UTILS_API int JEP_UTILS_CALL
jep_append_chars(jep_char_buffer* cb, char* c, int n)
{
	int i, result = 0;

	for (i = 0; i < n; i++)
		result += jep_append_char(cb, c[i]);

	return result;
}
