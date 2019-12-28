/**
 * jep_utils
 *
 * Author: John Powell
 */

#ifndef JEP_UTILS_H
#define JEP_UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>




/* shared object constants */
#ifdef _WIN32
#ifdef JEP_UTILS_EXPORTS
#define JEP_UTILS_API __declspec(dllexport)
#else
#define JEP_UTILS_API __declspec(dllimport)
#endif
#define JEP_UTILS_CALL __cdecl

#elif defined(__linux__)
#define JEP_UTILS_API
#define JEP_UTILS_CALL

#elif defined(__APPLE__)
#define JEP_UTILS_API
#define JEP_UTILS_CALL

#endif




/* character encodings */
#define JEP_ENCODING_UTF_8     1
#define JEP_ENCODING_UTF_16    2
#define JEP_ENCODING_UTF_16_BE 3
#define JEP_ENCODING_UTF_16_LE 4

/* byte orders */
#define JEP_NO_ENDIAN     0
#define JEP_BIG_ENDIAN    1
#define JEP_LITTLE_ENDIAN 2




/**
 * A byte is a single unit of information made of bits.
 * In theory, any given computer architecture could use any number of bits
 * for a single byte.
 * There is no official standard for the width of a byte, but
 * ISO/IEC 2382-1:1993 mentions that bytes typically use 8 bits.
 *
 * --------------------------------------------------------------------------
 * According to ISO/IEC 2382-1:1993, a byte is "A string that consists of a
 * number of bits, treated as a unit, and usually representing a character or
 * a part of a character."
 *
 * Notes about bytes from ISO/IEC 2382-1:1993:
 *   1. The number of bits in a byte is fixed for a given data processing
 *      system.
 *   2. The number of bits in a byte is usually 8.
 * --------------------------------------------------------------------------
 *
 * Due to the ubiquity of the 8-bit byte, a byte is assumed to be exactly
 * 8 bits. As such, the type uint8_t from the C99 header stdint.h can be
 * used interchangeably with the jep_byte type.
 * Furthermore, since the C primitive type char uses 8 bits commonly,
 * the type unsigned char can be used interchangeably with uint8_t.
 * A word is therefore considered to be 2 bytes, or 16 bits.
 * A double word (or dword) is considered to be 4 bytes, or 32 bits.
 *
 * For a platform to be supported, it must define CHAR_BIT to be 8,
 * and must use 8 bits as the width of a byte.
 */
typedef uint8_t jep_byte;




#if defined(CHAR_BIT) && CHAR_BIT == 8

/**
 * Bit extraction macros.
 * Each macro expectes as its argument an 8-bit integer.
 * The macro jep_lo_x will return the lower x bits.
 * The macro jep_hi_x will return the higher x bits.
 * The bits returned by these macros are right justified.
 *
 * Examples:
 *   Given the binary number b with a value of 10110010,
 *   jep_lo_4(b) = 00000010
 *   jep_hi_4(b) = 00001011
 *
 */
#define jep_lo_7(b) (b & 0x7F)
#define jep_lo_6(b) (b & 0x3F)
#define jep_lo_5(b) (b & 0x1F)
#define jep_lo_4(b) (b & 0x0F)
#define jep_lo_3(b) (b & 0x07)
#define jep_lo_2(b) (b & 0x03)
#define jep_lo_1(b) (b & 0x01)

#define jep_hi_7(b) ((b & 0xFE) >> 1)
#define jep_hi_6(b) ((b & 0xFC) >> 2)
#define jep_hi_5(b) ((b & 0xF8) >> 3)
#define jep_hi_4(b) ((b & 0xF0) >> 4)
#define jep_hi_3(b) ((b & 0xE0) >> 5)
#define jep_hi_2(b) ((b & 0xC0) >> 6)
#define jep_hi_1(b) ((b & 0x80) >> 7)

/**
 * Word extraction macros.
 * Each macro expectes a double word as its argument.
 * The bits returned by these macros are right justified.
 *
 * Examples:
 *   Given the hexadecimal number n with a value of 0xAB12CD34,
 *   jep_lo_word(n) = 0x0000CD34
 *   jep_hi_word(n) = 0x0000AB12
 */
#define jep_lo_word(n) (n & 0xFFFF)
#define jep_hi_word(n) ((n & 0xFFFF0000) >> 16)

/**
 * Byte extraction macros.
 * Each macro expectes a word as its argument.
 * The bits returned by these macros are right justified.
 *
 * Examples:
 *   Given the hexadecimal number n with a value of 0xCD34,
 *   jep_lo_byte(n) = 0x34
 *   jep_hi_byte(n) = 0xAB
 */
#define jep_lo_byte(n) (n & 0xFF)
#define jep_hi_byte(n) ((n & 0xFF00) >> 8)

/**
 * Splits an unsigned 32-bit integer into four unsigned 8-bit integers.
 *
 * Params:
 *   n - an unsigned, 32-bit integer
 *   b - an array of four unsigned 8-bit integers
 */
#define jep_split_u32(n, b) \
{\
	b[0] = jep_lo_byte(jep_lo_word(n)); \
	b[1] = jep_hi_byte(jep_lo_word(n)); \
	b[2] = jep_lo_byte(jep_hi_word(n)); \
	b[3] = jep_hi_byte(jep_hi_word(n)); \
}

/**
 * Builds an unsigned 32-bit integer from an array of unsigned 8-bit integers.
 *
 * Params:
 *   n - an unsigned, 32-bit integer
 *   b - an array of four unsigned 8-bit integers
 */
#define jep_build_u32(n, b) \
    n = (b[3] << 24) | (b[2] << 16) | (b[1] << 8) | b[0];

#else
#error CHAR_BIT is not 8
#endif

#define jep_alloc(t, n) (t*)malloc(sizeof(t) * n)
#define jep_realloc(a, t, n) (t*)realloc(a, sizeof(t) * n)

#endif
