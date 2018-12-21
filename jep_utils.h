/**
 * The main header file for jep_utils
 */

#include <stdlib.h>

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

/**********************
* byte buffer         *
**********************/

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


/**********************
 * character buffer   *
 **********************/

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
jep_append_char(jep_char_buffer* bb, char b);

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
jep_append_chars(jep_char_buffer* bb, char* b, int n);
