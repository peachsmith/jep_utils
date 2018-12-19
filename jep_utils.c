#include "jep_utils.h"

JEP_UTILS_API jep_byte_buffer* JEP_UTILS_CALL
jep_create_byte_buffer()
{

	jep_byte_buffer* bb = NULL;

	bb = (jep_byte_buffer*)malloc(sizeof(jep_byte_buffer));

	if (bb == NULL)
		return NULL;

	bb->cap = 50;
	bb->size = 0;
	bb->buffer = (unsigned char*)malloc(sizeof(unsigned char));

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
