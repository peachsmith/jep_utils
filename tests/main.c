#include "character_tests.h"
#include "unicode_tests.h"
#include "string_tests.h"
#include "byte_buffer_tests.h"
#include "char_buffer_tests.h"
#include "json_tests.h"
#include "huffman_tests.h"

#define MAX_PASSES 39

int main(int argc, char** argv)
{
	int passes = 0;

	// character (3 tests)
	passes += char_compare_less_test();
	passes += char_compare_greater_test();
	passes += char_compare_equal_test();

	// Unicode (7 tests)
	passes += utf8_decode_test();
	passes += utf8_encode_test();
	passes += utf16_be_bom_decode_test();
	passes += utf16_be_bom_encode_test();
	passes += utf16_le_bom_decode_test();
	passes += utf16_be_decode_test();
	passes += utf16_be_encode_test();
	passes += utf16_le_decode_test();
	passes += utf16_le_encode_test();

	// string (16 tests)
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
	passes += bytes_to_string_utf8_test();
	passes += bytes_to_string_utf16_be_bom_test();
	passes += bytes_to_string_utf16_le_bom_test();
	passes += bytes_to_string_utf16be_test();
	passes += bytes_to_string_utf16le_test();

	// byte buffer (3 tests)
	passes += byte_buffer_create_test();
	passes += byte_buffer_append_byte_test();
	passes += byte_buffer_append_bytes_test();

	// character buffer (3 tests)
	passes += char_buffer_create_test();
	passes += char_buffer_append_char_test();
	passes += char_buffer_append_chars_test();

	// JSON (2 tests)
	passes += json_parse_test();
	passes += json_field_test();

	// Huffman Coding (3 tests)
	passes += huff_encode_test();
	passes += huff_decode_test();
	passes += huff_read_test();

	printf("%d/%d tests passed\n", passes, MAX_PASSES);

	return 0;
}
