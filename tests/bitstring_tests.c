#include "bitstring_tests.h"

int bitstring_create_test()
{
    jep_bitstring *b = jep_create_bitstring();
    if (b == NULL)
        return 0;

    jep_destroy_bitstring(b);

    return 1;
}

int bitstring_push_bit_test()
{
    jep_bitstring *b = jep_create_bitstring();

    if (b == NULL)
        return 0;

    if (!jep_push_bit(b, 1))
    {
        jep_destroy_bitstring(b);
        return 0;
    }

    if (jep_get_bit(b, 0) != 1)
    {
        jep_destroy_bitstring(b);
        return 0;
    }

    jep_destroy_bitstring(b);

    return 1;
}

int bitstring_pop_bit_test()
{
    jep_bitstring *b = jep_create_bitstring();

    if (b == NULL)
        return 0;

    if (!jep_push_bit(b, 1))
    {
        jep_destroy_bitstring(b);
        return 0;
    }

    if (b->bit_count != 1)
    {
        jep_destroy_bitstring(b);
        return 0;
    }

    if (!jep_pop_bit(b))
    {
        jep_destroy_bitstring(b);
        return 0;
    }

    if (b->bit_count != 0)
    {
        jep_destroy_bitstring(b);
        return 0;
    }

    jep_destroy_bitstring(b);

    return 1;
}

int bitstring_concat_bits_test()
{
    jep_bitstring *b1 = jep_create_bitstring();
    jep_bitstring *b2 = jep_create_bitstring();

    if (b1 == NULL || b2 == NULL)
    {
        jep_destroy_bitstring(b1);
        jep_destroy_bitstring(b2);
        return 0;
    }

    // Populate the first bitstring.
    jep_push_bit(b1, 0);
    jep_push_bit(b1, 1);
    jep_push_bit(b1, 0);

    // Populate the second bitstring.
    jep_push_bit(b2, 1);
    jep_push_bit(b2, 0);
    jep_push_bit(b2, 1);

    // Add the contents of the second bitstring to the first.
    if (jep_concat_bits(b1, b2) != 3)
    {
        jep_destroy_bitstring(b1);
        jep_destroy_bitstring(b2);
        return 0;
    }

    // Verify the last bit in the first bitstring.
    if (jep_get_bit(b1, 5) != 1)
    {
        jep_destroy_bitstring(b1);
        jep_destroy_bitstring(b2);
        return 0;
    }

    jep_destroy_bitstring(b1);
    jep_destroy_bitstring(b2);

    return 1;
}

int bitstring_get_bit_out_of_bounds_test()
{
    jep_bitstring *b = jep_create_bitstring();

    if (b == NULL)
        return 0;

    jep_push_bit(b, 1);

    if (jep_get_bit(b, 2) != -1)
    {
        jep_destroy_bitstring(b);
        return 0;
    }

    jep_destroy_bitstring(b);

    return 1;
}

int bitstring_set_bit_test()
{
    jep_bitstring *b = jep_create_bitstring();

    if (b == NULL)
        return 0;

    jep_push_bit(b, 1);
    if (jep_get_bit(b, 0) != 1)
    {
        jep_destroy_bitstring(b);
        return 0;
    }

    jep_set_bit(b, 0, 0);
    if (jep_get_bit(b, 0) != 0)
    {
        jep_destroy_bitstring(b);
        return 0;
    }

    jep_destroy_bitstring(b);

    return 1;
}
