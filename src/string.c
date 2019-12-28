#include "jep_utils/jep_utils.h"
#include "jep_utils/string.h"




JEP_UTILS_API jep_string* JEP_UTILS_CALL
jep_create_string(size_t size)
{
	jep_string* str;
	jep_char* chars;

	str = (jep_string*)malloc(sizeof(jep_string));

	if (str == NULL)
		return NULL;

	str->size = size;
	str->chars = NULL;

	if (size > 0)
	{
		chars = (jep_char*)malloc(sizeof(jep_char) * size);

		if (chars == NULL)
		{
			free(str);
			return NULL;
		}

		str->chars = chars;
	}

	return str;
}

JEP_UTILS_API void JEP_UTILS_CALL
jep_destroy_string(jep_string* str)
{
	if (str == NULL)
		return;

	if (str->chars != NULL)
		free(str->chars);

	free(str);
}

JEP_UTILS_API jep_string* JEP_UTILS_CALL
jep_bytes_to_string(const char* bytes, int encoding, size_t n)
{
	jep_string* str;
	jep_code_point* pts;
	size_t res;

	res = 0;
	pts = NULL;
	str = jep_create_string(0);

	if (str == NULL)
		return NULL;

	switch (encoding)
	{
	case JEP_ENCODING_UTF_8:
		pts = jep_utf8_decode(bytes, n, &res);
		break;

	case JEP_ENCODING_UTF_16:
		pts = jep_utf16_decode(bytes, n, &res);
		break;

	case JEP_ENCODING_UTF_16_BE:
		pts = jep_utf16be_decode(bytes, n, &res);
		break;

	case JEP_ENCODING_UTF_16_LE:
		pts = jep_utf16le_decode(bytes, n, &res);
		break;

	default:
		break;
	}

	if (pts == NULL)
	{
		jep_destroy_string(str);
		return NULL;
	}

	if (res == 0)
	{
		jep_destroy_string(str);
		free(pts);
		return NULL;
	}

	str->size = res;
	str->chars = pts;

	return str;
}

JEP_UTILS_API jep_string* JEP_UTILS_CALL
jep_chars_to_string(jep_char* chars, size_t n)
{
	jep_string* str;
	size_t i;

	str = jep_create_string(n);

	if (str == NULL)
		return NULL;

	for (i = 0; i < n; i++)
	{
		str->chars[i] = chars[i];
	}

	return str;
}

JEP_UTILS_API int JEP_UTILS_CALL
jep_strcmp(jep_string* a, jep_string* b)
{
	if (a == NULL && b == NULL)
		return 0;

	if (a != NULL && b == NULL)
		return 1;

	if (a == NULL && b != NULL)
		return -1;

	if (a->size > b->size)
		return 1;

	if (a->size < b->size)
		return -1;

	size_t i;
	for (i = 0; i < a->size; i++)
	{
		if (a->chars[i] < b->chars[i]) return -1;
		else if (a->chars[i] > b->chars[i]) return 1;
	}

	return 0;
}

JEP_UTILS_API int JEP_UTILS_CALL
jep_strcpy(jep_string* src, jep_string* dest)
{
	if (src == NULL || dest == NULL)
		return 0;

	if (src->chars == NULL)
		return 0;

	if (src->size == 0)
		return 0;

	size_t i;
	jep_char* dchars;
	jep_char* nchars;

	dchars = dest->chars;

	// make sure we have a destination buffer
	if (dest->chars == NULL)
	{
		dest->chars = (jep_char*)malloc(sizeof(jep_char) * src->size);

		if (dest->chars == NULL)
			return 0;

		dest->size = src->size;
	}

	// make sure that the destination buffer size is equal to the
	// source buffer size.
	if (src->size != dest->size)
	{
		nchars = jep_realloc(dest->chars, jep_char, src->size);

		if (nchars != NULL)
			dest->chars = nchars;
		else
			return 0;

		dest->size = src->size;
	}

	for (i = 0; i < src->size; i++)
	{
		dest->chars[i] = src->chars[i];
	}

	return 1;
}

JEP_UTILS_API jep_string* JEP_UTILS_CALL
jep_new_string(const char* lit)
{
	return jep_bytes_to_string(lit, JEP_ENCODING_UTF_8, strlen(lit));
}

JEP_UTILS_API long JEP_UTILS_CALL
jep_string_to_long(jep_string* str, int base)
{
	size_t i;
	size_t size;
	jep_char* raw;
	long l;
	char* end;
	char* buffer;

	if (str == NULL)
		return 0;

	l = 0;
	size = str->size;
	raw = str->chars;

	if (size < 1)
		return 0;

	buffer = (char*)malloc(sizeof(char) * size + 1);

	if (buffer == NULL)
		return 0;

	for (i = 0; i < size; i++)
	{
		//if (raw[i].b3 == 0 && raw[i].b2 == 0)
		if (raw[i] <= 0x000000FF)
			buffer[i] = (str->chars[i] & 0xFF);
		else
			buffer[i] = '\0';
	}

	buffer[i] = '\0';

	l = strtol(buffer, &end, base);

	free(buffer);

	return l;
}

JEP_UTILS_API unsigned long JEP_UTILS_CALL
jep_string_to_ulong(jep_string* str, int base)
{
	size_t i;
	size_t size;
	jep_char* raw;
	unsigned long l;
	char* end;
	char* buffer;

	if (str == NULL)
		return 0;

	l = 0;
	size = str->size;
	raw = str->chars;

	if (size < 1)
		return 0;

	buffer = (char*)malloc(sizeof(char) * size + 1);

	if (buffer == NULL)
		return 0;

	for (i = 0; i < size; i++)
	{
		//if (raw[i].b3 == 0 && raw[i].b2 == 0)
		if (raw[i] <= 0x000000FF)
			buffer[i] = (str->chars[i] & 0xFF);
		else
			buffer[i] = '\0';
	}

	buffer[i] = '\0';

	l = strtoul(buffer, &end, base);

	free(buffer);

	return l;
}

JEP_UTILS_API int JEP_UTILS_CALL
jep_string_to_int(jep_string* str, int base)
{
	size_t i;
	size_t size;
	jep_char* raw;
	long l;
	int n;
	char* end;
	char* buffer;

	if (str == NULL)
		return 0;

	l = 0;
	size = str->size;
	raw = str->chars;

	if (size < 1)
		return 0;

	buffer = (char*)malloc(sizeof(char) * size + 1);

	if (buffer == NULL)
		return 0;

	for (i = 0; i < size; i++)
	{
		//if (raw[i].b3 == 0 && raw[i].b2 == 0)
		if (raw[i] <= 0x000000FF)
			buffer[i] = (str->chars[i] & 0xFF);
		else
			buffer[i] = '\0';
	}

	buffer[i] = '\0';

	l = strtol(buffer, &end, base);

	free(buffer);

	n = l >= INT_MIN && l <= INT_MAX ? l : 0;

	return n;
}

JEP_UTILS_API unsigned int JEP_UTILS_CALL
jep_string_to_uint(jep_string* str, int base)
{
	size_t i;
	size_t size;
	jep_char* raw;
	unsigned long l;
	unsigned int n;
	char* end;
	char* buffer;

	if (str == NULL)
		return 0;

	l = 0;
	size = str->size;
	raw = str->chars;

	if (size < 1)
		return 0;

	buffer = (char*)malloc(sizeof(char) * size + 1);

	if (buffer == NULL)
		return 0;

	for (i = 0; i < size; i++)
	{
		//if (raw[i].b3 == 0 && raw[i].b2 == 0)
		if (raw[i] <= 0x000000FF)
			buffer[i] = (str->chars[i] & 0xFF);
		else
			buffer[i] = '\0';
	}

	buffer[i] = '\0';

	l = strtoul(buffer, &end, base);

	free(buffer);

	n = l >= 0 && l <= UINT_MAX ? l : 0;

	return n;
}


JEP_UTILS_API double JEP_UTILS_CALL
jep_string_to_double(jep_string* str)
{
	size_t i;
	size_t size;
	jep_char* raw;
	double d;
	char* end;
	char* buffer;

	if (str == NULL)
		return 0;

	d = 0;
	size = str->size;
	raw = str->chars;

	if (size < 1)
		return 0;

	buffer = (char*)malloc(sizeof(char) * size + 1);

	if (buffer == NULL)
		return 0;

	for (i = 0; i < size; i++)
	{
		//if (raw[i].b3 == 0 && raw[i].b2 == 0)
		if (raw[i] <= 0x000000FF)
			buffer[i] = (str->chars[i] & 0xFF);
		else
			buffer[i] = '\0';
	}

	buffer[i] = '\0';

	d = strtod(buffer, &end);

	free(buffer);

	return d;
}
