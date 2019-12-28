#ifndef JEP_STRING_TESTS_H
#define JEP_STRING_TESTS_H

#include "jep_utils/string.h"

// 16 tests

int string_create_test();

int empty_string_test();

int bytes_to_string_test();

int bytes_to_string_utf8_test();

int bytes_to_string_utf16_be_bom_test();

int bytes_to_string_utf16_le_bom_test();

int bytes_to_string_utf16be_test();

int bytes_to_string_utf16le_test();

int new_string_test();

int string_compare_less_test();

int string_compare_greater_test();

int string_compare_equal_test();

int string_to_int_test();

int string_to_uint_test();

int string_to_long_test();

int string_to_ulong_test();

#endif