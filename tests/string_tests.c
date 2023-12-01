#include "string_tests.h"

int string_create_test()
{
    jep_string *s;
    int res;

    s = jep_create_string(5);

    if (s == NULL)
        return 0;

    res = s->size == 5 ? 1 : 0;

    jep_destroy_string(s);

    return res;
}

int empty_string_test()
{
    jep_string *s;
    int res;

    s = jep_create_string(0);

    if (s == NULL)
        return 0;

    res = s->size == 0 ? 1 : 0;

    jep_destroy_string(s);

    return res;
}

int bytes_to_string_test()
{
    const char *c_str = "bagel";
    jep_string *str;
    int res = 1;
    int enc = JEP_ENCODING_UTF_8;

    str = jep_bytes_to_string((const jep_byte *)c_str, enc, strlen(c_str));

    if (str == NULL)
        return 0;

    if (str->size != 5)
        res = 0;

    if (str->chars[0] != 0x62)
        res = 0;

    if (str->chars[4] != 0x6C)
        res = 0;

    jep_destroy_string(str);

    return res;
}

int bytes_to_string_utf8_test()
{
    jep_byte data[10] = {
        0x24,                  // U+0024
        0xC2, 0xA2,            // U+00A2
        0xE0, 0xA4, 0xB9,      // U+0939
        0xF0, 0x90, 0x8D, 0x88 // U+10348
    };

    jep_string *str;
    int res;

    str = jep_bytes_to_string(data, JEP_ENCODING_UTF_8, 10);

    if (str == NULL)
    {
        return 0;
    }

    if (str->size != 4)
    {
        jep_destroy_string(str);
        return 0;
    }

    res = 1;

    if (str->chars[0] != 0x24)
        res = 0;
    if (str->chars[1] != 0xA2)
        res = 0;
    if (str->chars[2] != 0x0939)
        res = 0;
    if (str->chars[3] != 0x10348)
        res = 0;

    jep_destroy_string(str);

    return res;
}

int bytes_to_string_utf16_be_bom_test()
{
    jep_byte data[14] =
        {
            0xFE, 0xFF,             // U+FEFF (BOM)
            0x00, 0x24,             // U+0024
            0x20, 0xAC,             // U+20AC
            0xD8, 0x01, 0xDC, 0x37, // U+10437
            0xD8, 0x52, 0xDF, 0x62  // U+24B62
        };

    jep_string *str;
    int res;

    str = jep_bytes_to_string(data, JEP_ENCODING_UTF_16, 14);

    if (str == NULL)
    {
        return 0;
    }

    if (str->size != 4)
    {
        jep_destroy_string(str);
        return 0;
    }

    res = 1;

    if (str->chars[0] != 0x24)
        res = 0;
    if (str->chars[1] != 0x20AC)
        res = 0;
    if (str->chars[2] != 0x10437)
        res = 0;
    if (str->chars[3] != 0x24B62)
        res = 0;

    jep_destroy_string(str);

    return res;
}

int bytes_to_string_utf16_le_bom_test()
{
    jep_byte data[14] =
        {
            0xFF, 0xFE,             // U+FEFF (BOM)
            0x24, 0x00,             // U+0024
            0xAC, 0x20,             // U+20AC
            0x01, 0xD8, 0x37, 0xDC, // U+10437
            0x52, 0xD8, 0x62, 0xDF  // U+24B62
        };

    jep_string *str;
    int res;

    str = jep_bytes_to_string(data, JEP_ENCODING_UTF_16, 14);

    if (str == NULL)
    {
        return 0;
    }

    if (str->size != 4)
    {
        jep_destroy_string(str);
        return 0;
    }

    res = 1;

    if (str->chars[0] != 0x24)
        res = 0;
    if (str->chars[1] != 0x20AC)
        res = 0;
    if (str->chars[2] != 0x10437)
        res = 0;
    if (str->chars[3] != 0x24B62)
        res = 0;

    jep_destroy_string(str);

    return res;
}

int bytes_to_string_utf16be_test()
{
    jep_byte data[12] =
        {
            0x00, 0x24,             // U+0024
            0x20, 0xAC,             // U+20AC
            0xD8, 0x01, 0xDC, 0x37, // U+10437
            0xD8, 0x52, 0xDF, 0x62  // U+24B62
        };

    jep_string *str;
    int res;

    str = jep_bytes_to_string(data, JEP_ENCODING_UTF_16_BE, 12);

    if (str == NULL)
    {
        return 0;
    }

    if (str->size != 4)
    {
        jep_destroy_string(str);
        return 0;
    }

    res = 1;

    if (str->chars[0] != 0x24)
        res = 0;
    if (str->chars[1] != 0x20AC)
        res = 0;
    if (str->chars[2] != 0x10437)
        res = 0;
    if (str->chars[3] != 0x24B62)
        res = 0;

    jep_destroy_string(str);

    return res;
}

int bytes_to_string_utf16le_test()
{
    jep_byte data[12] =
        {
            0x24, 0x00,             // U+0024
            0xAC, 0x20,             // U+20AC
            0x01, 0xD8, 0x37, 0xDC, // U+10437
            0x52, 0xD8, 0x62, 0xDF  // U+24B62
        };

    jep_string *str;
    int res;

    str = jep_bytes_to_string(data, JEP_ENCODING_UTF_16_LE, 12);

    if (str == NULL)
    {
        return 0;
    }

    if (str->size != 4)
    {
        jep_destroy_string(str);
        return 0;
    }

    res = 1;

    if (str->chars[0] != 0x24)
        res = 0;
    if (str->chars[1] != 0x20AC)
        res = 0;
    if (str->chars[2] != 0x10437)
        res = 0;
    if (str->chars[3] != 0x24B62)
        res = 0;

    jep_destroy_string(str);

    return res;
}

int new_string_test()
{
    jep_string *str;

    str = jep_new_string("hello");

    if (str == NULL)
        return 0;
    else
        jep_destroy_string(str);

    return 1;
}

int string_compare_less_test()
{
    jep_string *a;
    jep_string *b;
    int res;
    int enc = JEP_ENCODING_UTF_8;

    const char *c_str_a = "abc";
    const char *c_str_b = "abd";

    a = jep_bytes_to_string((const jep_byte *)c_str_a, enc, strlen(c_str_a));

    if (a == NULL)
        return 0;

    b = jep_bytes_to_string((const jep_byte *)c_str_b, enc, strlen(c_str_b));

    if (b == NULL)
    {
        jep_destroy_string(a);
        return 0;
    }

    res = jep_strcmp(a, b) == -1 ? 1 : 0;

    jep_destroy_string(a);
    jep_destroy_string(b);

    return res;
}

int string_compare_greater_test()
{
    jep_string *a;
    jep_string *b;
    int res;
    int enc = JEP_ENCODING_UTF_8;

    const char *c_str_a = "abd";
    const char *c_str_b = "abc";

    a = jep_bytes_to_string((const jep_byte *)c_str_a, enc, strlen(c_str_a));

    if (a == NULL)
        return 0;

    b = jep_bytes_to_string((const jep_byte *)c_str_b, enc, strlen(c_str_b));

    if (b == NULL)
    {
        jep_destroy_string(a);
        return 0;
    }

    res = jep_strcmp(a, b) == 1 ? 1 : 0;

    jep_destroy_string(a);
    jep_destroy_string(b);

    return res;
}

int string_compare_equal_test()
{
    jep_string *a;
    jep_string *b;
    int res;
    int enc = JEP_ENCODING_UTF_8;

    const char *c_str_a = "abc";
    const char *c_str_b = "abc";

    a = jep_bytes_to_string((const jep_byte *)c_str_a, enc, strlen(c_str_a));

    if (a == NULL)
        return 0;

    b = jep_bytes_to_string((const jep_byte *)c_str_b, enc, strlen(c_str_b));

    if (b == NULL)
    {
        jep_destroy_string(a);
        return 0;
    }

    res = jep_strcmp(a, b) == 0 ? 1 : 0;

    jep_destroy_string(a);
    jep_destroy_string(b);

    return res;
}

int string_to_int_test()
{
    jep_string *str;
    const char *bytes = "1234";
    int n;
    int enc = JEP_ENCODING_UTF_8;
    int res;

    str = jep_bytes_to_string((const jep_byte *)bytes, enc, strlen(bytes));

    if (str == NULL)
        return 0;

    n = jep_string_to_int(str, 10);

    jep_destroy_string(str);

    res = n == 1234 ? 1 : 0;

    return res;
}

int string_to_uint_test()
{
    jep_string *str;
    const char *bytes = "1234";
    unsigned int n;
    int enc = JEP_ENCODING_UTF_8;
    int res;

    str = jep_bytes_to_string((const jep_byte *)bytes, enc, strlen(bytes));

    if (str == NULL)
        return 0;

    n = jep_string_to_uint(str, 10);

    jep_destroy_string(str);

    res = n == 1234 ? 1 : 0;

    return res;
}

int string_to_long_test()
{
    jep_string *str;
    const char *bytes = "1234";
    long n;
    int enc = JEP_ENCODING_UTF_8;
    int res;

    str = jep_bytes_to_string((const jep_byte *)bytes, enc, strlen(bytes));

    if (str == NULL)
        return 0;

    n = jep_string_to_long(str, 10);

    jep_destroy_string(str);

    res = n == 1234 ? 1 : 0;

    return res;
}

int string_to_ulong_test()
{
    jep_string *str;
    const char *bytes = "1234";
    unsigned long n;
    int enc = JEP_ENCODING_UTF_8;
    int res;

    str = jep_bytes_to_string((const jep_byte *)bytes, enc, strlen(bytes));

    if (str == NULL)
        return 0;

    n = jep_string_to_ulong(str, 10);

    jep_destroy_string(str);

    res = n == 1234 ? 1 : 0;

    return res;
}

int string_c_str_test()
{
    jep_string *str = jep_new_string("Hello, World!");

    if (str == NULL)
        return 0;

    const char *c_str = jep_c_str(str);
    if (c_str == NULL)
    {
        jep_destroy_string(str);
        return 0;
    }

    size_t len = strlen(c_str);
    if (len != 13)
    {
        jep_destroy_string(str);
        return 0;
    }

    jep_destroy_string(str);

    return 1;
}

int string_c_str_multibyte_test()
{
    jep_byte data[10] = {
        0x24,                  // U+0024
        0xC2, 0xA2,            // U+00A2
        0xE0, 0xA4, 0xB9,      // U+0939
        0xF0, 0x90, 0x8D, 0x88 // U+10348
    };

    jep_string *str;

    str = jep_bytes_to_string(data, JEP_ENCODING_UTF_8, 10);

    if (str == NULL)
    {
        return 0;
    }

    if (str->size != 4)
    {
        jep_destroy_string(str);
        return 0;
    }

    const char *c_str = jep_c_str(str);
    if (c_str == NULL)
    {
        jep_destroy_string(str);
        return 0;
    }

    size_t len = strlen(c_str);
    if (len != 7)
    {
        jep_destroy_string(str);
        return 0;
    }

    int res = 1;
    // 24 A2 09 39 01 03 48
    if ((unsigned char)c_str[0] != 0x24)
        res = 0;
    if ((unsigned char)c_str[1] != 0xA2)
        res = 0;
    if ((unsigned char)c_str[2] != 0x09)
        res = 0;
    if ((unsigned char)c_str[3] != 0x39)
        res = 0;
    if ((unsigned char)c_str[4] != 0x01)
        res = 0;
    if ((unsigned char)c_str[5] != 0x03)
        res = 0;
    if ((unsigned char)c_str[6] != 0x48)
        res = 0;

    jep_destroy_string(str);

    return res;
}
