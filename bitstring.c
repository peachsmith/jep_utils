#include "jep_utils.h"




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

	jep_byte* bytes = (jep_byte*)malloc(sizeof(jep_byte));

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
		jep_byte* b = (jep_byte*)realloc(bs->bytes, bs->byte_count + 1);

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

JEP_UTILS_API int JEP_UTILS_CALL
jep_pop_bit(jep_bitstring* bs)
{
	if (bs == NULL || bs->bit_count < 1)
		return 0;

	bs->bytes[bs->byte_count - 1] &= ~(1 << (bs->current_bits - 1));

	bs->current_bits--;

	if (bs->current_bits == 0 && bs->byte_count > 1)
	{
		jep_byte* bytes = (jep_byte*)realloc(bs->bytes, bs->byte_count - 1);

		if (bytes == NULL)
			return 0;

		bs->bytes = bytes;
		bs->byte_count--;
		bs->current_bits = CHAR_BIT;
	}

	bs->bit_count--;

	return 1;
}
