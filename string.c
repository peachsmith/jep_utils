#include "jep_utils.h"




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
jep_bytes_to_string(const jep_byte* bytes, int encoding, size_t n)
{
	jep_string* str;
	jep_code_point* pts;
	size_t res;

	str = jep_create_string(0);

	if (str == NULL)
		return NULL;

	if (encoding == JEP_ENCODING_UTF_8)
	{
		pts = jep_utf8_decode(bytes, n, &res);

		if (pts == NULL)
		{
			jep_destroy_string(str);
			return NULL;
		}

		str->size = res;
		str->chars = pts;
	}

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
		jep_char ca = a->chars[i];
		jep_char cb = b->chars[i];

		if (ca.b3 > cb.b3) return 1;
		else if (ca.b3 < cb.b3) return -1;
		else if (ca.b2 > cb.b2) return 1;
		else if (ca.b2 < cb.b2) return -1;
		else if (ca.b1 > cb.b1) return 1;
		else if (ca.b1 < cb.b1) return -1;
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

	size_t i;
	jep_char* dchars;
	jep_char* nchars;

	dchars = dest->chars;

	// make sure we have a destination buffer
	if (dchars == NULL)
	{
		dchars = (jep_char*)malloc(sizeof(jep_char) * src->size);

		if (dchars == NULL)
			return 0;

		dest->size = src->size;
	}

	// make sure that the destination buffer size is equal to the
	// source buffer size.
	if (src->size != dest->size)
	{
		nchars = (jep_char*)realloc(dest->chars, src->size);

		if (nchars != NULL)
			dchars = nchars;
		else
			return 0;

		dest->size = src->size;
	}

	for (i = 0; i < src->size; i++)
	{
		dchars[i] = src->chars[i];
	}

	dest->chars = dchars;

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
		if (raw[i].b3 == 0 && raw[i].b2 == 0)
			buffer[i] = str->chars[i].b1;
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
		if (raw[i].b3 == 0 && raw[i].b2 == 0)
			buffer[i] = str->chars[i].b1;
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
		if (raw[i].b3 == 0 && raw[i].b2 == 0)
			buffer[i] = str->chars[i].b1;
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
		if (raw[i].b3 == 0 && raw[i].b2 == 0)
			buffer[i] = str->chars[i].b1;
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
		if (raw[i].b3 == 0 && raw[i].b2 == 0)
			buffer[i] = str->chars[i].b1;
		else
			buffer[i] = '\0';
	}

	buffer[i] = '\0';

	d = strtod(buffer, &end);

	free(buffer);

	return d;
}
