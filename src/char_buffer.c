#include "jep_utils/jep_utils.h"
#include "jep_utils/char_buffer.h"




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
	cb->buffer = (jep_char*)malloc(sizeof(jep_char) * cap);

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
jep_append_char(jep_char_buffer* cb, jep_char c)
{
	if (cb == NULL || cb->buffer == NULL)
		return 0;

	if (cb->size >= cb->cap)
	{
		size_t new_cap = cb->size + cb->size / 2;
		jep_char* buffer = (jep_char*)realloc(cb->buffer, new_cap);

		if (buffer == NULL)
			return 0;

		cb->buffer = buffer;
		cb->cap = new_cap;
	}

	if (cb->buffer == NULL)
	{
		cb->cap = 0;
		cb->size = 0;
		return 0;
	}

	cb->buffer[cb->size++] = c;

	return 1;
}

JEP_UTILS_API int JEP_UTILS_CALL
jep_append_chars(jep_char_buffer* cb, jep_char* c, int n)
{
	int i, result = 0;

	for (i = 0; i < n; i++)
	{
		int next_result = jep_append_char(cb, c[i]);
		if (next_result > 0)
			result += next_result;
		else
			return next_result;
	}


	return result;
}

JEP_UTILS_API void JEP_UTILS_CALL
jep_remove_char_at(jep_char_buffer* cb, size_t index)
{
	if (cb == NULL || cb->buffer == NULL || cb->size == 0)
		return;

	if (index >= cb->size)
		return;

	int removed = 0;
	jep_char nul = 0x00;
	size_t i;

	for (i = 0; i < cb->size; i++)
	{
		if (removed)
		{
			if (i > 0)
				cb->buffer[i - 1] = cb->buffer[i];

			if (i == cb->size - 1)
				cb->buffer[i] = nul;
		}

		if (i == index)
			removed = 1;
	}

	if (removed)
	{
		if (cb->size == 1)
			cb->buffer[0] = nul;

		cb->size--;
	}
}

JEP_UTILS_API int JEP_UTILS_CALL
jep_clear_char_buffer(jep_char_buffer* cb)
{
	if (cb == NULL || cb->buffer == NULL || cb->size == 0)
		return 0;

	free(cb->buffer);
	cb->buffer = NULL;
	cb->size = 0;

	jep_char* buffer = (jep_char*)malloc(sizeof(jep_char));

	if (buffer == NULL)
		return 0;

	cb->buffer = buffer;

	return 1;
}
