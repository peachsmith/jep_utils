#ifndef JEP_CORE_H
#define JEP_CORE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>

/* shared object constants */
#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#ifdef JEP_UTILS_EXPORTS
#define JEP_UTILS_API __declspec(dllexport)
#else
#define JEP_UTILS_API __declspec(dllimport)
#endif
#define JEP_UTILS_CALL __cdecl
#else
#define JEP_UTILS_API
#define JEP_UTILS_CALL
#endif // _WIN32

#ifdef __cplusplus
extern "C"
{
#endif

/* character encodings */
#define JEP_ENCODING_UTF_8 1
#define JEP_ENCODING_UTF_16 2
#define JEP_ENCODING_UTF_16_BE 3
#define JEP_ENCODING_UTF_16_LE 4

/* byte orders */
#define JEP_NO_ENDIAN 0
#define JEP_BIG_ENDIAN 1
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
#define jep_split_u32(n, b)                 \
    {                                       \
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

#define jep_alloc(t, n) (t *)malloc(sizeof(t) * n)
#define jep_realloc(a, t, n) (t *)realloc(a, sizeof(t) * n)

/**
 * A bit string is a sequence of bits.
 *
 * A bit string can have the following operations performed on itself:
 *   add - adds a bit or bits to the end of the sequence
 *   get - gets the value of a bit at a particular index
 *   set - sets the value of a bit at a particular index
 *   pop - removes the last bit of a sequence
 */
typedef struct jep_bitstring
{
    uint32_t bit_count;
    uint32_t byte_count;
    jep_byte *bytes;
    jep_byte current_bits;
} jep_bitstring;

/**
 * A byte buffer is a method of maintaining a dynamically allocated
 * collection of bytes.
 *
 * A byte buffer can have the following operations performed on itself:
 *   append - adds a new byte or bytes to the end of the buffer
 *   remove - removes a byte from a certain point in the buffer
 *   clear  - empties the buffer of all contents
 *
 */
typedef struct jep_byte_buffer
{
    size_t size;
    size_t cap;
    jep_byte *buffer;
} jep_byte_buffer;

/**
 * The value 0xC00000 is an out-of-range code point used to indicate the
 * absence of a character.
 */
#define JEP_CHAR_NONE 0xC00000

/**
 * A Unicode code point.
 * As of writing, the highest Unicode code point is U+10FFFF, which can fit
 * into three bytes. The characters in the basic multilingual plane (BMP) can
 * be represented using two bytes.
 * Therefore, an unsigned 32-bit integer is used to represent all Unicode
 * code points.
 *
 */
typedef uint32_t jep_code_point;

/**
 * A character is an individual element of a string.
 * The numeric value of a character is equivalent to a Unicode code point.
 */
typedef jep_code_point jep_char;

/**
 * A string is a sequence of characters.
 * Since the characters in a string are essentially Unicode code points,
 * an array of raw bytes must be decoded using a supported character
 * encoding before it can be converted to a string.
 *
 * If decoded from a sequence of raw bytes, the resulting string does not
 * contain any indication of which character encoding was used by those bytes.
 */
typedef struct jep_string
{
    jep_char *chars;
    size_t size;
} jep_string;

/**
 * A JSON token classifies a string of raw text within the context of JSON
 * syntax.
 */
typedef struct jep_json_token
{
    jep_string *str;
    int type;
    struct jep_json_token *next;
    struct jep_json_token *prev;
    struct jep_json_token *head;
    struct jep_json_token *tail;
} jep_json_token;

/**
 * Represents the current state of operations.
 * While tokenizing a raw string or parsing a stream of tokens, this
 * can be used to keep track of what the current action is.
 * For example, while parsing a stream of tokens, if a '{' token is
 * encountered, the state should be modified to indicate that the beginning
 * of an object has been encountered.
 */
typedef struct jep_json_state
{
    int state;
    struct jep_json_state *top;
    struct jep_json_state *prev;
} jep_json_state;

/**
 * A single piece of data.
 * A JSON value can be one of the following:
 *
 *  +-------------------------------------------------+
 *  | Type    | Example                               |
 *  +-------------------------------------------------+
 *  | string  | "example string"                      |
 *  | number  | 4                                     |
 *  | boolean | true                                  |
 *  | null    | null                                  |
 *  | object  | { "exampleName" : "example value" }   |
 *  | array   | [ 1, 2, 3 ]                           |
 *  +-------------------------------------------------+
 */
typedef struct jep_json_value
{
    int type;
    union
    {
        struct jep_json_object *obj;
        struct jep_json_array *arr;
        jep_string *raw;
    } data;
    struct jep_json_value *next;
} jep_json_value;

/**
 * A field is a name-value pair that exists within an object.
 * The textual representation of a JSON field should be a string of text,
 * followed by a colon then a value.
 * The value can be a number, a boolean, a string, an array, an object,
 * or null.
 *
 * Example:
 *   "someNumber" : 2.50
 */
typedef struct jep_json_field
{
    jep_string *name;
    jep_json_value *value;
    struct jep_json_field *next;
} jep_json_field;

/**
 * A JSON array is a collection of JSON values without names.
 * Example:
 *   [ "toast", "bagel", "biscuit" ]
 */
typedef struct jep_json_array
{
    jep_json_value *values;
    jep_json_value *tail;
} jep_json_array;

/**
 * A JSON object is a collection of fields.
 * Example:
 *   { "flavor" : "blueberry", "price" : 2.5 }
 */
typedef struct jep_json_object
{
    jep_json_field *fields;
    jep_json_field *tail;
} jep_json_object;

/**
 * A Huffman symbol represents a byte grouped with the Huffman metadata
 * associated with that byte.
 * This metadata can be things like frequency of occurrence, the depth
 * of the symbol in a Huffman tree, and the bit string code.
 */
typedef struct jep_huff_sym
{
    jep_byte b;          /* byte       */
    uint32_t f;          /* frequency  */
    uint32_t w;          /* weight     */
    uint32_t n;          /* tree depth */
    jep_bitstring *code; /* bit code   */
} jep_huff_sym;

/**
 * A Huffman dictionary is a collection of all of the symbols
 * found in a source of bytes.
 */
typedef struct jep_huff_dict
{
    jep_huff_sym *symbols;
    uint32_t count;
} jep_huff_dict;

/**
 * A Huffman node is a single element in a Huffman tree.
 */
typedef struct jep_huff_node
{
    jep_huff_sym sym;
    struct jep_huff_node *leaf_1;
    struct jep_huff_node *leaf_2;
    struct jep_huff_node *next;
    struct jep_huff_node *prev;
} jep_huff_node;

/**
 * A Huffman tree is an arrangment of Huffman symbols based on their
 * frequency of occurrence in the source.
 * The symbols are wrapped in nodes which are linked together into a
 * list referred to as a Huffman tree.
 * The tree can be traversed to determine the bit string code for a
 * symbol in a terminal node.
 */
typedef struct jep_huff_tree
{
    jep_huff_node *nodes;
    jep_huff_node *tail;
} jep_huff_tree;

/**
 * A Huffman code is a series of bits containing data encoded using
 * Huffman coding.
 */
typedef struct jep_huff_code
{
    jep_huff_tree *tree;
    jep_huff_dict *dict;
    jep_bitstring *data;
} jep_huff_code;

#ifdef __cplusplus
}
#endif

#endif