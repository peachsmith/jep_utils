/**
 * jep_utils
 *
 * Author: John Powell
 */

#include "jep_utils.h"

/* ------------------------------------------------- */
/*                   byte buffer                     */
/* ------------------------------------------------- */

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
		size_t new_cap = bb->size + bb->size / 2;
		unsigned char* buffer = (unsigned char*)realloc(bb->buffer, new_cap);

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
jep_append_bytes(jep_byte_buffer* bb, unsigned char* b, int n)
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

	unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char));

	if (buffer == NULL)
		return 0;
		
	bb->buffer = buffer;
	
	return 1;
}

/* ------------------------------------------------- */
/*                character buffer                   */
/* ------------------------------------------------- */

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
		size_t new_cap = cb->size + cb->size / 2;
		char* buffer = (char*)realloc(cb->buffer, new_cap);

		if (buffer == NULL)
			return -1;

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
jep_append_chars(jep_char_buffer* cb, char* c, int n)
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
	size_t i;

	for (i = 0; i < cb->size; i++)
	{
		if (removed)
		{
			if (i > 0)
				cb->buffer[i - 1] = cb->buffer[i];

			if (i == cb->size - 1)
				cb->buffer[i] = '\0';
		}

		if (i == index)
			removed = 1;
	}

	if (removed)
	{
		if (cb->size == 1)
			cb->buffer[0] = '\0';

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

	char* buffer = (char*)malloc(sizeof(char));

	if (buffer == NULL)
		return 0;

	cb->buffer = buffer;

	return 1;
}

/* ------------------------------------------------- */
/*                    bitstring                      */
/* ------------------------------------------------- */

JEP_UTILS_API jep_bitstring* JEP_UTILS_CALL
jep_create_bitstring()
{
	jep_bitstring* bs;

	bs = (jep_bitstring*)malloc(sizeof(jep_bitstring));

	if (bs == NULL)
		return NULL;

	bs->bit_count = 0;
	bs->byte_count = 1;
	bs->current_bits = 0;

	unsigned char* bytes = (unsigned char*)malloc(sizeof(unsigned char));

	if (bytes == NULL)
	{
		free(bs);
		return NULL;
	}

	bs->bytes = bytes;
	bs->bytes[0] = 0;

	return bs;
}

JEP_UTILS_API void JEP_UTILS_CALL
jep_destroy_bitstring(jep_bitstring* bs)
{
	if (bs == NULL)
		return;

	if (bs->bytes != NULL)
		free(bs->bytes);

	free(bs);
}

JEP_UTILS_API int JEP_UTILS_CALL
jep_add_bit(jep_bitstring* bs, unsigned int bit)
{
	if (bit != 0 && bit != 1)
		return 0;

	if (bs->current_bits == CHAR_BIT)
	{
		unsigned char* b = (unsigned char*)realloc(bs->bytes, bs->byte_count + 1);

		if (b == NULL)
			return 0;

		bs->bytes = b;
		bs->byte_count++;
		bs->current_bits = 0;
		bs->bytes[bs->byte_count - 1] = 0;
	}

	bs->bytes[bs->byte_count - 1] |= (bit << bs->current_bits++);

	bs->bit_count++;

	return 1;
}

JEP_UTILS_API int JEP_UTILS_CALL
jep_add_bits(jep_bitstring* dest, jep_bitstring* src)
{
	unsigned long i;
	unsigned long bit;
	unsigned long byte;
	int next_result;
	int final_result;

	bit = byte = final_result = 0;
	next_result = 1;

	for (i = 0; i < src->bit_count && next_result; i++)
	{
		if (bit == CHAR_BIT)
		{
			byte++;
			bit = 0;
		}

		next_result = 0;
		unsigned int next_bit = 0;

		if (src->bytes[byte] & (1 << bit++))
			next_bit = 1;
		
		next_result = jep_add_bit(dest, next_bit);

		if (next_result)
			final_result += next_result;
	}

	return final_result;
}

JEP_UTILS_API int JEP_UTILS_CALL
jep_get_bit(jep_bitstring* bs, int index)
{
	int res = -1;
	unsigned long byte = 0;
	unsigned long bit = 0;
	unsigned long i;
	
	if ((unsigned int)index + 1 > bs->bit_count)
		return -1;

	for (i = 0; i < bs->bit_count; i++)
	{
		if (bit == CHAR_BIT)
		{
			byte++;
			bit = 0;
		}

		if (byte * CHAR_BIT + bit == index)
		{
			if (bs->bytes[byte] & (1 << bit))
				res = 1;
			else
				res = 0;
		}

		bit++;
	}

	return res;
}

JEP_UTILS_API void JEP_UTILS_CALL
jep_set_bit(jep_bitstring* bs, int index, unsigned int value)
{
	unsigned long byte = 0;
	unsigned long bit = 0;
	unsigned long i;

	if (bit < 0 || bit > 1)
		return;

	if ((unsigned int)index + 1 > bs->bit_count)
		return;

	for (i = 0; i < bs->bit_count; i++)
	{
		if (bit == CHAR_BIT)
		{
			byte++;
			bit = 0;
		}

		if (byte * CHAR_BIT + bit == index)
		{
			if (bs->bytes[byte] & (1 << bit) && !value)
			{
				bs->bytes[byte] ^= (1 << bit);
			}
			else if (!(bs->bytes[byte] & (1 << bit)) && value)
			{
				bs->bytes[byte] |= (1 << bit);
			}
		}

		bit++;
	}
}
