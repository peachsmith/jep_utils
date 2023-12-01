#ifndef JEP_CHAR_BUFFER_H
#define JEP_CHAR_BUFFER_H

#include "jep_core.h"

/**
 * A character buffer is a method of maintaining a dynamically allocated
 * collection of characters.
 *
 * A character buffer can have the following operations performed on itself:
 *   append - adds a new character or characters to the end of the buffer
 *   remove - removes a character from a certain point in the buffer
 *   clear  - empties the buffer of all contents
 *
 */
typedef struct jep_char_buffer
{
    size_t size;
    size_t cap;
    jep_char *buffer;
} jep_char_buffer;

/**
 * Allocates resources for a new char buffer.
 * Returns NULL if memory allocation fails.
 *
 * Returns:
 *   jep_char_buffer - a new char buffer
 */
JEP_UTILS_API jep_char_buffer *JEP_UTILS_CALL
jep_create_char_buffer();

/**
 * Frees resources allocated for a new char buffer.
 *
 * Params:
 *   jep_char_buffer - the char buffer to be destroyed
 */
JEP_UTILS_API void JEP_UTILS_CALL
jep_destroy_char_buffer(jep_char_buffer *bb);

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
jep_append_char(jep_char_buffer *cb, jep_char c);

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
jep_append_chars(jep_char_buffer *cb, jep_char *c, int n);

/**
 * Removes a character at the specified index
 *
 * Params:
 *   jep_char_buffer - a chracter buffer
 *   size_t - the index of the character to remove
 */
JEP_UTILS_API void JEP_UTILS_CALL
jep_remove_char_at(jep_char_buffer *cb, size_t index);

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
jep_clear_char_buffer(jep_char_buffer *cb);

#endif
