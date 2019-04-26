/**
 * jep_utils
 *
 * Author: John Powell
 */

#ifndef JEP_UTILS_H
#define JEP_UTILS_H

#include <stdlib.h>
#include <limits.h>

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

typedef struct jep_byte_buffer {

	size_t size;
	size_t cap;
	unsigned char* buffer;

}jep_byte_buffer;

typedef struct jep_char_buffer {

	size_t size;
	size_t cap;
	char* buffer;

}jep_char_buffer;

typedef struct jep_bitstring {
	unsigned long bit_count;
	unsigned long byte_count;
	unsigned char* bytes;
	unsigned char current_bits;
}jep_bitstring;

/* ------------------------------------------------- */
/*                   byte buffer                     */
/* ------------------------------------------------- */

/**
 * Allocates resources for a new byte buffer.
 * Returns NULL if memory allocation fails.
 *
 * Returns:
 *   jep_byte_buffer - a new byte buffer
 */
JEP_UTILS_API jep_byte_buffer* JEP_UTILS_CALL
jep_create_byte_buffer();

/**
 * Frees resources allocated for a new byte buffer.
 *
 * Params:
 *   jep_byte_buffer - the byte buffer to be destroyed
 */
JEP_UTILS_API void JEP_UTILS_CALL
jep_destroy_byte_buffer(jep_byte_buffer* bb);

/**
 * Appends a byte to a byte buffer.
 * Returns 1 on success or 0 on failure.
 *
 * Params:
 *   jep_byte_buffer - the byte buffer to receive the new byte
 *   unsigned char - the byte to be appended
 *
 * Returns:
 *   int - an integer indicating success or failure
 */
JEP_UTILS_API int JEP_UTILS_CALL
jep_append_byte(jep_byte_buffer* bb, unsigned char b);

/**
 * Appends a byte to a byte buffer.
 * Returns the number of bytes added.
 *
 * Params:
 *   jep_byte_buffer - the byte buffer to receive the new byte
 *   unsigned char* - a pointer to an array of bytes
 *   int - the number of bytes to append
 *
 * Returns:
 *   int - an integer indicating the number of bytes added
 */
JEP_UTILS_API int JEP_UTILS_CALL
jep_append_bytes(jep_byte_buffer* bb, unsigned char* b, int n);

/**
 * Removes a byte at the specified index
 *
 * Params:
 *   jep_byte_buffer - a byte buffer
 *   size_t - the index of the character to remove
 */
JEP_UTILS_API void JEP_UTILS_CALL
jep_remove_byte_at(jep_byte_buffer* bb, size_t index);

/**
 * Removes all bytes from a byte buffer and reallocates
 * a new, empty buffer.
 *
 * Params:
 *   jep_byte_buffer - a byte buffer to be emptied
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
JEP_UTILS_API int JEP_UTILS_CALL
jep_clear_byte_buffer(jep_byte_buffer* bb);

/* ------------------------------------------------- */
/*                character buffer                   */
/* ------------------------------------------------- */

 /**
  * Allocates resources for a new char buffer.
  * Returns NULL if memory allocation fails.
  *
  * Returns:
  *   jep_char_buffer - a new char buffer
  */
JEP_UTILS_API jep_char_buffer* JEP_UTILS_CALL
jep_create_char_buffer();

/**
 * Frees resources allocated for a new char buffer.
 *
 * Params:
 *   jep_char_buffer - the char buffer to be destroyed
 */
JEP_UTILS_API void JEP_UTILS_CALL
jep_destroy_char_buffer(jep_char_buffer* bb);

/**
 * Appends a char to a char buffer.
 * Returns 1 on success or 0 on failure.
 *
 * Params:
 *   jep_char_buffer - the char buffer to receive the new char
 *   char - the char to be appended
 *
 * Returns:
 *   int - an integer indicating success or failure
 */
JEP_UTILS_API int JEP_UTILS_CALL
jep_append_char(jep_char_buffer* cb, char c);

/**
 * Appends a char to a char buffer.
 * Returns the number of chars added.
 *
 * Params:
 *   jep_char_buffer - the char buffer to receive the new char
 *   char* - a pointer to an array of chars
 *   int - the number of chars to append
 *
 * Returns:
 *   int - an integer indicating the number of chars added
 */
JEP_UTILS_API int JEP_UTILS_CALL
jep_append_chars(jep_char_buffer* cb, char* c, int n);

/**
 * Removes a character at the specified index
 *
 * Params:
 *   jep_char_buffer - a chracter buffer
 *   size_t - the index of the character to remove
 */
JEP_UTILS_API void JEP_UTILS_CALL
jep_remove_char_at(jep_char_buffer* cb, size_t index);

/**
 * Removes all bytes from a byte buffer and reallocates
 * a new, empty buffer.
 *
 * Params:
 *   jep_byte_buffer - a byte buffer to be emptied
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
JEP_UTILS_API int JEP_UTILS_CALL
jep_clear_char_buffer(jep_char_buffer* cb);

/* ------------------------------------------------- */
/*                    bitstring                      */
/* ------------------------------------------------- */

/**
 * Creates a new bitstring.
 *
 * Returns:
 *   jep_bitstring - a new bitstring
 */
JEP_UTILS_API jep_bitstring* JEP_UTILS_CALL
jep_create_bitstring();

/**
 * Frees the resources allocated for a bitstring.
 *
 * Params:
 *   jep_bitstring - a bitstring
 */
JEP_UTILS_API void JEP_UTILS_CALL
jep_destroy_bitstring(jep_bitstring* bs);

/**
 * Adds a bit to a bitstring.
 *
 * Params:
 *   jep_bitstring - a bitstring to receive the new bit
 *   unsigned int - the new bit to insert (either 0 or 1)
 * 
 * Returns:
 *   int - 1 on success or 0 on failure
 */
JEP_UTILS_API int JEP_UTILS_CALL
jep_add_bit(jep_bitstring* bs, unsigned int bit);

/**
 * Adds the contents of one bitstring to another.
 *
 * Params:
 *   jep_bitstring - the destination bitstring
 *   jep_bitstring - the source bitstring
 *
 * Returns:
 *   int - the number of bits successfully added
 */
JEP_UTILS_API int JEP_UTILS_CALL
jep_add_bits(jep_bitstring* dest, jep_bitstring* src);

/**
 * Retrieves the bit value stored at the specifide index
 * in a bitstring.
 *
 * Params:
 *   jep_bitstring - a bitstring
 *   int - the index of the bit to access starting at 0
 *
 * Returns:
 *   int - the value of the bit at the specified index or
 *         -1 on failure
 */
JEP_UTILS_API int JEP_UTILS_CALL
jep_get_bit(jep_bitstring* bs, int index);

/**
 * Sets the bit value stored at the specifide index
 * in a bitstring.
 *
 * Params:
 *   jep_bitstring - a bitstring
 *   int - the index of the bit to access starting at 0
 *   unsigned int - the new bit value
 */
JEP_UTILS_API void JEP_UTILS_CALL
jep_set_bit(jep_bitstring* bs, int index, unsigned int value);

#endif
