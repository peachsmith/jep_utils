#ifndef JEP_CHARACTER_H
#define JEP_CHARACTER_H

#include "jep_utils.h"
#include "unicode.h"




/**
 * A character is an individual element of a string.
 * The numeric value of a character is equivalent to a Unicode code point.
 */
typedef jep_code_point jep_char;




/**
 * Determines if two characters (a and b) represent the same Unicode
 * code point. Given the characters a and b, the result of a comparison
 * of a and b should be -1, 0, or 1 if a is less than, equal to, or greater
 * than b respectively.
 *
 * Examples:
 *
 *   a < b:
 *     jep_char a = 0x42;            // a = 'B'
 *     jep_char b = 0x43;            // b = 'C'
 *     int res = jep_char_cmp(a, b); // res = -1
 *
 *   a == b:
 *     jep_char a = 0x42;            // a = 'B'
 *     jep_char b = 0x42;            // b = 'B'
 *     int res = jep_char_cmp(a, b); // res = 0
 *
 *   a > b:
 *     jep_char a = 0x42;            // a = 'B'
 *     jep_char b = 0x41;            // b = 'A'
 *     int res = jep_char_cmp(a, b); // res = 1
 *
 *
 * Params:
 *   jep_char - the first character
 *   jep_char - the second character
 *
 * Returns:
 *   int - 1 if a > b, 0 if a == b, or -1 if a < b
 */
JEP_UTILS_API int JEP_UTILS_CALL
jep_char_cmp(jep_char a, jep_char b);

#endif
