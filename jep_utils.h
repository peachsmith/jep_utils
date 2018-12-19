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
