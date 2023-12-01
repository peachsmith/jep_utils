#ifndef JEP_BITSTRING_H
#define JEP_BITSTRING_H

#include "jep_core.h"

/**
 * Creates a new bitstring.
 *
 * Returns:
 *   jep_bitstring - a new bitstring
 */
JEP_UTILS_API jep_bitstring *JEP_UTILS_CALL
jep_create_bitstring();

/**
 * Frees the resources allocated for a bitstring.
 *
 * Params:
 *   jep_bitstring - a bitstring
 */
JEP_UTILS_API void JEP_UTILS_CALL
jep_destroy_bitstring(jep_bitstring *bs);

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
jep_add_bit(jep_bitstring *bs, unsigned int bit);

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
jep_add_bits(jep_bitstring *dest, jep_bitstring *src);

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
jep_get_bit(jep_bitstring *bs, int index);

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
jep_set_bit(jep_bitstring *bs, int index, unsigned int value);

/**
 * Removes the last bit from a bitstring.
 * Returns 1 on success or 0 on failure.
 *
 * Params:
 *   jep_bitstring - a bitstring
 */
JEP_UTILS_API int JEP_UTILS_CALL
jep_pop_bit(jep_bitstring *bs);

#endif
