#include "jep_utils/jep_utils.h"
#include "jep_utils/byte_buffer.h"




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
	bb->buffer = (jep_byte*)malloc(sizeof(jep_byte) * cap);

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
jep_append_byte(jep_byte_buffer* bb, jep_byte b)
{
	if (bb == NULL || bb->buffer == NULL)
		return 0;

	if (bb->size >= bb->cap)
	{
		size_t new_cap = bb->size + bb->size / 2;
		jep_byte* buffer = (jep_byte*)realloc(bb->buffer, new_cap);

		if (buffer == NULL)
			return -1;

		bb->buffer = buffer;
		bb->cap = new_cap;
	}

	if (bb->buffer == NULL)
	{
		bb->cap = 0;
		bb->size = 0;
		return 0;
	}

	bb->buffer[bb->size++] = b;

	return 1;
}

JEP_UTILS_API int JEP_UTILS_CALL
jep_append_bytes(jep_byte_buffer* bb, jep_byte* b, int n)
{
	int i, result = 0;

	for (i = 0; i < n; i++)
	{
		int next_result = jep_append_byte(bb, b[i]);
		if (next_result > 0)
			result += next_result;
		else
			return next_result;
	}


	return result;
}

JEP_UTILS_API void JEP_UTILS_CALL
jep_remove_byte_at(jep_byte_buffer* bb, size_t index)
{
	if (bb == NULL || bb->buffer == NULL || bb->size == 0)
		return;

	if (index >= bb->size)
		return;

	int removed = 0;
	size_t i;

	for (i = 0; i < bb->size; i++)
	{
		if (removed)
		{
			if (i > 0)
				bb->buffer[i - 1] = bb->buffer[i];

			if (i == bb->size - 1)
				bb->buffer[i] = 0;
		}

		if (i == index)
			removed = 1;
	}

	if (removed)
	{
		if (bb->size == 1)
			bb->buffer[0] = 0;

		bb->size--;
	}
}

JEP_UTILS_API int JEP_UTILS_CALL
jep_clear_byte_buffer(jep_byte_buffer* bb)
{
	if (bb == NULL || bb->buffer == NULL || bb->size == 0)
		return 0;

	free(bb->buffer);
	bb->buffer = NULL;
	bb->size = 0;

	jep_byte* buffer = (jep_byte*)malloc(sizeof(jep_byte));

	if (buffer == NULL)
		return 0;

	bb->buffer = buffer;

	return 1;
}
