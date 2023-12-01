#include "jep_utils.h"

/* UTF-8 prefix codes */
#define UTF8_1 0x00
#define UTF8_2 0x06
#define UTF8_3 0x0E
#define UTF8_4 0x1E

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
 *   uint8_t - a byte representing the start of a Unicode sequence
 *
 * Returns:
 *   int - the number of bytes in the sequence.
 */
#define jep_utf8_len(b)        \
    ((b >> 7) == UTF8_1)   ? 1 \
    : ((b >> 5) == UTF8_2) ? 2 \
    : ((b >> 4) == UTF8_3) ? 3 \
    : ((b >> 3) == UTF8_4) ? 4 \
                           : 1

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
#define jep_utf16_len(b) (b <= 0xD7 || b >= 0xE0) ? 2 : 4

/*-----------------------------------------------------------------*/
/*               Code Point Encoding and Decoding                  */
/*-----------------------------------------------------------------*/

/**
 * Decodes a UTF-8 byte sequence into a code point.
 * If the number of bytes is less than 1 or greater than 4,
 * then the code point U+000000 is returned.
 *
 * Params:
 *   uint8_t* - a reference to a sequence of bytes
 *   size_t - the number of bytes in the sequence
 *
 * Returns:
 *   code_point - a Unicode code point
 */
static jep_code_point utf8_decode_point(uint8_t *seq, size_t n);

/**
 * Encodes a Unicode code point into a sequence of bytes using UTF-8.
 *
 * Params:
 *   uint32_t - a Unicode code point
 *   uint8_t* - an array of four 8-bit integers
 *
 * Returns:
 *   size_t - the number of bytes in the encoded sequence
 */
static size_t utf8_encode_point(jep_code_point p, uint8_t *seq);

/**
 * Decodes a UTF-16 byte sequence into a code point.
 * If the number of bytes is anything other than 2 or 4,
 * then the code point U+000000 is returned.
 *
 * Params:
 *   uint8_t* - a reference to a sequence of bytes
 *   size_t - the number of bytes in the sequence
 *
 * Returns:
 *   code_point - a Unicode code point
 */
static jep_code_point utf16_decode_point(uint8_t *seq, size_t n);

/**
 * Encodes a Unicode code point into a sequence of bytes using UTF-16.
 *
 * Params:
 *   uint32_t - a Unicode code point
 *   uint8_t* - an array of four 8-bit integers
 *
 * Returns:
 *   size_t - the number of bytes in the encoded sequence
 */
static size_t utf16_encode_point(jep_code_point p, uint8_t *seq);

/*-----------------------------------------------------------------*/
/*                   Public API Implementation                     */
/*-----------------------------------------------------------------*/

jep_code_point *jep_utf8_decode(const jep_byte *bytes, size_t n, size_t *res)
{
    size_t i;             // index
    uint8_t seq[4];       // UTF-8 encoded byte sequence
    size_t len;           // length of byte sequence
    size_t end;           // ending index of byte sequence
    uint8_t b;            // the current byte
    size_t d;             // difference of end - i
    jep_code_point point; // Unicode code point
    jep_code_point *pts;  // array of Unicode code points
    jep_code_point *npts; // code point reallocation buffer
    size_t c;             // number of decoded code points
    size_t cap;           // string capacity

    // Attempt to allocate an initial array of code points.
    // If the allocation fails, return NULL.
    cap = 10;
    pts = (jep_code_point *)malloc(sizeof(jep_code_point) * cap);

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
                npts = (jep_code_point *)realloc(pts, sizeof(jep_code_point) * cap);

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

jep_byte *jep_utf8_encode(const jep_code_point *str, size_t n, size_t *res)
{
    jep_byte *bytes; // Array of encoded bytes
    jep_byte *tmp;   // Temporary pointer used in resizing
    size_t i, j;     // Indices
    uint8_t seq[4];  // Array to hold a single byte sequence
    size_t cap;      // Capacity of output array
    size_t new_cap;  // Temporary capacity for resizing
    size_t count;    // Total number of encoded bytes
    size_t len;      // Length of a single byte sequence

    len = 0;
    count = 0;
    cap = 50;
    bytes = (jep_byte *)malloc(sizeof(jep_byte) * cap);

    if (bytes == NULL)
        return NULL;

    // Loop through the code points and place their encoded byte
    // sequences in the output array.
    for (i = 0; i < n; i++)
    {
        // Encode a single code point as a byte sequence.
        len = utf8_encode_point(str[i], seq);

        // Put the encoded bytes in the output array.
        for (j = 0; j < len; j++)
        {
            // Resize the output array if necessary.
            if (count >= cap)
            {
                new_cap = cap + cap / 2;
                tmp = (jep_byte *)realloc(bytes, new_cap);

                if (tmp == NULL)
                    return NULL;

                bytes = tmp;
            }

            bytes[count++] = seq[j];
        }
    }

    // Resize the output array if excess memory was allocated.
    if (count < cap)
    {
        tmp = (jep_byte *)realloc(bytes, count);

        if (tmp == NULL)
            return NULL;

        bytes = tmp;
    }

    *res = count;

    return bytes;
}

jep_code_point *utf16_decode(
    const jep_byte *bytes,
    size_t n,
    size_t *res,
    int order)
{
    size_t i;             // index
    uint8_t seq[4];       // UTF-16BE encoded byte sequence
    size_t len;           // length of byte sequence
    size_t end;           // ending index of byte sequence
    uint8_t b;            // the current byte
    size_t d;             // difference of end - i
    jep_code_point point; // Unicode code point
    jep_code_point *pts;  // array of Unicode code points
    jep_code_point *npts; // code point reallocation buffer
    size_t c;             // number of decoded code points
    size_t cap;           // string capacity
    uint8_t tmp;          // used during byte swapping
    int swap;             // Byte Order Marker (BOM)
    int bom;              // Whether or not a BOM is present

    // Attempt to allocate an initial array of code points.
    // If the allocation fails, return NULL.
    cap = 10;
    pts = (jep_code_point *)malloc(sizeof(jep_code_point) * cap);

    if (pts == NULL)
    {
        *res = 0;
        return NULL;
    }

    // Check for the BOM to determine if we need to do byte swapping.
    // If the BOM is 0xFEFF, or if the BOM is not present, then we
    // don't do byte swapping. If the BOM is 0xFFEF, then we do byte
    // swapping.
    swap = 0;
    if (!order && n >= 2)
    {
        swap = (bytes[0] == 0xFE && bytes[1] == 0xFF)   ? 0
               : (bytes[0] == 0xFF && bytes[1] == 0xFE) ? 1
                                                        : 0;
    }

    // Determine if a BOM is present.
    bom = 0;
    if (n >= 2)
    {
        bom = (bytes[0] == 0xFE && bytes[1] == 0xFF)   ? 1
              : (bytes[0] == 0xFF && bytes[1] == 0xFE) ? 1
                                                       : 0;
    }

    // Group the bytes of the input array into byte sequences,
    // decode them into Unicode code points, then append those
    // code points to the output array.
    for (i = (!order && bom) ? 2 : 0, c = 0; i < n; i++)
    {
        if ((order == JEP_LITTLE_ENDIAN || swap) && i < n - 1)
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
        if (order == JEP_LITTLE_ENDIAN || swap)
        {
            // Swap the first two bytes
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
                npts = (jep_code_point *)realloc(pts, cap);

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

jep_byte *jep_utf16_encode(
    const jep_code_point *str,
    size_t n,
    size_t *res,
    int e,
    int bom)
{
    jep_byte *bytes; // Array of encoded bytes
    jep_byte *tmp;   // Temporary pointer used in resizing
    size_t i, j;     // Indices
    uint8_t seq[4];  // Array to hold a single byte sequence
    uint8_t swp;     // Temporary byte used for byte swapping
    size_t cap;      // Capacity of output array
    size_t new_cap;  // Temporary capacity for resizing
    size_t count;    // Total number of encoded bytes
    size_t len;      // Length of a single byte sequence

    len = 0;
    count = 0;
    cap = 50;
    bytes = (jep_byte *)malloc(sizeof(jep_byte) * cap);

    if (bytes == NULL)
        return NULL;

    if (bom)
    {
        bytes[count++] = e == JEP_LITTLE_ENDIAN ? 0xFF : 0xFE;
        bytes[count++] = e == JEP_LITTLE_ENDIAN ? 0xFE : 0xFF;
    }

    // Loop through the code points and place their encoded byte
    // sequences in the output array.
    for (i = 0; i < n; i++)
    {
        // Encode a single code point as a byte sequence.
        len = utf16_encode_point(str[i], seq);

        if (e == JEP_LITTLE_ENDIAN)
        {
            // Swap the first two bytes
            swp = seq[0];
            seq[0] = seq[1];
            seq[1] = swp;

            // Swap the second two bytes
            swp = seq[2];
            seq[2] = seq[3];
            seq[3] = swp;
        }

        // Put the encoded bytes in the output array.
        for (j = 0; j < len; j++)
        {
            // Resize the output array if necessary.
            if (count >= cap)
            {
                new_cap = cap + cap / 2;
                tmp = (jep_byte *)realloc(bytes, new_cap);

                if (tmp == NULL)
                    return NULL;

                bytes = tmp;
            }

            bytes[count++] = seq[j];
        }
    }

    // Resize the output array if excess memory was allocated.
    if (count < cap)
    {
        tmp = (jep_byte *)realloc(bytes, count);

        if (tmp == NULL)
            return NULL;

        bytes = tmp;
    }

    *res = count;

    return bytes;
}

jep_byte *jep_utf16be_encode(const jep_code_point *str, size_t n, size_t *res)
{
    return jep_utf16_encode(str, n, res, JEP_BIG_ENDIAN, 0);
}

jep_byte *jep_utf16le_encode(const jep_code_point *str, size_t n, size_t *res)
{
    return jep_utf16_encode(str, n, res, JEP_LITTLE_ENDIAN, 0);
}

jep_code_point *jep_utf16_decode(const jep_byte *bytes, size_t n, size_t *res)
{
    return utf16_decode(bytes, n, res, JEP_NO_ENDIAN);
}

jep_code_point *jep_utf16be_decode(const jep_byte *bytes, size_t n, size_t *res)
{
    return utf16_decode(bytes, n, res, JEP_BIG_ENDIAN);
}

jep_code_point *jep_utf16le_decode(const jep_byte *bytes, size_t n, size_t *res)
{
    return utf16_decode(bytes, n, res, JEP_LITTLE_ENDIAN);
}

/*-----------------------------------------------------------------*/
/*         Code Point Encoding and Decoding Implementation         */
/*-----------------------------------------------------------------*/

static jep_code_point utf8_decode_point(uint8_t *seq, size_t n)
{
    uint8_t b1; // U+....XX
    uint8_t b2; // U+..XX..
    uint8_t b3; // U+XX....

    uint8_t x;
    uint8_t y;
    uint8_t z;
    uint8_t v;

    x = y = z = v = 0;
    b1 = b2 = b3 = 0;

    // For single byte sequences, the code point is the first element.
    if (n == 1)
        return (jep_code_point)(seq[0]);

    if (n == 2)
    {
        // encoded sequence:
        // +---------------------+
        // | 1        | 0        |
        // +---------------------+
        // | 110xxxxx | 10yyyyyy |
        // +---------------------+
        x = jep_lo_5(seq[1]);
        y = jep_lo_6(seq[0]);

        // code point: 00000xxx xxyyyyyy
        b2 = x >> 2;
        b1 = (x << 6) | y;
    }
    else if (n == 3)
    {
        // encoded sequence:
        // +--------------------------------+
        // | 3        | 2        | 1        |
        // +--------------------------------+
        // | 1110xxxx | 10yyyyyy | 10zzzzzz |
        // +--------------------------------+
        x = jep_lo_3(seq[2]);
        y = jep_lo_6(seq[1]);
        z = jep_lo_6(seq[0]);

        // code point: xxxxyyyy yyzzzzzz
        b2 = (x << 4) | (y >> 2);
        b1 = (y << 6) | z;
    }
    else if (n == 4)
    {
        // encoded sequence:
        // +-------------------------------------------+
        // | 3        | 2        | 1        | 0        |
        // +-------------------------------------------+
        // | 11110xxx | 10yyyyyy | 10zzzzzz | 10vvvvvv |
        // +-------------------------------------------+
        x = jep_lo_3(seq[3]);
        y = jep_lo_6(seq[2]);
        z = jep_lo_6(seq[1]);
        v = jep_lo_6(seq[0]);

        // code point: 000xxxyy yyyyzzzz zzvvvvvv
        b3 = (x << 2) | (y >> 4);
        b2 = ((y & 0x0F) << 4) | (z >> 2);
        b1 = (z << 6) | v;
    }

    return (b3 << 16) | (b2 << 8) | b1;
}

static size_t utf8_encode_point(jep_code_point p, uint8_t *seq)
{
    uint8_t b1;
    uint8_t b2;
    uint8_t b3;

    seq[0] = seq[1] = seq[2] = seq[3] = 0;

    b1 = jep_lo_byte(p);
    b2 = jep_hi_byte(p);
    b3 = jep_lo_byte(jep_hi_word(p));

    if (p <= 0x7F)
    {
        seq[0] = b1;
        return 1;
    }

    if (p >= 0x80 && p <= 0x7FF)
    {
        // code point: 00000xxx xxyyyyyy

        // encoded sequence:
        // +---------------------+
        // | 0        | 1        |
        // +---------------------+
        // | 110xxxxx | 10yyyyyy |
        // +---------------------+
        seq[0] = (UTF8_2 << 5) | (jep_lo_3(b2) << 2) | jep_hi_2(b1);
        seq[1] = 0x80 | jep_lo_6(b1);

        return 2;
    }

    if (p >= 0x800 && p <= 0xFFFF)
    {
        // code point: xxxxyyyy yyzzzzzz

        // encoded sequence:
        // +--------------------------------+
        // | 3        | 2        | 1        |
        // +--------------------------------+
        // | 1110xxxx | 10yyyyyy | 10zzzzzz |
        // +--------------------------------+
        seq[0] = (UTF8_3 << 4) | jep_hi_4(b2);
        seq[1] = 0x80 | (jep_lo_4(b2) << 2) | jep_hi_2(b1);
        seq[2] = 0x80 | jep_lo_6(b1);

        return 3;
    }

    if (p >= 0x10000 && p <= 0x10FFFF)
    {
        // code point: 000xxxyy yyyyzzzz zzvvvvvv

        // encoded sequence:
        // +-------------------------------------------+
        // | 3        | 2        | 1        | 0        |
        // +-------------------------------------------+
        // | 11110xxx | 10yyyyyy | 10zzzzzz | 10vvvvvv |
        // +-------------------------------------------+
        seq[0] = (UTF8_4 << 3) | (jep_lo_5(b3) >> 2);
        seq[1] = 0x80 | (jep_lo_2(b3) << 4) | jep_hi_4(b2);
        seq[2] = 0x80 | (jep_lo_4(b2) << 2) | jep_hi_2(b1);
        seq[3] = 0x80 | jep_lo_6(b1);

        return 4;
    }

    return 0;
}

static jep_code_point utf16_decode_point(uint8_t *seq, size_t n)
{
    uint32_t hi; // high surrogate
    uint32_t lo; // low surrogate

    if (n == 2)
        return (seq[1] << 8) | seq[0];

    // The second element is the high half of thelow surrogate,
    // and the first element is the lower half.
    // Shift the second element to the left by 8 bits and OR it
    // with the first element. Then, subtract 0xDC00.
    lo = ((seq[1] << 8) | seq[0]) - 0xDC00;

    // The fourth element is the high half of the high surrogate,
    // and the third element is the lower half.
    // Shift the fourth element to the left by 8 bits and OR it
    // with the third element. Then, subtract 0xD800, and multiply
    // the difference by 0x400.
    hi = (((seq[3] << 8) | seq[2]) - 0xD800) * 0x400;

    // Add the high and low surrogates with 0x10000
    return lo + hi + 0x10000;
}

static size_t utf16_encode_point(jep_code_point p, uint8_t *seq)
{
    uint32_t hi; // high surrogate
    uint32_t lo; // low surrogate

    if (p >= 0x00 && p <= 0xFFFF)
    {
        seq[1] = jep_lo_byte(p);
        seq[0] = jep_hi_byte(p);
        return 2;
    }

    // Subtract 0x10000 from the code point
    p -= 0x10000;

    // For the low surrogate,
    // add 0xDC00 to the remainder of dividing the code point by 0x400.
    lo = p % 0x400 + 0xDC00;

    // For the high surrogate,
    // add 0xD800 to the quotient of the code point and 0x400.
    hi = p / 0x400 + 0xD800;

    seq[3] = jep_lo_byte(lo); // low half of low surrogate
    seq[2] = jep_hi_byte(lo); // high half of low surrogate
    seq[1] = jep_lo_byte(hi); // low half of high surrogate
    seq[0] = jep_hi_byte(hi); // high half of high surrogate

    return 4;
}
