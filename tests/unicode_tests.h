#ifndef JEP_UNICODE_TESTS_H
#define JEP_UNICODE_TESTS_H

#include "jep_utils/unicode.h"

#include "unicode_tests.h"

int utf8_decode_test();

int utf8_encode_test();

int utf16_be_bom_decode_test();

int utf16_be_bom_encode_test();

int utf16_le_bom_decode_test();

int utf16_be_decode_test();

int utf16_be_encode_test();

int utf16_le_decode_test();

int utf16_le_encode_test();

#endif
