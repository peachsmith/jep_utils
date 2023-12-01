#ifndef JEP_JSON_H
#define JEP_JSON_H

#include "jep_core.h"

/**
 * Parses a string of text into a JSON object.
 *
 * Params:
 *   jep_string - a string containing raw JSON text
 *   json_object - a reference to a pointer to a JSON object
 *
 * Returns:
 *   int - 0 on success, otherwise an error code.
 */
JEP_UTILS_API int JEP_UTILS_CALL
jep_parse_json_string(jep_string *raw, jep_json_object **obj);

/**
 * Frees the memory allocated for a JSON object.
 *
 * Params:
 *   json_object - a JSON object
 */
JEP_UTILS_API void JEP_UTILS_CALL
jep_destroy_json_object(jep_json_object *obj);

/**
 * Retrieves field from within a JSON object.
 *
 * Params:
 *   json_object - a JSON object
 *   const char - the name of the JSON field
 *
 * Returns:
 *   json_field
 */
JEP_UTILS_API jep_json_field *JEP_UTILS_CALL
jep_get_json_field(jep_json_object *obj, jep_string *name);

#endif
