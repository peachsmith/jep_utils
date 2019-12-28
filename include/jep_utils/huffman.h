#ifndef JEP_HUFFMAN_H
#define JEP_HUFFMAN_H

#include "jep_utils.h"
#include "bitstring.h"
#include "byte_buffer.h"




/**
 * A Huffman symbol represents a byte grouped with the Huffman metadata
 * associated with that byte.
 * This metadata can be things like frequency of occurrence, the depth
 * of the symbol in a Huffman tree, and the bit string code.
 */
typedef struct jep_huff_sym {
    jep_byte b;          /* byte       */
    uint32_t f;          /* frequency  */
    uint32_t w;          /* weight     */
    uint32_t n;          /* tree depth */
    jep_bitstring* code; /* bit code   */
}jep_huff_sym;

/**
 * A Huffman dictionary is a collection of all of the symbols
 * found in a source of bytes.
 */
typedef struct jep_huff_dict {
    jep_huff_sym* symbols;
    uint32_t count;
}jep_huff_dict;

/**
 * A Huffman node is a single element in a Huffman tree.
 */
typedef struct jep_huff_node {
    jep_huff_sym sym;
    struct jep_huff_node* leaf_1;
    struct jep_huff_node* leaf_2;
    struct jep_huff_node* next;
    struct jep_huff_node* prev;
}jep_huff_node;

/**
 * A Huffman tree is an arrangment of Huffman symbols based on their
 * frequency of occurrence in the source.
 * The symbols are wrapped in nodes which are linked together into a
 * list referred to as a Huffman tree.
 * The tree can be traversed to determine the bit string code for a
 * symbol in a terminal node.
 */
typedef struct jep_huff_tree {
    jep_huff_node* nodes;
    jep_huff_node* tail;
}jep_huff_tree;

/**
 * A Huffman code is a series of bits containing data encoded using
 * Huffman coding.
 */
typedef struct jep_huff_code {
    jep_huff_tree* tree;
    jep_huff_dict* dict;
    jep_bitstring* data;
}jep_huff_code;




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
JEP_UTILS_API jep_byte_buffer* JEP_UTILS_CALL
jep_huff_encode(jep_byte_buffer* raw);

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
JEP_UTILS_API jep_byte_buffer* JEP_UTILS_CALL
jep_huff_decode(jep_byte_buffer* encoded);

/**
 * Reads data encoded with Huffman Coding from a byte buffer.
 *
 * Params:
 *   jep_byte_buffer - a collection of encoded bytes
 *
 * Returns:
 *   huff_code - a Huffman Coding context or NULL on failure
 */
JEP_UTILS_API jep_huff_code* JEP_UTILS_CALL
jep_huff_read(jep_byte_buffer* raw);

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
jep_huff_write(jep_huff_code* hc, jep_byte_buffer* buffer);

/**
 * Frees the resources allocated for a Huffman Coding context.
 *
 * Params:
 *   huff_code - a Huffman Coding context.
 */
JEP_UTILS_API void JEP_UTILS_CALL
jep_destroy_huff_code(jep_huff_code* hc);

#endif
