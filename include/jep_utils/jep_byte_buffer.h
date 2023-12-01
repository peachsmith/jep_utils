#ifndef JEP_BYTE_BUFFER_H
#define JEP_BYTE_BUFFER_H

#include "jep_core.h"

/**
 * Allocates resources for a new byte buffer.
 * Returns NULL if memory allocation fails.
 *
 * Returns:
 *   jep_byte_buffer - a new byte buffer
 */
JEP_UTILS_API jep_byte_buffer *JEP_UTILS_CALL
jep_create_byte_buffer();

/**
 * Frees resources allocated for a new byte buffer.
 *
 * Params:
 *   jep_byte_buffer - the byte buffer to be destroyed
 */
JEP_UTILS_API void JEP_UTILS_CALL
jep_destroy_byte_buffer(jep_byte_buffer *bb);

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
jep_append_byte(jep_byte_buffer *bb, jep_byte b);

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
jep_append_bytes(jep_byte_buffer *bb, jep_byte *b, int n);

/**
 * Removes a byte at the specified index
 *
 * Params:
 *   jep_byte_buffer - a byte buffer
 *   size_t - the index of the character to remove
 */
JEP_UTILS_API void JEP_UTILS_CALL
jep_remove_byte_at(jep_byte_buffer *bb, size_t index);

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
jep_clear_byte_buffer(jep_byte_buffer *bb);

#endif
