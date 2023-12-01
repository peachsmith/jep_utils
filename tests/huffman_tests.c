#include "huffman_tests.h"

int huff_encode_test()
{
    jep_byte data[6] = {
        0x43, 0x41, 0x42, 0x43, 0x43, 0x42};

    jep_byte_buffer *raw;
    jep_byte_buffer *encoded;
    int res;

    raw = jep_create_byte_buffer();

    if (raw == NULL)
    {
        return 0;
    }

    jep_append_bytes(raw, data, 6);

    encoded = jep_huff_encode(raw);

    jep_destroy_byte_buffer(raw);

    if (encoded == NULL)
    {
        return 0;
    }

    res = 1;

    if (encoded->buffer[2] != 0x41)
        res = 0;

    if (encoded->buffer[15] != 0x42)
        res = 0;

    if (encoded->buffer[28] != 0x43)
        res = 0;

    jep_destroy_byte_buffer(encoded);

    return res;
}

int huff_decode_test()
{
    // The byte sequence 0x43, 0x41, 0x42, 0x43, 0x43, 0x42
    // encoded using Huffman coding
    jep_byte data[54] = {
        0x01, 0x02, 0x41, 0x03, 0x02,
        0x00, 0x00, 0x00, 0x01, 0x00,
        0x00, 0x00, 0x02, 0x03, 0x02,
        0x42, 0x03, 0x02, 0x00, 0x00,
        0x00, 0x01, 0x00, 0x00, 0x00,
        0x02, 0x01, 0x02, 0x43, 0x03,
        0x01, 0x00, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x01, 0x00,
        0x04, 0x05, 0x09, 0x00, 0x00,
        0x00, 0x02, 0x00, 0x00, 0x00,
        0x01, 0x8E, 0x00, 0x06};

    jep_byte_buffer *encoded;
    jep_byte_buffer *raw;
    int res;

    encoded = jep_create_byte_buffer();

    if (encoded == NULL)
        return 0;

    jep_append_bytes(encoded, data, 54);

    raw = jep_huff_decode(encoded);

    jep_destroy_byte_buffer(encoded);

    if (raw == NULL)
        return 0;

    res = 1;

    if (raw->buffer[0] != 0x43)
        res = 0;

    if (raw->buffer[1] != 0x41)
        res = 0;

    if (raw->buffer[2] != 0x42)
        res = 0;

    if (raw->buffer[3] != 0x43)
        res = 0;

    if (raw->buffer[4] != 0x43)
        res = 0;

    if (raw->buffer[5] != 0x42)
        res = 0;

    return res;
}

int huff_read_test()
{
    // The byte sequence 0x43, 0x41, 0x42, 0x43, 0x43, 0x42
    // encoded using Huffman coding
    jep_byte data[54] = {
        0x01, 0x02, 0x41, 0x03, 0x02,
        0x00, 0x00, 0x00, 0x01, 0x00,
        0x00, 0x00, 0x02, 0x03, 0x02,
        0x42, 0x03, 0x02, 0x00, 0x00,
        0x00, 0x01, 0x00, 0x00, 0x00,
        0x02, 0x01, 0x02, 0x43, 0x03,
        0x01, 0x00, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x01, 0x00,
        0x04, 0x05, 0x09, 0x00, 0x00,
        0x00, 0x02, 0x00, 0x00, 0x00,
        0x01, 0x8E, 0x00, 0x06};

    jep_byte_buffer *raw;
    jep_huff_code *huff;
    int res;

    raw = jep_create_byte_buffer();

    if (raw == NULL)
        return 0;

    jep_append_bytes(raw, data, 54);

    huff = jep_huff_read(raw);

    jep_destroy_byte_buffer(raw);

    if (huff == NULL)
        return 0;

    res = 1;

    if (huff->data->bit_count != 9)
        res = 0;

    if (huff->data->byte_count != 2)
        res = 0;

    if (huff->dict->count != 3)
        res = 0;

    if (huff->dict->symbols[0].b != 0x41)
        res = 0;

    if (huff->dict->symbols[1].b != 0x42)
        res = 0;

    if (huff->dict->symbols[2].b != 0x43)
        res = 0;

    jep_destroy_huff_code(huff);

    return res;
}
