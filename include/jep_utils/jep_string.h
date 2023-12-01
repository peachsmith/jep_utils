#ifndef JEP_STRING_H
#define JEP_STRING_H

#include "jep_core.h"

/**
 * Creates a new string.
 *
 * Params:
 *   size_t - the number of characters in the string.
 *
 * Returns:
 *   jep_string - a new string
 */
JEP_UTILS_API jep_string *JEP_UTILS_CALL
jep_create_string(size_t size);

/**
 * Frees the memory allocated for a string.
 *
 * Params:
 *   jep_string - a string
 */
JEP_UTILS_API void JEP_UTILS_CALL
jep_destroy_string(jep_string *str);

/**
 * Decodes an array of bytes into a string.
 *
 * Params:
 *   jep_byte* - an array of bytes
 *   int - the encoding of the bytes
 *   size_t - the number of bytes to decode
 *
 * Returns:
 *   jep_string - a string or NULL on failure
 */
JEP_UTILS_API jep_string *JEP_UTILS_CALL
jep_bytes_to_string(const jep_byte *bytes, int encoding, size_t n);

/**
 * Converts an array of characters into a string.
 *
 * Params:
 *   jep_char* - an array of characters
 *   size_t - the number of characters in the array
 *
 * Returns:
 *   jep_string - a string or NULL on failure
 */
JEP_UTILS_API jep_string *JEP_UTILS_CALL
jep_chars_to_string(jep_char *chars, size_t n);

/**
 * Compares two strings.
 *
 * Params:
 *   jep_string - a string
 */
JEP_UTILS_API int JEP_UTILS_CALL
jep_strcmp(jep_string *a, jep_string *b);

/**
 * Copies the contents of one string into another.
 *
 * Params:
 *   jep_string - the source string
 *   jep_string - the destination string
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
JEP_UTILS_API int JEP_UTILS_CALL
jep_strcpy(jep_string *src, jep_string *dest);

/**
 * Creates a string from a constant C string literal.
 * The string literal passed in must end with the NUL terminating character.
 * It is assumed that the string literal is encoded using UTF-8.
 * If the encoding of the string literal is anything other than UTF-8,
 * the behavior is undefined.
 *
 * Examples:
 *   const char* c_str = "Hello, World!";
 *   jep_string* j_str_1 = jep_new_string(c_str);
 *   jep_string* j_str_2 = jep_new_string("Good morning.");
 *
 * Params:
 *   const char* - a C string literal
 *
 * Returns:
 *   jep_string - a new string
 */
JEP_UTILS_API jep_string *JEP_UTILS_CALL
jep_new_string(const char *lit);

/**
 * Retrieves the value of a character at the specified index in a string.
 * If the index is out of bounds, then an out-of-range sentinel code point
 * is returned.
 *
 * Params:
 *   jep_string* - a string
 *   size_t - an index
 *
 * Returns:
 *   jep_char - the character at index i or a code point greater than U+10FFFF
 */
JEP_UTILS_API jep_char JEP_UTILS_CALL
jep_char_at(jep_string *str, size_t i);

/**
 * Converts a string into a primitive long.
 *
 * Params:
 *   jep_string - a string
 *   int - the radix (or base) of the number system
 *
 * Returns:
 *   long - a long representation of the source string
 */
JEP_UTILS_API long JEP_UTILS_CALL
jep_string_to_long(jep_string *str, int radix);

/**
 * Converts a string into a primitive unsigned long.
 *
 * Params:
 *   jep_string - a string
 *   int - the radix (or base) of the number system
 *
 * Returns:
 *   unsigned long - an unsignedlong representation of the source string
 */
JEP_UTILS_API unsigned long JEP_UTILS_CALL
jep_string_to_ulong(jep_string *str, int radix);

/**
 * Converts a string into a primitive int.
 *
 * Params:
 *   jep_string - a string
 *   int - the radix (or base) of the number system
 *
 * Returns:
 *   int - an int representation of the source string
 */
JEP_UTILS_API int JEP_UTILS_CALL
jep_string_to_int(jep_string *str, int radix);

/**
 * Converts a string into a primitive unsigned int.
 *
 * Params:
 *   jep_string - a string
 *   int - the radix (or base) of the number system
 *
 * Returns:
 *   int - an unsigned int representation of the source string
 */
JEP_UTILS_API unsigned int JEP_UTILS_CALL
jep_string_to_uint(jep_string *str, int radix);

/**
 * Converts a string into a primitive double.
 *
 * Params:
 *   jep_string - a string
 *
 * Returns:
 *   int - a double representation of the source string
 */
JEP_UTILS_API double JEP_UTILS_CALL
jep_string_to_double(jep_string *str);

#endif
