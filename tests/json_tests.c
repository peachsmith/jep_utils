#include "json_tests.h"

int json_parse_test()
{
    int parsed;
    jep_string *jstr;
    jep_json_object *jobj;

    jstr = jep_new_string("{"
                          "\"data\" : \"bagel\","
                          "\"array\": [ 1, 2, 3 ],"
                          "\"object\": { \"name\": \"blueberry\" }"
                          "}");
    jobj = NULL;

    if (jstr == NULL)
        return 0;

    parsed = jep_parse_json_string(jstr, &jobj);

    if (jobj == NULL)
    {
        jep_destroy_string(jstr);
        return 0;
    }

    jep_destroy_string(jstr);
    jep_destroy_json_object(jobj);

    // jep_parse_json_string returns 0 on success
    return !parsed;
}

int json_field_test()
{
    int res;
    int parsed;
    jep_string *jstr;
    jep_string *fstr;
    jep_json_object *jobj;
    jep_json_field *jfield;
    jep_string *data;

    res = 1;
    jstr = jep_new_string("{ \"data\" : \"bagel\" }");
    fstr = jep_new_string("data");
    jobj = NULL;

    if (jstr == NULL && fstr != NULL)
    {
        jep_destroy_string(fstr);
        return 0;
    }
    else if (jstr != NULL && fstr == NULL)
    {
        jep_destroy_string(jstr);
        return 0;
    }

    parsed = jep_parse_json_string(jstr, &jobj);

    if (jobj == NULL)
    {
        jep_destroy_string(jstr);
        jep_destroy_string(fstr);
        return 0;
    }

    jfield = jep_get_json_field(jobj, fstr);

    if (parsed)
        res = 0;
    else if (jfield == NULL)
        res = 0;
    else if (jfield->value == NULL)
        res = 0;
    else if (jfield->value->data.raw == NULL)
        res = 0;
    else
    {
        data = jfield->value->data.raw;

        if (data->chars[0] != 0x62 || data->chars[1] != 0x61 || data->chars[2] != 0x67 || data->chars[3] != 0x65 || data->chars[4] != 0x6C)
            res = 0;
    }

    jep_destroy_string(jstr);
    jep_destroy_string(fstr);
    jep_destroy_json_object(jobj);

    return res;
}
