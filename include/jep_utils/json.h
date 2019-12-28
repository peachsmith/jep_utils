#ifndef JEP_JSON_H
#define JEP_JSON_H

#include "jep_utils.h"
#include "string.h"




/**
 * A JSON token classifies a string of raw text within the context of JSON
 * syntax.
 */
typedef struct jep_json_token {
	jep_string* str;
	int type;
	struct jep_json_token* next;
	struct jep_json_token* prev;
	struct jep_json_token* head;
	struct jep_json_token* tail;
}jep_json_token;

/**
 * Represents the current state of operations.
 * While tokenizing a raw string or parsing a stream of tokens, this
 * can be used to keep track of what the current action is.
 * For example, while parsing a stream of tokens, if a '{' token is
 * encountered, the state should be modified to indicate that the beginning
 * of an object has been encountered.
 */
typedef struct jep_json_state {
	int state;
	struct jep_json_state* top;
	struct jep_json_state* prev;
}jep_json_state;

/**
 * A single piece of data.
 * A JSON value can be one of the following:
 *
 *  +-------------------------------------------------+
 *  | Type    | Example                               |
 *  +-------------------------------------------------+
 *  | string  | "example string"                      |
 *  | number  | 4                                     |
 *  | boolean | true                                  |
 *  | null    | null                                  |
 *  | object  | { "exampleName" : "example value" }   |
 *  | array   | [ 1, 2, 3 ]                           |
 *  +-------------------------------------------------+
 */
typedef struct jep_json_value {
	int type;
	union {
		struct jep_json_object* obj;
		struct jep_json_array* arr;
		jep_string* raw;
	}data;
	struct jep_json_value* next;
}jep_json_value;

/**
 * A field is a name-value pair that exists within an object.
 * The textual representation of a JSON field should be a string of text,
 * followed by a colon then a value.
 * The value can be a number, a boolean, a string, an array, an object,
 * or null.
 *
 * Example:
 *   "someNumber" : 2.50
 */
typedef struct jep_json_field {
	jep_string* name;
	jep_json_value* value;
	struct jep_json_field* next;
}jep_json_field;

/**
 * A JSON array is a collection of JSON values without names.
 * Example:
 *   [ "toast", "bagel", "biscuit" ]
 */
typedef struct jep_json_array {
	jep_json_value* values;
	jep_json_value* tail;
}jep_json_array;

/**
 * A JSON object is a collection of fields.
 * Example:
 *   { "flavor" : "blueberry", "price" : 2.5 }
 */
typedef struct jep_json_object {
	jep_json_field* fields;
	jep_json_field* tail;
}jep_json_object;




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
jep_parse_json_string(jep_string* raw, jep_json_object** obj);

/**
 * Frees the memory allocated for a JSON object.
 *
 * Params:
 *   json_object - a JSON object
 */
JEP_UTILS_API void JEP_UTILS_CALL
jep_destroy_json_object(jep_json_object* obj);

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
JEP_UTILS_API jep_json_field* JEP_UTILS_CALL
jep_get_json_field(jep_json_object* obj, jep_string* name);

#endif
