#ifndef JEP_HUFFMAN_H
#define JEP_HUFFMAN_H

#include "jep_core.h"

/**
 * Encodes a series of bytes as a bitstring using Huffman Coding.
 * Returns NULL on failure.
 *
 * Params:
 *   jep_byte_buffer - a collection of raw, unencoded bytes
 *
 * Returns:
 *   jep_byte_buffer - a collection of bytes of data encoded with
 *     Huffman Coding.
 */
JEP_UTILS_API jep_byte_buffer *JEP_UTILS_CALL
jep_huff_encode(jep_byte_buffer *raw);

/**
 * Decodes a bitstring containing data encoded with Huffman Coding.
 * Returns NULL on failure.
 *
 * Params:
 *   jep_byte_buffer - a collection of bytes of data encoded with
 *     Huffman Coding.
 *
 * Returns:
 *   jep_byte_buffer - a collection of raw, unencoded bytes
 */
JEP_UTILS_API jep_byte_buffer *JEP_UTILS_CALL
jep_huff_decode(jep_byte_buffer *encoded);

/**
 * Reads data encoded with Huffman Coding from a byte buffer.
 *
 * Params:
 *   jep_byte_buffer - a collection of encoded bytes
 *
 * Returns:
 *   huff_code - a Huffman Coding context or NULL on failure
 */
JEP_UTILS_API jep_huff_code *JEP_UTILS_CALL
jep_huff_read(jep_byte_buffer *raw);

/**
 * Writes data encoded with Huffman Coding to a byte buffer.
 * The data should be preceded by the bitcode dictionary.
 *
 * Params:
 *   huff_code - a Huffman Coding context
 *   jep_byte_buffer - a byte buffer
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
JEP_UTILS_API int JEP_UTILS_CALL
jep_huff_write(jep_huff_code *hc, jep_byte_buffer *buffer);

/**
 * Frees the resources allocated for a Huffman Coding context.
 *
 * Params:
 *   huff_code - a Huffman Coding context.
 */
JEP_UTILS_API void JEP_UTILS_CALL
jep_destroy_huff_code(jep_huff_code *hc);

#endif
