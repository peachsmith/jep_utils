#include "jep_utils.h"

jep_string *jep_create_string(size_t size)
{
    jep_string *str;
    jep_char *chars;
    char *c_chars;

    str = (jep_string *)malloc(sizeof(jep_string));

    if (str == NULL)
    {
        return NULL;
    }

    str->size = size;
    str->chars = NULL;
    str->c_buf = NULL;

    if (size > 0)
    {
        // number of jep_char elements == size
        chars = (jep_char *)malloc(sizeof(jep_char) * size);
        if (chars == NULL)
        {
            free(str);
            return NULL;
        }
        str->chars = chars;

        // number of char elements == sizeof(jep_char) * size + 1
        // We add 1 element for the NUL terminator.
        c_chars = (char *)malloc(sizeof(jep_char) * size + 1);
        if (c_chars == NULL)
        {
            free(chars);
            free(str);
            return NULL;
        }
        str->c_buf = c_chars;
    }

    return str;
}

void jep_destroy_string(jep_string *str)
{
    if (str == NULL)
    {
        return;
    }

    if (str->chars != NULL)
    {
        free(str->chars);
    }

    if (str->c_buf != NULL)
    {
        free(str->c_buf);
    }

    free(str);
}

jep_string *jep_bytes_to_string(const jep_byte *bytes, int encoding, size_t n)
{
    jep_string *str;
    jep_code_point *pts;
    size_t res;

    res = 0;
    pts = NULL;
    str = jep_create_string(0);

    if (str == NULL)
    {
        return NULL;
    }

    switch (encoding)
    {
    case JEP_ENCODING_UTF_8:
        pts = jep_utf8_decode(bytes, n, &res);
        break;

    case JEP_ENCODING_UTF_16:
        pts = jep_utf16_decode(bytes, n, &res);
        break;

    case JEP_ENCODING_UTF_16_BE:
        pts = jep_utf16be_decode(bytes, n, &res);
        break;

    case JEP_ENCODING_UTF_16_LE:
        pts = jep_utf16le_decode(bytes, n, &res);
        break;

    default:
        break;
    }

    if (pts == NULL)
    {
        jep_destroy_string(str);
        return NULL;
    }

    if (res == 0)
    {
        jep_destroy_string(str);
        free(pts);
        return NULL;
    }

    // number of char elements == sizeof(jep_char) * size + 1
    // We add 1 element for the NUL terminator.
    char *c_chars = (char *)malloc(sizeof(jep_char) * n + 1);
    if (c_chars == NULL)
    {
        jep_destroy_string(str);
        free(pts);
        return NULL;
    }

    str->size = res;
    str->chars = pts;
    str->c_buf = c_chars;

    return str;
}

jep_string *jep_chars_to_string(jep_char *chars, size_t n)
{
    jep_string *str;
    size_t i;

    str = jep_create_string(n);

    if (str == NULL)
    {
        return NULL;
    }

    for (i = 0; i < n; i++)
    {
        str->chars[i] = chars[i];
    }

    return str;
}

int jep_strcmp(jep_string *a, jep_string *b)
{
    if (a == NULL && b == NULL)
    {
        return 0;
    }

    if (a != NULL && b == NULL)
    {
        return 1;
    }

    if (a == NULL && b != NULL)
    {
        return -1;
    }

    if (a->size > b->size)
    {
        return 1;
    }

    if (a->size < b->size)
    {
        return -1;
    }

    size_t i;
    for (i = 0; i < a->size; i++)
    {
        if (a->chars[i] < b->chars[i])
        {
            return -1;
        }
        else if (a->chars[i] > b->chars[i])
        {
            return 1;
        }
    }

    return 0;
}

int jep_strcpy(jep_string *src, jep_string *dest)
{
    if (src == NULL || dest == NULL)
    {
        return 0;
    }

    if (src->chars == NULL)
    {
        return 0;
    }

    if (src->size == 0)
    {
        return 0;
    }

    size_t i;
    jep_char *nchars;
    char *ncstr;

    // make sure we have a destination buffer
    if (dest->chars == NULL)
    {
        dest->chars = (jep_char *)malloc(sizeof(jep_char) * src->size);

        if (dest->chars == NULL)
        {
            return 0;
        }

        dest->size = src->size;
    }

    // make sure we have a C string buffer in the destination
    if (dest->c_buf == NULL)
    {
        dest->c_buf = (char *)malloc(sizeof(jep_char) * src->size + 1);

        if (dest->c_buf == NULL)
        {
            return 0;
        }
    }

    // make sure that the destination buffer size is equal to the
    // source buffer size.
    if (src->size != dest->size)
    {
        nchars = jep_realloc(dest->chars, jep_char, src->size);

        if (nchars != NULL)
        {
            dest->chars = nchars;
        }
        else
        {
            return 0;
        }

        ncstr = jep_realloc(dest->chars, char, sizeof(jep_char) * src->size + 1);

        if (ncstr != NULL)
        {
            dest->c_buf = ncstr;
        }
        else
        {
            return 0;
        }

        dest->size = src->size;
    }

    for (i = 0; i < src->size; i++)
    {
        dest->chars[i] = src->chars[i];
    }

    return 1;
}

jep_string *jep_new_string(const char *lit)
{
    return jep_bytes_to_string((const jep_byte *)lit, JEP_ENCODING_UTF_8, strlen(lit));
}

jep_char jep_char_at(jep_string *str, size_t i)
{
    return i < str->size ? str->chars[i] : JEP_CHAR_NONE;
}

long jep_string_to_long(jep_string *str, int base)
{
    size_t i;
    size_t size;
    jep_char *raw;
    long l;
    char *end;
    char *buffer;

    if (str == NULL)
    {
        return 0;
    }

    l = 0;
    size = str->size;
    raw = str->chars;

    if (size < 1)
    {
        return 0;
    }

    buffer = (char *)malloc(sizeof(char) * size + 1);

    if (buffer == NULL)
    {
        return 0;
    }

    for (i = 0; i < size; i++)
    {
        // if (raw[i].b3 == 0 && raw[i].b2 == 0)
        if (raw[i] <= 0x000000FF)
        {
            buffer[i] = (str->chars[i] & 0xFF);
        }
        else
        {
            buffer[i] = '\0';
        }
    }

    buffer[i] = '\0';

    l = strtol(buffer, &end, base);

    free(buffer);

    return l;
}

unsigned long jep_string_to_ulong(jep_string *str, int base)
{
    size_t i;
    size_t size;
    jep_char *raw;
    unsigned long l;
    char *end;
    char *buffer;

    if (str == NULL)
    {
        return 0;
    }

    l = 0;
    size = str->size;
    raw = str->chars;

    if (size < 1)
    {
        return 0;
    }

    buffer = (char *)malloc(sizeof(char) * size + 1);

    if (buffer == NULL)
    {
        return 0;
    }

    for (i = 0; i < size; i++)
    {
        // if (raw[i].b3 == 0 && raw[i].b2 == 0)
        if (raw[i] <= 0x000000FF)
        {
            buffer[i] = (str->chars[i] & 0xFF);
        }
        else
        {
            buffer[i] = '\0';
        }
    }

    buffer[i] = '\0';

    l = strtoul(buffer, &end, base);

    free(buffer);

    return l;
}

int jep_string_to_int(jep_string *str, int base)
{
    size_t i;
    size_t size;
    jep_char *raw;
    long l;
    int n;
    char *end;
    char *buffer;

    if (str == NULL)
    {
        return 0;
    }

    l = 0;
    size = str->size;
    raw = str->chars;

    if (size < 1)
    {
        return 0;
    }

    buffer = (char *)malloc(sizeof(char) * size + 1);

    if (buffer == NULL)
    {
        return 0;
    }

    for (i = 0; i < size; i++)
    {
        // if (raw[i].b3 == 0 && raw[i].b2 == 0)
        if (raw[i] <= 0x000000FF)
        {
            buffer[i] = (str->chars[i] & 0xFF);
        }
        else
        {
            buffer[i] = '\0';
        }
    }

    buffer[i] = '\0';

    l = strtol(buffer, &end, base);

    free(buffer);

    n = l >= INT_MIN && l <= INT_MAX ? l : 0;

    return n;
}

unsigned int jep_string_to_uint(jep_string *str, int base)
{
    size_t i;
    size_t size;
    jep_char *raw;
    unsigned long l;
    unsigned int n;
    char *end;
    char *buffer;

    if (str == NULL)
    {
        return 0;
    }

    l = 0;
    size = str->size;
    raw = str->chars;

    if (size < 1)
    {
        return 0;
    }

    buffer = (char *)malloc(sizeof(char) * size + 1);

    if (buffer == NULL)
    {
        return 0;
    }

    for (i = 0; i < size; i++)
    {
        // if (raw[i].b3 == 0 && raw[i].b2 == 0)
        if (raw[i] <= 0x000000FF)
        {
            buffer[i] = (str->chars[i] & 0xFF);
        }
        else
        {
            buffer[i] = '\0';
        }
    }

    buffer[i] = '\0';

    l = strtoul(buffer, &end, base);

    free(buffer);

    n = l >= 0 && l <= UINT_MAX ? l : 0;

    return n;
}

double jep_string_to_double(jep_string *str)
{
    size_t i;
    size_t size;
    jep_char *raw;
    double d;
    char *end;
    char *buffer;

    if (str == NULL)
    {
        return 0;
    }

    d = 0;
    size = str->size;
    raw = str->chars;

    if (size < 1)
    {
        return 0;
    }

    buffer = (char *)malloc(sizeof(char) * size + 1);

    if (buffer == NULL)
    {
        return 0;
    }

    for (i = 0; i < size; i++)
    {
        // if (raw[i].b3 == 0 && raw[i].b2 == 0)
        if (raw[i] <= 0x000000FF)
        {
            buffer[i] = (str->chars[i] & 0xFF);
        }
        else
        {
            buffer[i] = '\0';
        }
    }

    buffer[i] = '\0';

    d = strtod(buffer, &end);

    free(buffer);

    return d;
}

const char *jep_c_str(jep_string *str)
{
    const char *c_str = NULL;
    size_t end = 0;
    size_t jchar_size = sizeof(jep_char);

    for (size_t i = 0; i < str->size; i++)
    {
        size_t offset = 0;
        for (size_t j = jchar_size; j > 0; j--)
        {
            char c = ((str->chars[i] >> ((j - 1) * CHAR_BIT)) & UCHAR_MAX);
            if (c)
            {
                str->c_buf[end++] = c;
            }
        }
    }

    str->c_buf[end] = '\0';

    c_str = str->c_buf;

    return c_str;
}
