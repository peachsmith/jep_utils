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
#define JEP_TEST_EXPECTED 29
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

/**
 * Performs the following 5 assertions:
 * 1. bs != null
 * 2. bs->bytes != null
 * 3. bs->byte_count == 1
 * 4. bs->bit_count = 0
 * 5. bs->current_bits = 0
 * 
 * Params:
 *   jep_bitstring - a bitstring
 * 
 * Returns:
 *   int - the number of successful assertions
 */
int jep_create_bitstring_tests(jep_bitstring* bs);

/**
 * Performs the following 2 assertion:
 * 1. bs->bit_count == 1
 * 2. bs->current_bits == 1
 * 
 * Params:
 *   jep_bitstring - a bitstring
 * 
 * Returns:
 *   int - the number of successful assertions
 */
int jep_add_bit_tests(jep_bitstring* bs);

/**
 * Performs the following 3 assertion:
 * 1. bs->bit_count == 9
 * 2. bs->current_bits == 1
 * 3. bs->byte_count == 2
 * 
 * Params:
 *   jep_bitstring - a bitstring
 * 
 * Returns:
 *   int - the number of successful assertions
 */
int jep_add_bits_tests(jep_bitstring* bs);

/**
 * Performs the following 2 assertions:
 * 1. bit_a == 0
 * 2. bit_b == 1
 * 
 * Params:
 *   int - an integer containing a bit value
 *   int - an integer containing a bit value
 * 
 * Returns:
 *   int - the number of successful assertions
 */
int jep_get_bit_tests(int bit_a, int bit_b);

/**
 * Performs the following assertion:
 * bit == 1
 * 
 * Params:
 *   int - an integer containing a bit value
 * 
 * Returns:
 *   int - the number of successful assertions
 */
int jep_set_bit_tests(int bit);

#endif
