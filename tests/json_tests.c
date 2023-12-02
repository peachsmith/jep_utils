#include "json_tests.h"

#include <stdio.h>

int json_parse_test()
{
    int parsed;
    jep_string *jstr;
    jep_json_object *jobj;

    jstr = jep_new_string("{"
                          "\"data\" : \"bagel\","
                          "\"array\": [ 4, \"four\", { \"price\": 100 } ],"
                          "\"object\": { \"name\": \"blueberry\" }"
                          "}");
    jobj = NULL;

    if (jstr == NULL)
    {
        return 0;
    }

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
    jep_string *data;

    jep_json_object *jobj;
    jep_json_field *jfield;

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
        {
            res = 0;
        }
    }

    jep_destroy_string(jstr);
    jep_destroy_string(fstr);

    jep_destroy_json_object(jobj);

    return res;
}

static void print_json_value(jep_json_value *jval, int indent, int shouldIndent, int comma);
static void print_json_array(jep_json_array *jarr, int indent, int shouldIndent, int comma);
static void print_json_object(jep_json_object *jobj, int indent, int shouldIndent, int comma);

static void print_json_value(jep_json_value *jval, int indent, int shouldIndent, int comma)
{
    switch (jval->type)
    {
    case JSON_VALUE_NUMBER:
    case JSON_VALUE_STRING:
    case JSON_VALUE_BOOLEAN:
    case JSON_VALUE_NULL:
        if (shouldIndent)
        {
            for (int i = 0; i < indent; i++)
            {
                putc(' ', stdout);
            }
        }

        if (jval->type == JSON_VALUE_STRING)
        {
            putc('"', stdout);
        }

        printf("%s", jep_c_str(jval->data.raw));

        if (jval->type == JSON_VALUE_STRING)
        {
            putc('"', stdout);
        }

        if (comma)
        {
            printf(",");
        }

        printf("\n");

        break;

    case JSON_VALUE_ARRAY:
        print_json_array(jval->data.arr, indent + 1, shouldIndent, comma);
        break;

    case JSON_VALUE_OBJECT:
        print_json_object(jval->data.obj, indent + 1, shouldIndent, comma);
        break;

    default:
        break;
    }
}

static void print_json_array(jep_json_array *jarr, int indent, int shouldIndent, int comma)
{
    if (shouldIndent)
    {
        for (int i = 0; i < indent - 1; i++)
        {
            putc(' ', stdout);
        }
    }
    printf("[\n");

    jep_json_value *v = jarr->values;
    while (v != NULL)
    {
        print_json_value(v, indent, 1, v->next != NULL);
        v = v->next;
    }

    for (int i = 0; i < indent - 1; i++)
    {
        putc(' ', stdout);
    }
    printf("]");
    if (comma)
    {
        printf(",");
    }

    printf("\n");
}

static void print_json_object(jep_json_object *jobj, int indent, int shouldIndent, int comma)
{
    if (shouldIndent)
    {
        for (int i = 0; i < indent - 1; i++)
        {
            putc(' ', stdout);
        }
    }
    printf("{\n");

    jep_json_field *f = jobj->fields;
    while (f != NULL)
    {
        for (int i = 0; i < (indent ? indent : indent + 1); i++)
        {
            putc(' ', stdout);
        }
        printf("\"%s\": ", jep_c_str(f->name));
        print_json_value(f->value, indent + 1, 0, f->next != NULL);
        f = f->next;
    }

    for (int i = 0; i < indent - 1; i++)
    {
        putc(' ', stdout);
    }
    printf("}");
    if (comma)
    {
        printf(",");
    }

    printf("\n");
}

int json_nested_field_test()
{
    int parsed;
    jep_string *jstr;
    jep_string *arr_str;
    jep_string *obj_str;

    jep_json_object *jobj;
    jep_json_field *arr_field;
    jep_json_field *obj_field;

    jstr = jep_new_string(" {                                        "
                          "   \"nullField\": null,                   "
                          "   \"boolField\": true,                   "
                          "   \"stringField\": \"bagel\",            "
                          "   \"arrayField\": [                      "
                          "     4,                                   "
                          "     \"four\",                            "
                          "     { \"price\": 100 },                  "
                          "     [ \"A\", { \"B\": null }, [ true ] ] "
                          "   ],                                     "
                          "   \"objectField\": {                     "
                          "     \"flavor\": \"blueberry\",           "
                          "     \"namedArray\": [ 1, 2, 3 ],         "
                          "     \"nestedObject\": {                  "
                          "       \"nestedField\": 42                "
                          "     }                                    "
                          "   }                                      "
                          " }                                        ");
    arr_str = jep_new_string("arrayField");
    obj_str = jep_new_string("objectField");
    jobj = NULL;

    if (jstr == NULL)
    {
        return 0;
    }

    int res = 1;

    parsed = jep_parse_json_string(jstr, &jobj);

    if (jobj == NULL)
    {
        jep_destroy_string(jstr);
        return 0;
    }

    if (parsed)
    {
        res = 0;
    }

    arr_field = jep_get_json_field(jobj, arr_str);

    if (arr_field->value->type != JSON_VALUE_ARRAY)
    {
        res = 0;
    }

    if (arr_field->value->data.arr->values->type != JSON_VALUE_NUMBER)
    {
        res = 0;
    }

    const char *e0 = jep_c_str(arr_field->value->data.arr->values->data.raw);

    if (e0[0] != '4')
    {
        res = 0;
    }

    // TODO: remove this from this project and put it somewhere more appropriate.
    print_json_object(jobj, 0, 0, 0);

    jep_destroy_string(jstr);
    jep_destroy_string(arr_str);
    jep_destroy_string(obj_str);

    jep_destroy_json_object(jobj);

    return res;
}
