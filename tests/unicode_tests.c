#include "unicode_tests.h"

int utf8_decode_test()
{
    jep_byte data[10] = {
        0x24,                  // U+0024
        0xC2, 0xA2,            // U+00A2
        0xE0, 0xA4, 0xB9,      // U+0939
        0xF0, 0x90, 0x8D, 0x88 // U+10348
    };

    jep_code_point *p;
    size_t dec;
    int res;

    p = jep_utf8_decode(data, 10, &dec);

    res = 1;

    if (dec != 4)
    {
        free(p);
        return 0;
    }

    if (p[0] != 0x0024)
        res = 0;
    if (p[1] != 0x00A2)
        res = 0;
    if (p[2] != 0x0939)
        res = 0;
    if (p[3] != 0x10348)
        res = 0;

    free(p);

    return res;
}

int utf8_encode_test()
{
    // expected UTF-8 encoded data:
    // 24 C2 A2 E0 A4 B9 F0 90 8D 88

    jep_code_point data[4] = {
        // byte sequence (hex)
        0x24,   // 24
        0xA2,   // C2 A2
        0x0939, // E0 A4 B9
        0x10348 // F0 90 8D 88
    };

    jep_byte *encoded;
    size_t s;
    int res;

    encoded = jep_utf8_encode(data, 4, &s);

    if (encoded == NULL)
        return 0;

    res = 1;

    if (s != 10)
    {
        free(encoded);
        res = 0;
    }

    if (encoded[0] != 0x24)
        res = 0;
    if (encoded[1] != 0xC2)
        res = 0;
    if (encoded[2] != 0xA2)
        res = 0;
    if (encoded[3] != 0xE0)
        res = 0;
    if (encoded[4] != 0xA4)
        res = 0;
    if (encoded[5] != 0xB9)
        res = 0;
    if (encoded[6] != 0xF0)
        res = 0;
    if (encoded[7] != 0x90)
        res = 0;
    if (encoded[8] != 0x8D)
        res = 0;
    if (encoded[9] != 0x88)
        res = 0;

    free(encoded);

    return res;
}

int utf16_be_bom_decode_test()
{
    jep_byte data[14] =
        {
            0xFE, 0xFF,             // U+FEFF (BOM)
            0x00, 0x24,             // U+0024
            0x20, 0xAC,             // U+20AC
            0xD8, 0x01, 0xDC, 0x37, // U+10437
            0xD8, 0x52, 0xDF, 0x62  // U+24B62
        };

    jep_code_point *p;
    size_t dec;
    int res;

    p = jep_utf16_decode(data, 14, &dec);

    res = 1;

    if (dec != 4)
    {
        free(p);
        return 0;
    }

    if (p[0] != 0x0024)
        res = 0;
    if (p[1] != 0x20AC)
        res = 0;
    if (p[2] != 0x10437)
        res = 0;
    if (p[3] != 0x24B62)
        res = 0;

    free(p);

    return res;
}

int utf16_be_bom_encode_test()
{
    jep_code_point data[4] = {
        // byte sequence (hex)
        0x24,    // 00 24
        0x20AC,  // 20 AC
        0x10437, // D8 01 DC 37
        0x24B62  // D8 52 DF 62
    };

    jep_byte *encoded;
    size_t s;
    int res;

    encoded = jep_utf16_encode(data, 4, &s, 1, 1);

    if (encoded == NULL)
        return 0;

    if (s != 14)
    {
        free(encoded);
        return 0;
    }

    res = 1;

    // expected UTF-16 encoded data:
    // FE FF 00 24 20 AC D8 01 DC 37 D8 52 DF 62
    if (encoded[0] != 0xFE)
        res = 0;
    if (encoded[1] != 0xFF)
        res = 0;
    if (encoded[2] != 0x00)
        res = 0;
    if (encoded[3] != 0x24)
        res = 0;
    if (encoded[4] != 0x20)
        res = 0;
    if (encoded[5] != 0xAC)
        res = 0;
    if (encoded[6] != 0xD8)
        res = 0;
    if (encoded[7] != 0x01)
        res = 0;
    if (encoded[8] != 0xDC)
        res = 0;
    if (encoded[9] != 0x37)
        res = 0;
    if (encoded[10] != 0xD8)
        res = 0;
    if (encoded[11] != 0x52)
        res = 0;
    if (encoded[12] != 0xDF)
        res = 0;
    if (encoded[13] != 0x62)
        res = 0;

    free(encoded);

    return res;
}

int utf16_le_bom_decode_test()
{
    jep_byte data[14] =
        {
            0xFF, 0xFE,             // U+FEFF (BOM)
            0x24, 0x00,             // U+0024
            0xAC, 0x20,             // U+20AC
            0x01, 0xD8, 0x37, 0xDC, // U+10437
            0x52, 0xD8, 0x62, 0xDF  // U+24B62
        };

    jep_code_point *p;
    size_t dec;
    int res;

    p = jep_utf16_decode(data, 14, &dec);

    res = 1;

    if (dec != 4)
    {
        free(p);
        return 0;
    }

    if (p[0] != 0x0024)
        res = 0;
    if (p[1] != 0x20AC)
        res = 0;
    if (p[2] != 0x10437)
        res = 0;
    if (p[3] != 0x24B62)
        res = 0;

    free(p);

    return res;
}

int utf16_be_decode_test()
{
    jep_byte data[12] =
        {
            0x00, 0x24,             // U+0024
            0x20, 0xAC,             // U+20AC
            0xD8, 0x01, 0xDC, 0x37, // U+10437
            0xD8, 0x52, 0xDF, 0x62  // U+24B62
        };

    jep_code_point *p;
    size_t dec;
    int res;

    p = jep_utf16be_decode(data, 12, &dec);

    res = 1;

    if (dec != 4)
    {
        free(p);
        return 0;
    }

    if (p[0] != 0x0024)
        res = 0;
    if (p[1] != 0x20AC)
        res = 0;
    if (p[2] != 0x10437)
        res = 0;
    if (p[3] != 0x24B62)
        res = 0;

    free(p);

    return res;
}

int utf16_be_encode_test()
{
    jep_code_point data[4] = {
        // byte sequence (hex)
        0x24,    // 00 24
        0x20AC,  // 20 AC
        0x10437, // D8 01 DC 37
        0x24B62  // D8 52 DF 62
    };

    jep_byte *encoded;
    size_t s;
    int res;

    encoded = jep_utf16be_encode(data, 4, &s);

    if (encoded == NULL)
        return 0;

    if (s != 12)
    {
        free(encoded);
        return 0;
    }

    res = 1;

    // expected UTF-16 encoded data:
    // 00 24 20 AC D8 01 DC 37 D8 52 DF 62
    if (encoded[0] != 0x00)
        res = 0;
    if (encoded[1] != 0x24)
        res = 0;
    if (encoded[2] != 0x20)
        res = 0;
    if (encoded[3] != 0xAC)
        res = 0;
    if (encoded[4] != 0xD8)
        res = 0;
    if (encoded[5] != 0x01)
        res = 0;
    if (encoded[6] != 0xDC)
        res = 0;
    if (encoded[7] != 0x37)
        res = 0;
    if (encoded[8] != 0xD8)
        res = 0;
    if (encoded[9] != 0x52)
        res = 0;
    if (encoded[10] != 0xDF)
        res = 0;
    if (encoded[11] != 0x62)
        res = 0;

    free(encoded);

    return res;
}

int utf16_le_decode_test()
{
    jep_byte data[12] =
        {
            0x24, 0x00,             // U+0024
            0xAC, 0x20,             // U+20AC
            0x01, 0xD8, 0x37, 0xDC, // U+10437
            0x52, 0xD8, 0x62, 0xDF  // U+24B62
        };

    jep_code_point *p;
    size_t dec;
    int res;

    p = jep_utf16le_decode(data, 12, &dec);

    res = 1;

    if (dec != 4)
    {
        free(p);
        return 0;
    }

    if (p[0] != 0x0024)
        res = 0;
    if (p[1] != 0x20AC)
        res = 0;
    if (p[2] != 0x10437)
        res = 0;
    if (p[3] != 0x24B62)
        res = 0;

    free(p);

    return res;
}

int utf16_le_encode_test()
{
    jep_code_point data[4] = {
        // byte sequence (hex)
        0x24,    // 00 24
        0x20AC,  // 20 AC
        0x10437, // D8 01 DC 37
        0x24B62  // D8 52 DF 62
    };

    jep_byte *encoded;
    size_t s;
    int res;

    encoded = jep_utf16le_encode(data, 4, &s);

    if (encoded == NULL)
        return 0;

    if (s != 12)
    {
        free(encoded);
        return 0;
    }

    res = 1;

    // expected UTF-16 encoded data:
    // 00 24 20 AC D8 01 DC 37 D8 52 DF 62
    if (encoded[0] != 0x24)
        res = 0;
    if (encoded[1] != 0x00)
        res = 0;
    if (encoded[2] != 0xAC)
        res = 0;
    if (encoded[3] != 0x20)
        res = 0;
    if (encoded[4] != 0x01)
        res = 0;
    if (encoded[5] != 0xD8)
        res = 0;
    if (encoded[6] != 0x37)
        res = 0;
    if (encoded[7] != 0xDC)
        res = 0;
    if (encoded[8] != 0x52)
        res = 0;
    if (encoded[9] != 0xD8)
        res = 0;
    if (encoded[10] != 0x62)
        res = 0;
    if (encoded[11] != 0xDF)
        res = 0;

    free(encoded);

    return res;
}