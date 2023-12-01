#include "byte_buffer_tests.h"

int byte_buffer_create_test()
{
    jep_byte_buffer *bb;

    bb = jep_create_byte_buffer();

    if (bb == NULL)
        return 0;

    jep_destroy_byte_buffer(bb);

    return 1;
}

int byte_buffer_append_byte_test()
{
    int res;
    jep_byte_buffer *bb;
    jep_byte b;

    bb = jep_create_byte_buffer();

    if (bb == NULL)
        return 0;

    b = (jep_byte)4U;

    res = jep_append_byte(bb, b);

    jep_destroy_byte_buffer(bb);

    return res;
}

int byte_buffer_append_bytes_test()
{
    int res;
    jep_byte_buffer *bb;
    jep_byte b[3] = {4, 5, 6};

    bb = jep_create_byte_buffer();

    if (bb == NULL)
        return 0;

    res = jep_append_bytes(bb, b, 3);

    jep_destroy_byte_buffer(bb);

    return res == 3 ? 1 : 0;
}