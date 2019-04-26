/**
 * Test cases for the jep_utils library.
 *
 * Author: John Powell
 */

#ifndef JEP_UTILS_TESTS_H
#define JEP_UTILS_TESTS_H

#include "jep_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define JEP_TEST_BUFFER_SIZE 50
#define JEP_TEST_EXPECTED 16
#define JEP_TEST_REPORT_SEPARATOR "-----------------------------------\n"
#define JEP_TEST_REPORT_RESULT(a, b) "%-28s [%-4s]\n", a, (b ? "PASS" : "FAIL")

/**
 * Perform the following 4 assertions:
 * 1. bb != NULL
 * 2. bb->buffer != NULL
 * 3. bb->size == 0
 * 4. bb->cap == 50
 *
 * Params:
 *   jep_byte_buffer - a byte buffer
 *
 * Returns:
 *   int - the number of successful assertions
 */
int jep_create_byte_buffer_tests(jep_byte_buffer* bb);

/**
 * Perform the following 2 assertions:
 * 1. bb->size == 1
 * 2. bb->cap == 50
 *
 * Params:
 *   jep_byte_buffer - a byte buffer
 *
 * Returns:
 *   int - the number of successful assertions
 */
int jep_append_byte_tests(jep_byte_buffer* bb);

/**
 * Perform the following 2 assertions:
 * 1. bb->size == 51
 * 2. bb->cap == 75
 *
 * Params:
 *   jep_byte_buffer - a byte buffer
 *
 * Returns:
 *   int - the number of successful assertions
 */
int jep_append_bytes_tests(jep_byte_buffer* bb);

/**
 * Perform the following 4 assertions:
 * 1. cb != NULL
 * 2. cb->buffer != NULL
 * 3. cb->size == 0
 * 4. cb->cap == 50
 *
 * Params:
 *   jep_char_buffer - a character buffer
 *
 * Returns:
 *   int - the number of successful assertions
 */
int jep_create_char_buffer_tests(jep_char_buffer* cb);

/**
 * Perform the following 2 assertions:
 * 1. cb->size == 1
 * 2. cb->cap == 50
 *
 * Params:
 *   jep_char_buffer - a character buffer
 *
 * Returns:
 *   int - the number of successful assertions
 */
int jep_append_char_tests(jep_char_buffer* cb);

/**
 * Perform the following 2 assertions:
 * 1. cb->size == 51
 * 2. cb->cap == 75
 *
 * Params:
 *   jep_char_buffer - a character buffer
 *
 * Returns:
 *   int - the number of successful assertions
 */
int jep_append_chars_tests(jep_char_buffer* cb);

#endif
