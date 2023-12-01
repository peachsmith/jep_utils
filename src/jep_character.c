#include "jep_utils.h"

int jep_char_cmp(jep_char a, jep_char b)
{
    // Given the code points U+UUVVWW and U+XXYYZZ,
    // compare UU and XX, then VV and YY, then WW and ZZ.
    // return a.b3 > b.b3 ? 1
    //    : a.b3 < b.b3 ? -1
    //    : a.b2 > b.b2 ? 1
    //    : a.b2 < b.b2 ? -1
    //    : a.b1 > b.b1 ? 1
    //    : a.b1 < b.b1 ? -1
    //    : 0;

    return a < b   ? -1
           : a > b ? 1
                   : 0;
}
