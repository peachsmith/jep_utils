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




#define JEP_ENCODING_UTF_8     1
#define JEP_ENCODING_UTF_16_BE 2
#define JEP_ENCODING_UTF_16_LE 3




 /**
  * A byte.
  * According to the C standard, a char has the size of a single byte.
  * Therefore, the size of a char primitive is always 1. The number of
  * bits in a byte is specified by the CHAR_BIT constant in limits.h.
  */
typedef unsigned char jep_byte;




/**
 * A Unicode code point.
 * As of writing, the highest Unicode code point is U+10FFFF,
 * which can fit into three bytes.
 *
 * The members b1, b2, and b3 are the first, second, and third bytes
 * respectively. So for the code point U+XXYYZZ, XX holds the value
 * b3, YY holds the value of b2, and ZZ holds the value of b1.
 * For example, given the code point U+000024, the b1 member would
 * have a value of 0x24, and b2 and b3 would have a value of 0x00.
 *
 * The characters in the basic multilingual plane (BMP) can be represented
 * using two bytes.
 */
typedef struct jep_code_point {
	jep_byte b1;
	jep_byte b2;
	jep_byte b3;
}jep_code_point;




/**
 * A character is an individual element of a string.
 * The value of a character is equivalent to a Unicode code point.
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
typedef struct jep_string {
	jep_char* chars;
	size_t size;
}jep_string;




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
typedef struct jep_byte_buffer {
	size_t size;
	size_t cap;
	jep_byte* buffer;
}jep_byte_buffer;




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
typedef struct jep_char_buffer {
	size_t size;
	size_t cap;
	jep_char* buffer;
}jep_char_buffer;




/**
 * A bit string is a sequence of bits.
 *
 * A bit string can have the following operations performed on itself:
 *   add - adds a bit or bits to the end of the sequence
 *   get - gets the value of a bit at a particular index
 *   set - sets the value of a bit at a particular index
 *   pop - removes the last bit of a sequence
 */
typedef struct jep_bitstring {
	unsigned long bit_count;
	unsigned long byte_count;
	jep_byte* bytes;
	jep_byte current_bits;
}jep_bitstring;




/**
 * A Huffman symbol represents a byte grouped with the Huffman metadata
 * associated with that byte.
 * This metadata can be things like frequency of occurrence, the depth
 * of the symbol in a Huffman tree, and the bit string code.
 */
typedef struct jep_huff_sym {
	jep_byte b;          /* byte       */
	unsigned long f;     /* frequency  */
	unsigned long w;     /* weight     */
	unsigned long n;     /* tree depth */
	jep_bitstring* code; /* bit code   */
}jep_huff_sym;

/**
 * A Huffman dictionary is a collection of all of the symbols
 * found in a source of bytes.
 */
typedef struct jep_huff_dict {
	jep_huff_sym* symbols;
	unsigned long count;
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
	unsigned long count;
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
 * A JSON token classifies a string of raw text within the context of JSON
 * syntax.
 */
typedef struct json_token {
	jep_string* str;
	int type;
	struct json_token* next;
	struct json_token* prev;
	struct json_token* head;
	struct json_token* tail;
}json_token;

/**
 * Represents the current state of operations.
 * While tokenizing a raw string or parsing a stream of tokens, this
 * can be used to keep track of what the current action is.
 * For example, while parsing a stream of tokens, if a '{' token is
 * encountered, the state should be modified to indicate that the beginning
 * of an object has been encountered.
 */
typedef struct json_state {
	int state;
	struct json_state* top;
	struct json_state* prev;
}json_state;

/**
 * A single piece of data.
 * A JSON value can be one of the following:
 *
 *   Type     Example
 *   -------------------------
 *   string   "example string"
 *   number   4
 *   boolean  true
 *   null     null
 *   object   { "exampleField" : "example value" }
 *   array    [ 1, 2, 3 ]
 */
typedef struct json_value {
	int type;
	union {
		struct json_object* obj;
		struct json_array* arr;
		jep_string* raw;
	}data;
	struct json_value* next;
}json_value;

/**
 * A field is a name-value pair that exists within an object.
 * The textual representation of a JSON field should be a string
 * of text, followed by a colon, followed by the value.
 *
 * Example:
 *   "someNumber" : 2.50
 */
typedef struct json_field {
	jep_string* name;
	json_value* value;
	struct json_field* next;
}json_field;

/**
 * A JSON array is a collection of JSON values without names.
 * Example:
 *   [ "toast", "bagel", "biscuit" ]
 */
typedef struct json_array {
	json_value* values;
	json_value* tail;
}json_array;

/**
 * A JSON object is a collection of fields.
 * Example:
 *   { "flavor" : "blueberry", "price" : 2.5 }
 */
typedef struct json_object {
	json_field* fields;
	json_field* tail;
}json_object;




/* ----------------------------------------------------------------------- */
/*                              character                                  */
/* ----------------------------------------------------------------------- */

/**
 * Determines if two characters (a and b) represent the same Unicode
 * code point. The result should be -1 if a < b, 0 if a == b, and 1 if a > b.
 *
 * Examples:
 *   Let
 *     jep_char a = { 0x00, 0x00, 0x42 }; // 'B'
 *     jep_char b = { 0x00, 0x00, 0x43 }; // 'C'
 *   Then
 *     jep_char_cmp(a, b) = 1
 *
 *   Let
 *     jep_char a = { 0x00, 0x00, 0x42 }; // 'B'
 *     jep_char b = { 0x00, 0x00, 0x42 }; // 'B'
 *   Then
 *     jep_char_cmp(a, b) = 0
 *
 *   Let
 *     jep_char a = { 0x00, 0x00, 0x42 }; // 'B'
 *     jep_char b = { 0x00, 0x00, 0x41 }; // 'A'
 *   Then
 *     jep_char_cmp(a, b) = -1
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




/* ----------------------------------------------------------------------- */
/*                             byte buffer                                 */
/* ----------------------------------------------------------------------- */

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
jep_append_byte(jep_byte_buffer* bb, jep_byte b);

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
jep_append_bytes(jep_byte_buffer* bb, jep_byte* b, int n);

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




/* ----------------------------------------------------------------------- */
/*                           character buffer                              */
/* ----------------------------------------------------------------------- */

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
jep_append_char(jep_char_buffer* cb, jep_char c);

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
jep_append_chars(jep_char_buffer* cb, jep_char* c, int n);

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




/* ----------------------------------------------------------------------- */
/*                             bit string                                  */
/* ----------------------------------------------------------------------- */

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

/**
 * Removes the last bit from a bitstring.
 * Returns 1 on success or 0 on failure.
 *
 * Params:
 *   jep_bitstring - a bitstring
 */
JEP_UTILS_API int JEP_UTILS_CALL
jep_pop_bit(jep_bitstring* bs);




/* ----------------------------------------------------------------------- */
/*                            Huffman Coding                               */
/* ----------------------------------------------------------------------- */

/**
 * Encodes a series of bytes as a bitstring using Huffman Coding.
 * Returns NULL on failure.
 *
 * Params:
 *   jep_byte_buffer - a collection of unencoded bytes
 *
 * Returns:
 *   huff_code - a Huffman Coding context containing encoded data
 */
JEP_UTILS_API jep_huff_code* JEP_UTILS_CALL
jep_huff_encode(jep_byte_buffer* raw);

/**
 * Decodes a bitstring containing data encoded with Huffman Coding.
 * Returns NULL on failure.
 *
 * Params:
 *   huff_code - a Huffman Coding context containing encoded data
 *
 * Returns:
 *   jep_byte_buffer - a collection of unencoded bytes
 */
JEP_UTILS_API jep_byte_buffer* JEP_UTILS_CALL
jep_huff_decode(jep_huff_code* hc);

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
jep_read_huff_code_from_buffer(jep_byte_buffer* raw);

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
jep_write_huff_code_to_buffer(jep_huff_code* hc, jep_byte_buffer* buffer);

/**
 * Frees the resources allocated for a Huffman Coding context.
 *
 * Params:
 *   huff_code - a Huffman Coding context.
 */
JEP_UTILS_API void JEP_UTILS_CALL
jep_destroy_huff_code(jep_huff_code* hc);




/* ----------------------------------------------------------------------- */
/*                               Unicode                                   */
/* ----------------------------------------------------------------------- */

/**
 * Decodes an array of UTF-8 encoded bytes to a string
 * of characters.
 *
 * Params:
 *   jep_byte* - an array of bytes
 *   size_t - the number of bytes in the array
 *
 * Returns:
 *   jep_char* - an array of characters or NULL on failure
 */
JEP_UTILS_API jep_code_point* JEP_UTILS_CALL
jep_utf8_decode(const jep_byte* bytes, size_t n, size_t* res);

/**
 * Decodes an array of UTF-16 encoded bytes to a string
 * of characters. If a byte order marker (BOM) is detected at the
 * beginning of the byte sequence, then the bytes of each surrogate
 * pair and BMP code point will be swapped.
 *
 * Params:
 *   jep_byte* - an array of bytes
 *   size_t - the number of bytes in the array
 *
 * Returns:
 *   jep_char* - an array of characters or NULL on failure
 */
JEP_UTILS_API jep_code_point* JEP_UTILS_CALL
jep_utf16_decode(jep_byte* bytes, size_t n, size_t* res);

/**
 * Decodes an array of UTF-16BE encoded bytes to a string
 * of characters. If a BOM is encountered, it is treated as
 * just another encoded character.
 *
 * Params:
 *   jep_byte* - an array of bytes
 *   size_t - the number of bytes in the array
 *
 * Returns:
 *   jep_char* - an array of characters or NULL on failure
 */
JEP_UTILS_API jep_code_point* JEP_UTILS_CALL
jep_utf16be_decode(jep_byte* bytes, size_t n, size_t* res);

/**
 * Decodes an array of UTF-16LE encoded bytes to a string
 * of characters. If a BOM is encountered, it is treated as
 * just another encoded character.
 *
 * Params:
 *   jep_byte* - an array of bytes
 *   size_t - the number of bytes in the array
 *
 * Returns:
 *   jep_char* - an array of characters or NULL on failure
 */
JEP_UTILS_API jep_code_point* JEP_UTILS_CALL
jep_utf16le_decode(jep_byte* bytes, size_t n, size_t* res);




/* ----------------------------------------------------------------------- */
/*                               Strings                                   */
/* ----------------------------------------------------------------------- */

/**
 * Creates a new string.
 *
 * Params:
 *   size_t - the number of characters in the string.
 *
 * Returns:
 *   jep_string - a new string
 */
JEP_UTILS_API jep_string* JEP_UTILS_CALL
jep_create_string(size_t size);

/**
 * Frees the memory allocated for a string.
 *
 * Params:
 *   jep_string - a string
 */
JEP_UTILS_API void JEP_UTILS_CALL
jep_destroy_string(jep_string* str);

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
JEP_UTILS_API jep_string* JEP_UTILS_CALL
jep_bytes_to_string(const jep_byte* bytes, int encoding, size_t n);

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
JEP_UTILS_API jep_string* JEP_UTILS_CALL
jep_chars_to_string(jep_char* chars, size_t n);

/**
 * Compares two strings.
 *
 * Params:
 *   jep_string - a string
 */
JEP_UTILS_API int JEP_UTILS_CALL
jep_strcmp(jep_string* a, jep_string* b);

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
jep_strcpy(jep_string* src, jep_string* dest);

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
JEP_UTILS_API jep_string* JEP_UTILS_CALL
jep_new_string(const char* lit);

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
jep_string_to_long(jep_string* str, int radix);

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
jep_string_to_ulong(jep_string* str, int radix);

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
jep_string_to_int(jep_string* str, int radix);

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
jep_string_to_uint(jep_string* str, int radix);

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
jep_string_to_double(jep_string* str);




/* ----------------------------------------------------------------------- */
/*                                 JSON                                    */
/* ----------------------------------------------------------------------- */

/**
 * Parses a string of text into a JSON object.
 *
 * Params:
 *   jep_string - a string containing raw JSON text
 *   json_object - a reference to a pointer to a JSON object
 *
 * Returns:
 *   int - 0 on success, otherwise an error code.
 */
JEP_UTILS_API int JEP_UTILS_CALL
jep_parse_json_string(jep_string* raw, json_object** obj);

/**
 * Frees the memory allocated for a JSON object.
 *
 * Params:
 *   json_object - a JSON object
 */
JEP_UTILS_API void JEP_UTILS_CALL
jep_destroy_json_object(json_object* obj);

/**
 * Retrieves field from within a JSON object.
 *
 * Params:
 *   json_object - a JSON object
 *   const char - the name of the JSON field
 *
 * Returns:
 *   json_field
 */
JEP_UTILS_API json_field* JEP_UTILS_CALL
jep_get_json_field(json_object* obj, jep_string* name);

#endif
