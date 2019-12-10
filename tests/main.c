#include "jep_utils.h"




int char_compare_less_test()
{
	jep_char a = { 0x00, 0x00, 0x00 };
	jep_char b = { 0x01, 0x00, 0x00 };

	return jep_char_cmp(a, b) == -1 ? 1 : 0;
}

int char_compare_greater_test()
{
	jep_char a = { 0x01, 0x00, 0x00 };
	jep_char b = { 0x00, 0x00, 0x00 };

	return jep_char_cmp(a, b) == 1 ? 1 : 0;
}

int char_compare_equal_test()
{
	jep_char a = { 0x00, 0x00, 0x00 };
	jep_char b = { 0x00, 0x00, 0x00 };

	return jep_char_cmp(a, b) == 0 ? 1 : 0;
}




int byte_buffer_create_test()
{
	jep_byte_buffer* bb;

	bb = jep_create_byte_buffer();

	if (bb == NULL)
		return 0;

	jep_destroy_byte_buffer(bb);

	return 1;
}

int byte_buffer_append_byte_test()
{
	int res;
	jep_byte_buffer* bb;
	jep_byte b;

	bb = jep_create_byte_buffer();

	if (bb == NULL)
		return 0;

	b = (jep_byte)4U;

	res = jep_append_byte(bb, b);

	jep_destroy_byte_buffer(bb);

	return res;
}

int byte_buffer_append_bytes_test()
{
	int res;
	jep_byte_buffer* bb;
	jep_byte b[3] = { 4, 5, 6 };

	bb = jep_create_byte_buffer();

	if (bb == NULL)
		return 0;

	res = jep_append_bytes(bb, b, 3);

	jep_destroy_byte_buffer(bb);

	return res == 3 ? 1 : 0;
}




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
	jep_char c = { 0x65, 0x00, 0x00 };

	cb = jep_create_char_buffer();

	if (cb == NULL)
		return 0;

	res = jep_append_char(cb, c);

	if (cb->buffer[0].b1 != 0x65)
		res = 0;

	jep_destroy_char_buffer(cb);

	return res;
}

int char_buffer_append_chars_test()
{
	int res;
	jep_char_buffer* cb;
	jep_char c[3] = {
		{ 0x65, 0x00, 0x00 },
		{ 0x66, 0x00, 0x00 },
		{ 0x67, 0x00, 0x00 }
	};

	cb = jep_create_char_buffer();

	if (cb == NULL)
		return 0;

	res = jep_append_chars(cb, c, 3);

	jep_destroy_char_buffer(cb);

	return res == 3 ? 1 : 0;
}



int string_create_test()
{
	jep_string* s;
	int res;

	s = jep_create_string(5);

	if (s == NULL)
		return 0;

	res = s->size == 5 ? 1 : 0;

	jep_destroy_string(s);

	return res;
}

int empty_string_test()
{
	jep_string* s;
	int res;

	s = jep_create_string(0);

	if (s == NULL)
		return 0;

	res = s->size == 0 ? 1 : 0;

	jep_destroy_string(s);

	return res;
}

int bytes_to_string_test()
{
	const char* c_str = "bagel";
	jep_string* str;
	int res = 1;
	int enc = JEP_ENCODING_UTF_8;

	str = jep_bytes_to_string(c_str, enc, strlen(c_str));

	if (str == NULL)
		return 0;

	if (str->size != 5)
		res = 0;

	if (str->chars[0].b1 != (jep_byte)'b')
		res = 0;

	if (str->chars[4].b1 != (jep_byte)'l')
		res = 0;

	jep_destroy_string(str);

	return res;
}

int new_string_test()
{
	jep_string* str;

	str = jep_new_string("hello");

	if (str == NULL)
		return 0;
	else
		jep_destroy_string(str);

	return 1;
}

int string_compare_less_test()
{
	jep_string* a;
	jep_string* b;
	int res;
	int enc = JEP_ENCODING_UTF_8;

	const char* c_str_a = "abc";
	const char* c_str_b = "abd";

	a = jep_bytes_to_string(c_str_a, enc, strlen(c_str_a));

	if (a == NULL)
		return 0;

	b = jep_bytes_to_string(c_str_b, enc, strlen(c_str_b));

	if (b == NULL)
	{
		jep_destroy_string(a);
		return 0;
	}

	res = jep_strcmp(a, b) == -1 ? 1 : 0;

	jep_destroy_string(a);
	jep_destroy_string(b);

	return res;
}

int string_compare_greater_test()
{
	jep_string* a;
	jep_string* b;
	int res;
	int enc = JEP_ENCODING_UTF_8;

	const char* c_str_a = "abd";
	const char* c_str_b = "abc";

	a = jep_bytes_to_string(c_str_a, enc, strlen(c_str_a));

	if (a == NULL)
		return 0;

	b = jep_bytes_to_string(c_str_b, enc, strlen(c_str_b));

	if (b == NULL)
	{
		jep_destroy_string(a);
		return 0;
	}

	res = jep_strcmp(a, b) == 1 ? 1 : 0;

	jep_destroy_string(a);
	jep_destroy_string(b);

	return res;
}

int string_compare_equal_test()
{
	jep_string* a;
	jep_string* b;
	int res;
	int enc = JEP_ENCODING_UTF_8;

	const char* c_str_a = "abc";
	const char* c_str_b = "abc";

	a = jep_bytes_to_string(c_str_a, enc, strlen(c_str_a));

	if (a == NULL)
		return 0;

	b = jep_bytes_to_string(c_str_b, enc, strlen(c_str_b));

	if (b == NULL)
	{
		jep_destroy_string(a);
		return 0;
	}

	res = jep_strcmp(a, b) == 0 ? 1 : 0;

	jep_destroy_string(a);
	jep_destroy_string(b);

	return res;
}

int string_to_int_test()
{
	jep_string* str;
	const char* bytes = "1234";
	int n;
	int enc = JEP_ENCODING_UTF_8;
	int res;

	str = jep_bytes_to_string(bytes, enc, strlen(bytes));

	if (str == NULL)
		return 0;

	n = jep_string_to_int(str, 10);

	jep_destroy_string(str);

	res = n == 1234 ? 1 : 0;

	return res;
}

int string_to_uint_test()
{
	jep_string* str;
	const char* bytes = "1234";
	unsigned int n;
	int enc = JEP_ENCODING_UTF_8;
	int res;

	str = jep_bytes_to_string(bytes, enc, strlen(bytes));

	if (str == NULL)
		return 0;

	n = jep_string_to_uint(str, 10);

	jep_destroy_string(str);

	res = n == 1234 ? 1 : 0;

	return res;
}

int string_to_long_test()
{
	jep_string* str;
	const char* bytes = "1234";
	long n;
	int enc = JEP_ENCODING_UTF_8;
	int res;

	str = jep_bytes_to_string(bytes, enc, strlen(bytes));

	if (str == NULL)
		return 0;

	n = jep_string_to_long(str, 10);

	jep_destroy_string(str);

	res = n == 1234 ? 1 : 0;

	return res;
}

int string_to_ulong_test()
{
	jep_string* str;
	const char* bytes = "1234";
	unsigned long n;
	int enc = JEP_ENCODING_UTF_8;
	int res;

	str = jep_bytes_to_string(bytes, enc, strlen(bytes));

	if (str == NULL)
		return 0;

	n = jep_string_to_ulong(str, 10);

	jep_destroy_string(str);

	res = n == 1234 ? 1 : 0;

	return res;
}


int json_parse_test()
{
	int parsed;
	jep_string* jstr;
	json_object* jobj;

	jstr = jep_new_string("{ \"data\" : \"bagel\" }");
	jobj = NULL;

	if (jstr == NULL)
		return 0;

	parsed = jep_parse_json_string(jstr, &jobj);

	if (jobj == NULL)
	{
		jep_destroy_string(jstr);
		return 0;
	}

	jep_destroy_string(jstr);
	jep_destroy_json_object(jobj);

	return 1;
}

int json_field_test()
{
	int res;
	int parsed;
	jep_string* jstr;
	jep_string* fstr;
	json_object* jobj;
	json_field* jfield;
	jep_string* data;

	res = 1;
	jstr = jep_new_string("{ \"data\" : \"bagel\" }");
	fstr = jep_new_string("data");
	jobj = NULL;

	if (jstr == NULL && fstr != NULL)
	{
		jep_destroy_string(fstr);
		return 0;
	}
	else if (jstr != NULL && fstr == NULL)
	{
		jep_destroy_string(jstr);
		return 0;
	}

	parsed = jep_parse_json_string(jstr, &jobj);

	if (jobj == NULL)
	{
		jep_destroy_string(jstr);
		jep_destroy_string(fstr);
		return 0;
	}

	jfield = jep_get_json_field(jobj, fstr);

	if (jfield == NULL)
		res = 0;
	else if (jfield->value == NULL)
		res = 0;
	else if (jfield->value->data.raw == NULL)
		res = 0;
	else
	{
		data = jfield->value->data.raw;

		if (data->chars[0].b1 != (jep_byte)'b'
			|| data->chars[1].b1 != (jep_byte)'a'
			|| data->chars[2].b1 != (jep_byte)'g'
			|| data->chars[3].b1 != (jep_byte)'e'
			|| data->chars[4].b1 != (jep_byte)'l')
			res = 0;
	}

	jep_destroy_string(jstr);
	jep_destroy_string(fstr);
	jep_destroy_json_object(jobj);

	return res;
}




#define MAX_PASSES 22

int main(int argc, char** argv)
{
	int passes = 0;

	// character
	passes += char_compare_less_test();
	passes += char_compare_greater_test();
	passes += char_compare_equal_test();

	// byte buffer
	passes += byte_buffer_create_test();
	passes += byte_buffer_append_byte_test();
	passes += byte_buffer_append_bytes_test();

	// character buffer
	passes += char_buffer_create_test();
	passes += char_buffer_append_char_test();
	passes += char_buffer_append_chars_test();
	
	// string
	passes += string_create_test();
	passes += empty_string_test();
	passes += bytes_to_string_test();
	passes += new_string_test();
	passes += string_compare_less_test();
	passes += string_compare_greater_test();
	passes += string_compare_equal_test();
	passes += string_to_int_test();
	passes += string_to_uint_test();
	passes += string_to_long_test();
	passes += string_to_ulong_test();

	// JSON
	passes += json_parse_test();
	passes += json_field_test();

	printf("%d/%d tests passed\n", passes, MAX_PASSES);

	return 0;
}
