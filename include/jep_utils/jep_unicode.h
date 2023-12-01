#ifndef JEP_UNICODE_H
#define JEP_UNICODE_H

#include "jep_core.h"

/**
 * Decodes an array of UTF-8 encoded bytes to a string of characters.
 * This function takes as its third argument a reference to a number that will
 * be populated with the number of code points successfully decoded.
 *
 * Params:
 *   uint8_t* - an array of bytes
 *   size_t - the number of bytes in the array
 *   res - a reference to a number
 *
 * Returns:
 *   jep_char* - an array of characters or NULL on failure
 */
JEP_UTILS_API jep_code_point *JEP_UTILS_CALL
jep_utf8_decode(const jep_byte *bytes, size_t n, size_t *res);

/**
 * Encodes a string of text with the UTF-8 character encoding.
 * The third argument is a reference to a number that will be
 * populated with the amount of bytes in the output array.
 *
 * Params:
 *   jep_code_point* - an array of Unicode code points
 *   size_t - the size of the code point array.
 *   size_t* - a reference to a number
 *
 * Returns:
 *   jep_byte* - an array of bytes encoded with UTF-8.
 */
JEP_UTILS_API jep_byte *JEP_UTILS_CALL
jep_utf8_encode(const jep_code_point *str, size_t n, size_t *res);

/**
 * Encodes a string of text with the UTF-16 character encoding.
 * The third argument is a reference to a number that will be
 * populated with the amount of bytes in the output array.
 *
 * By default, big endian byte order is assumed and the BOM
 * will be omitted.
 *
 * Params:
 *   jep_code_point* - an array of Unicode code points
 *   size_t - the size of the code point array.
 *   size_t* - a reference to a number
 *   int - byte order. 0, 1, or 2 for default, big, or little endianness.
 *   int - 1 for BOM, 0 for no BOM
 *
 * Returns:
 *   jep_byte* - an array of bytes encoded with UTF-16.
 */
JEP_UTILS_API jep_byte *JEP_UTILS_CALL
jep_utf16_encode(
    const jep_code_point *str,
    size_t n,
    size_t *res,
    int e,
    int bom);

/**
 * Encodes a string of text with the UTF-16BE character encoding.
 * The third argument is a reference to a number that will be
 * populated with the amount of bytes in the output array.
 *
 * By default, big endian byte order is assumed and the BOM
 * will be omitted.
 *
 * Params:
 *   jep_code_point* - an array of Unicode code points
 *   size_t - the size of the code point array.
 *   size_t* - a reference to a number
 *
 * Returns:
 *   jep_byte* - an array of bytes encoded with UTF-16.
 */
JEP_UTILS_API jep_byte *JEP_UTILS_CALL
jep_utf16be_encode(const jep_code_point *str, size_t n, size_t *res);

/**
 * Encodes a string of text with the UTF-16LE character encoding.
 * The third argument is a reference to a number that will be
 * populated with the amount of bytes in the output array.
 *
 * By default, big endian byte order is assumed and the BOM
 * will be omitted.
 *
 * Params:
 *   jep_code_point* - an array of Unicode code points
 *   size_t - the size of the code point array.
 *   size_t* - a reference to a number
 *
 * Returns:
 *   jep_byte* - an array of bytes encoded with UTF-16.
 */
JEP_UTILS_API jep_byte *JEP_UTILS_CALL
jep_utf16le_encode(const jep_code_point *str, size_t n, size_t *res);

/**
 * Decodes an array of UTF-16 encoded bytes to a string of characters.
 * If a byte order marker (BOM) is detected at the beginning of the byte
 * sequence, then the bytes of each surrogate pair and BMP code point will
 * be swapped.
 *
 * Params:
 *   uint8_t* - an array of bytes
 *   size_t - the number of bytes in the array
 *
 * Returns:
 *   jep_char* - an array of characters or NULL on failure
 */
JEP_UTILS_API jep_code_point *JEP_UTILS_CALL
jep_utf16_decode(const jep_byte *bytes, size_t n, size_t *res);

/**
 * Decodes an array of UTF-16BE encoded bytes to a string of characters.
 * If a BOM is encountered, it is treated as just another encoded character.
 *
 * Params:
 *   uint8_t* - an array of bytes
 *   size_t - the number of bytes in the array
 *
 * Returns:
 *   jep_char* - an array of characters or NULL on failure
 */
JEP_UTILS_API jep_code_point *JEP_UTILS_CALL
jep_utf16be_decode(const jep_byte *bytes, size_t n, size_t *res);

/**
 * Decodes an array of UTF-16LE encoded bytes to a string of characters.
 * If a BOM is encountered, it is treated as just another encoded character.
 *
 * Params:
 *   uint8_t* - an array of bytes
 *   size_t - the number of bytes in the array
 *
 * Returns:
 *   jep_char* - an array of characters or NULL on failure
 */
JEP_UTILS_API jep_code_point *JEP_UTILS_CALL
jep_utf16le_decode(const jep_byte *bytes, size_t n, size_t *res);

#endif
