#include "jep_utils.h"




/* byte orders */
#define NO_ENDIAN 0
#define BIG_ENDIAN 1
#define LITTLE_ENDIAN 2

/* UTF-8 prefix codes */
#define UTF8_1     0x00
#define UTF8_2     0x06
#define UTF8_3     0x0E
#define UTF8_4     0x1E

/**
 * Determines how many bytes are in a Unicode sequence by examining the
 * prefix code. The byte length is determined using the following mapping:
 *
 * +--------------------------+
 * | Prefix Code | Byte Count |
 * +--------------------------+
 * | 0           |  1         |
 * | 110         |  2         |
 * | 1110        |  3         |
 * | 11110       |  4         |
 * +--------------------------+
 *
 * If the prefix code is not one of the four acceptable values,
 * then 1 is returned.
 *
 * Params:
 *   jep_byte - a byte representing the start of a Unicode sequence
 *
 * Returns:
 *   size_t - the number of bytes in the sequence.
 */
#define jep_utf8_len(b)                \
      ((b >> 7) == UTF8_1) ? (size_t)1 \
    : ((b >> 5) == UTF8_2) ? (size_t)2 \
    : ((b >> 4) == UTF8_3) ? (size_t)3 \
    : ((b >> 3) == UTF8_4) ? (size_t)4 \
    : (size_t)1


/**
 * Determines how many bytes are in a UTF-16 encoded sequence by examining the
 * first byte value. The sequence length is determined using the following
 * mapping:
 *
 * +----------------------------------+
 * | Range of First Byte | Byte Count |
 * +----------------------------------+
 * | [0x00, 0xD7]        | 2          |
 * | [0xE0, 0xFF]        | 2          |
 * | [0xD8, 0xDF]        | 4          |
 * +----------------------------------+
 *
 * Params:
 *   jep_byte - a byte representing the first byte in a UTF-16 encoded
 *              byte sequence
 *
 * Returns:
 *   size_t - the number of bytes in the sequence.
 */
#define jep_utf16_len(b) (b <= 0xD7 || b >= 0xE0) ? (size_t)2 : (size_t)4


/**
 * The jep_low_x macros retrieve the lowest x bits from a byte.
 * So if b is the byte 11111111, then jep_low_4(b) should return 00001111,
 * jep_low_3(b) should return 00000111, etc.
 */
#if defined(CHAR_BIT) && CHAR_BIT == 8
#define jep_low_6(b) (b & (0xFF >> 2))
#define jep_low_5(b) (b & (0xFF >> 3))
#define jep_low_4(b) (b & (0xFF >> 4))
#define jep_low_3(b) (b & (0xFF >> 5))
#else
#error CHAR_BIT is not 8
#endif




 /**
  * Decodes a UTF-8 byte sequence into a code point.
  * If the number of bytes is less than 1 or greater than 4,
  * then the code point U+000000 is returned.
  *
  * Params:
  *   jep_byte* - a reference to a sequence of bytes
  *   size_t - the number of bytes in the sequence
  *
  * Returns:
  *   code_point - a Unicode code point
  */
static jep_code_point utf8_decode_point(jep_byte* seq, size_t n);

/**
 * Decodes a UTF-16 byte sequence into a code point.
 * If the number of bytes is anything other than 2 or 4,
 * then the code point U+000000 is returned.
 *
 * Params:
 *   jep_byte* - a reference to a sequence of bytes
 *   size_t - the number of bytes in the sequence
 *
 * Returns:
 *   code_point - a Unicode code point
 */
static jep_code_point utf16_decode_point(jep_byte* seq, size_t n);



JEP_UTILS_API int JEP_UTILS_CALL
jep_char_cmp(jep_char a, jep_char b)
{
	// Given the code points U+UUVVWW and U+XXYYZZ,
	// compare UU and XX, then VV and YY, then WW and ZZ.
	return a.b3 > b.b3 ? 1
		: a.b3 < b.b3 ? -1
		: a.b2 > b.b2 ? 1
		: a.b2 < b.b2 ? -1
		: a.b1 > b.b1 ? 1
		: a.b1 < b.b1 ? -1
		: 0;
}




JEP_UTILS_API jep_code_point* JEP_UTILS_CALL
jep_utf8_decode(const jep_byte* bytes, size_t n, size_t* res)
{
	size_t i;             // index
	jep_byte seq[4];      // UTF-8 encoded byte sequence
	size_t len;           // length of byte sequence
	size_t end;           // ending index of byte sequence
	jep_byte b;           // the current byte
	size_t d;             // difference of end - i
	jep_code_point point; // Unicode code point
	jep_code_point* pts;  // array of Unicode code points
	jep_code_point* npts; // code point reallocation buffer
	size_t c;             // number of decoded code points
	size_t cap;           // string capacity

	// Attempt to allocate an initial array of code points.
	// If the allocation fails, return NULL.
	cap = 10;
	pts = (jep_code_point*)malloc(sizeof(jep_code_point) * cap);

	if (pts == NULL)
	{
		*res = 0;
		return NULL;
	}

	// Group the bytes of the input array into byte sequences,
	// decode them into Unicode code points, then append those
	// code points to the output array.
	for (i = 0, c = 0; i < n; i++)
	{
		len = jep_utf8_len(bytes[i]);
		end = i + len;

		// Collect up to four bytes from the input array.
		// On each iteration, d represents the number of bytes
		// that have yet to be added to the sequence.
		do
		{
			b = bytes[i++];
			d = end - i;

			if (d >= 0 && d <= 4)
				seq[d] = b;

		} while (i < end && i < n && d > 0);

		// If we've finished building the byte sequence,
		// decode it and add it to the list of code points.
		if (d == 0)
		{
			// Decode a code point from a byte sequence
			point = utf8_decode_point(seq, len);

			// Reallocate code point buffer if needed
			if (c == cap - 1)
			{
				cap = cap + (cap / 2);
				npts = (jep_code_point*)realloc(pts, sizeof(jep_code_point) * cap);

				if (npts != NULL)
					pts = npts;
				else
				{
					free(pts);
					*res = 0;
					return NULL;
				}
			}

			// Append the new code point to the output array
			pts[c++] = point;
		}
		// else handle encoding error

		// Since i is incremented while building the byte sequence,
		// we decrement it here so that we don't skip over an index.
		i--;
	}

	*res = c;

	return pts;
}

jep_code_point* utf16_decode(jep_byte* bytes, size_t n, size_t* res, int order)
{
	size_t i;             // index
	jep_byte seq[4];      // UTF-16BE encoded byte sequence
	size_t len;           // length of byte sequence
	size_t end;           // ending index of byte sequence
	jep_byte b;           // the current byte
	size_t d;             // difference of end - i
	jep_code_point point; // Unicode code point
	jep_code_point* pts;  // array of Unicode code points
	jep_code_point* npts; // code point reallocation buffer
	size_t c;             // number of decoded code points
	size_t cap;           // string capacity
	jep_byte tmp;         // used during byte swapping
	int bom;              // Byte Order Marker (BOM)

	// Attempt to allocate an initial array of code points.
	// If the allocation fails, return NULL.
	cap = 10;
	pts = (jep_code_point*)malloc(sizeof(jep_code_point) * cap);

	if (pts == NULL)
	{
		*res = 0;
		return NULL;
	}

	// Check for the BOM to determine if we need to do byte swapping.
	// If the BOM is 0xFEFF, or if the BOM is not present, then we
	// don't do byte swapping. If the BOM is 0xFFEF, then we do byte
	// swapping.
	bom = 0;
	if (!order && n >= 2)
	{
		bom = (bytes[0] == 0xFE && bytes[1] == 0xFF) ? 0
			: (bytes[0] == 0xFF && bytes[1] == 0xFE) ? 1
			: 0;
	}

	// Group the bytes of the input array into byte sequences,
	// decode them into Unicode code points, then append those
	// code points to the output array.
	for (i = 0, c = 0; i < n; i++)
	{
		if ((order == LITTLE_ENDIAN || bom) && i < n - 1)
			len = jep_utf16_len(bytes[i + 1]);
		else
			len = jep_utf16_len(bytes[i]);

		end = i + len;

		// Collect up to four bytes from the input array.
		// On each iteration, d represents the number of bytes
		// that have yet to be added to the sequence.
		do
		{
			b = bytes[i++];
			d = end - i;

			if (d >= 0 && d <= 4)
				seq[d] = b;

		} while (i < end && i < n && d > 0);

		// Perform byte swapping if necessary.
		if (order == LITTLE_ENDIAN || bom)
		{
			/* Swap the first two bytes */
			tmp = seq[0];
			seq[0] = seq[1];
			seq[1] = tmp;

			// Swap the second two bytes
			tmp = seq[2];
			seq[2] = seq[3];
			seq[3] = tmp;
		}

		// If we've finished building the byte sequence,
		// decode it and add it to the list of code points.
		if (d == 0)
		{
			// Decode a code point from a byte sequence
			point = utf16_decode_point(seq, len);

			// Reallocate code point buffer if needed
			if (c == cap - 1)
			{
				cap = cap + (cap / 2);
				npts = (jep_code_point*)realloc(pts, cap);

				if (npts != NULL)
					pts = npts;
				else
				{
					free(pts);
					*res = 0;
					return NULL;
				}
			}

			//Append the new code point to the output array
			pts[c++] = point;
		}
		// else handle encoding error

		// Since i is incremented while building the byte sequence,
		// we decrement it here so that we don't skip over an index.
		i--;
	}

	*res = c;

	return pts;
}

jep_code_point* jep_utf16_decode(jep_byte* bytes, size_t n, size_t* res)
{
	return utf16_decode(bytes, n, res, NO_ENDIAN);
}

jep_code_point* jep_utf16be_decode(jep_byte* bytes, size_t n, size_t* res)
{
	return utf16_decode(bytes, n, res, BIG_ENDIAN);
}

jep_code_point* jep_utf16le_decode(jep_byte* bytes, size_t n, size_t* res)
{
	return utf16_decode(bytes, n, res, LITTLE_ENDIAN);
}




static jep_code_point utf8_decode_point(jep_byte* seq, size_t n)
{
	jep_byte b1 = (jep_byte)0U; // U+....XX
	jep_byte b2 = (jep_byte)0U; // U+..XX..
	jep_byte b3 = (jep_byte)0U; // U+XX....
	jep_code_point p;           // Unicode code point

	if (n == 1)
	{
		// For sequences 1 byte long, the byte value
		// is equal to the code point value.
		b1 = seq[0];
	}
	else if (n == 2)
	{
		// For sequences 2 bytes long:
		// encoded sequqnce: 110xxxxx 10yyyyyy
		// decoded code point: 00000xxx xxyyyyyy
		b2 |= (jep_low_5(seq[1]) >> 2);
		b1 |= ((jep_low_5(seq[1]) & 0x03) << 6);
		b1 |= jep_low_6(seq[0]);
	}
	else if (n == 3)
	{
		// For sequences 3 bytes long:
		// encoded sequqnce: 1110xxxx 10yyyyyy 10zzzzzz
		// decoded code point: xxxxyyyy yyzzzzzz
		b2 |= (jep_low_4(seq[2]) << 4);
		b2 |= (jep_low_6(seq[1]) >> 2);
		b1 |= ((jep_low_6(seq[1]) & 0x03) << 6);
		b1 |= jep_low_6(seq[0]);
	}
	else if (n == 4)
	{
		// For sequences 4 bytes long:
		// encoded sequqnce: 11110xxx 10yyyyyy 10zzzzzz 10vvvvvv
		// decoded code point: 000xxxyy yyyyzzzz zzvvvvvv
		b3 |= (jep_low_3(seq[3]) << 2);
		b3 |= (jep_low_6(seq[2]) >> 4);
		b2 |= ((jep_low_6(seq[2]) & 0x0F) << 4);
		b2 |= (jep_low_6(seq[1]) >> 2);
		b1 |= ((jep_low_6(seq[1]) & 0x03) << 6);
		b1 |= jep_low_6(seq[0]);
	}

	p.b1 = b1;
	p.b2 = b2;
	p.b3 = b3;

	return p;
}

static jep_code_point utf16_decode_point(jep_byte* seq, size_t n)
{
	jep_byte b1 = (jep_byte)0U; // U+....XX
	jep_byte b2 = (jep_byte)0U; // U+..XX..
	jep_byte b3 = (jep_byte)0U; // U+XX....
	unsigned long hi;           // high surrogate
	unsigned long lo;           // low surrogate
	unsigned long c;            // numeric code point
	jep_code_point p;           // Unicode code point

	// Byte swapping should have already been performed if
	// it was deemed necessary.
	// So the order of the bytes in the sequence passed to
	// this function should always be the same.
	//
	// +-------------------------------------+
	// | Index | Data                        |
	// --------------------------------------+
	// | 0     | low half of low surrogate   |
	// | 1     | high half of low surrogate  |
	// | 2     | low hald of high surrogate  |
	// | 3     | high half of high surrogate |
	// +-------------------------------------+

	if (n == 2)
	{
		b1 = seq[0];
		b2 = seq[1];
	}
	else
	{
		// We assume that the byte order has already
		// been accounted for so that the fourth element
		// in the sequence is the high half of the high surrogate
		// and the third element is the lower half.
		hi = seq[3] << 8;
		hi |= seq[2];

		// Likewise, the high half of the low surrogate is
		// the second element of the sequence, and the low
		// half is the first element.
		lo = seq[1] << 8;
		lo |= seq[0];

		// Subtract 0xD800 from the high surrogate and
		// multiply the difference by 0x400
		hi -= 0xD800;
		hi *= 0x400;

		// Subtract 0xDC00 from the low surrogate
		lo -= 0xDC00;

		// Add the high and low surrogates with 0x10000
		c = hi + lo + 0x10000;

		b1 = (jep_byte)(c & 0x0000FF);
		b2 = (jep_byte)((c & 0x00FF00) >> 8);
		b3 = (jep_byte)((c & 0xFF0000) >> 16);
	}

	p.b1 = b1;
	p.b2 = b2;
	p.b3 = b3;

	return p;
}
