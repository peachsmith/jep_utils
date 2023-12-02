#include "jep_utils.h"

/*-----------------------------------------------------------------*/
/*                        Token Functions                          */
/*-----------------------------------------------------------------*/

/**
 * Creates a new JSON token.
 * JSON_BEGIN is a valid token type in addition to being
 * a tokenization state.
 *
 * Params:
 *   int - the type of token
 *
 * Returns:
 *   json_token - a new JSON token or NULL on failure
 */
static jep_json_token *create_json_token(int type);

/**
 * Frees the memory allocated for a JSON token.
 *
 * Params:
 *   json_token - a reference to a JSON token
 */
static void destroy_json_token(jep_json_token *tok);

/**
 * Appends a JSON token onto a list of tokens.
 *
 * Params:
 *   json_token - a linked list of JSON tokens
 *   json_token - the new token to append
 */
static void add_json_token(jep_json_token *list, jep_json_token *next);

/*-----------------------------------------------------------------*/
/*                  State Management Functions                     */
/*-----------------------------------------------------------------*/

/**
 * Creates a new JSON tokenization state.
 *
 * Params:
 *   int - the integer value of the state
 *
 * Returns:
 *   json_state - a new JSON state
 */
static jep_json_state *create_json_state(int state);

/**
 * Frees the memory allocated for a JSON tokenization state.
 *
 * Params:
 *   json_state - a reference to a JSON tokenization state
 */
static void destroy_json_state(jep_json_state *state);

/**
 * Pushes a JSON tokenization onto a stack of JSON
 * tokenization states.
 *
 * Params:
 *   json_state - a stack of JSON tokenization states
 *   json_state - a new JSON tokenization state
 */
static void push_state(jep_json_state *stack, jep_json_state *state);

/**
 * Pops the top JSON tokenization state from the stack.
 *
 * Params:
 *   json_state - a stack of JSON tokenization states
 *
 * Returns:
 *   json_state - the former top of the stack
 */
static jep_json_state *pop_state(jep_json_state *stack);

/**
 * Frees the memory allocated for a JSON tokenization state stack.
 *
 * Params:
 *   json_state - a reference to a JSON tokenization state stack
 */
static void destroy_json_state_stack(jep_json_state *state);

/*-----------------------------------------------------------------*/
/*                     Token Value Functions                       */
/*-----------------------------------------------------------------*/

/**
 * Determines if a character is considered whitespace.
 *
 * Params:
 *   jep_char - a character
 *
 * Returns:
 *   int - 1 if the character is whitespace, otherwise 0
 */
static int is_whitespace(jep_char c);

/**
 * Determines if a character is a valid escape character.
 *
 * Params:
 *   jep_char - a character
 *
 * Returns:
 *   int - 1 if the character is escapable, otherwise 0
 */
static int is_escape(jep_char c);

/**
 * Unescapes a character.
 * For example, unescaping the 'n' character should result in '\n'.
 *
 * Params:
 *   jep_char - a character
 *
 * Returns:
 *   int - 1 if the character is escapable, otherwise 0
 */
static jep_char unescape(jep_char c);

/**
 * Determines if a character is a valid number character.
 *
 * Params:
 *   jep_char - a character
 *   int - the base (e.g. 10 for decimal or 16 for hexidecimal)
 *
 * Returns:
 *   int - 1 if the character is part of a number, otherwise 0
 */
static int is_number(jep_char c, int base);

/**
 * Determines if a character is a valid starting character
 * for a numerical value.
 *
 * Params:
 *   jep_char - a character
 *
 * Returns:
 *   int - 1 if the character is part of a number, otherwise 0
 */
static int is_number_start(jep_char c);

/**
 * Determines if a character is a valid boolean character.
 *
 * Params:
 *   jep_char - a character
 *
 * Returns:
 *   int - 1 if the character is part of a number, otherwise 0
 */
static int is_boolean(jep_char c);

/**
 * Determines if a character is a valid starting character
 * for a boolean value.
 *
 * Params:
 *   jep_char - a character
 *
 * Returns:
 *   int - 1 if the character is part of a number, otherwise 0
 */
static int is_boolean_start(jep_char c);

/**
 * Determines if a character is a valid null character.
 *
 * Params:
 *   jep_char - a character
 *
 * Returns:
 *   int - 1 if the character is part of a number, otherwise 0
 */
static int is_null(jep_char c);

/**
 * Determines if a character is a valid starting character
 * for a null value.
 *
 * Params:
 *   jep_char - a character
 *
 * Returns:
 *   int - 1 if the character is part of a number, otherwise 0
 */
static int is_null_start(jep_char c);

/*-----------------------------------------------------------------*/
/*                    Object Creation Functions                    */
/*-----------------------------------------------------------------*/

/**
 * Creates a new, empty JSON object.
 *
 * Returns:
 *   jep_json_object
 */
static jep_json_object *create_json_object();

/**
 * Creates a new JSON field
 *
 * Returns:
 *   jep_json_field
 */
static jep_json_field *create_json_field();

/**
 * Creates a new JSON array
 *
 * Returns:
 *   jep_json_array
 */
static jep_json_array *create_json_array();

/**
 * Creates a new JSON value
 *
 * Returns:
 *   jep_json_value
 */
static jep_json_value *create_json_value();

/**
 * Frees the memory allocated for a JSON field.
 *
 * Params:
 *   jep_json_field - the JSON field to destroy
 */
static void destroy_json_field(jep_json_field *f);

/**
 * Frees the memory allocated for a JSON array.
 *
 * Params:
 *   jep_json_field - the JSON array to destroy
 */
static void destroy_json_array(jep_json_array *a);

/**
 * Frees the memory allocated for a JSON value.
 *
 * Params:
 *   jep_json_field - the JSON value to destroy
 */
static void destroy_json_value(jep_json_value *v);

/**
 * Adds a JSON value to a JSON array.
 *
 * Params:
 *   jep_json_array - a JSON array to receive the value
 *   jep_json_value - a JSON value to be inserted into the array
 */
static void append_value(jep_json_array *arr, jep_json_value *val);

/**
 * Adds a JSON field to a JSON object.
 *
 * Params:
 *   jep_json_object - a JSON object to recieve the field
 *   jep_json_field - a JSON field to be inserted into the object
 */
static void append_field(jep_json_object *obj, jep_json_field *field);

/*-----------------------------------------------------------------*/
/*                        Parsing Functions                        */
/*-----------------------------------------------------------------*/

/**
 * Converts a buffer of raw character data into a list
 * of JSON tokens.
 * If an error occurrs, a reference to an integer is populated
 * with the error code.
 *
 * Params:
 *   jep_char_buffer* - a character buffer
 *   int* - a reference to an integer that will hold the error code.
 *
 * Returns:
 *   jep_json_token* - a list of JSON tokens
 */
static jep_json_token *tokenize_json(jep_string *raw, int *err);

/**
 * Parses a list of JSON tokens into a JSON object.
 *
 * Params:
 *   jep_json_token** - a reference to a list of JSON tokens
 *
 * Returns:
 *   jep_json_object - a JSON object
 */
static jep_json_object *parse_json(jep_json_token **tokens, int parse_type, jep_json_array **out_arr);

/**
 * Frees the memory allocated for a list of JSON tokens.
 *
 * Params:
 *   jep_json_token* - a reference to a list of JSON tokens
 */
static void destroy_json_token_list(jep_json_token *list);

/*-----------------------------------------------------------------*/
/*                 Token Function Implementations                  */
/*-----------------------------------------------------------------*/

static jep_json_token *create_json_token(int type)
{
    if (type < JSON_BEGIN || type > JSON_TOKEN_NULL)
    {
        return NULL;
    }

    jep_json_token *tok = (jep_json_token *)malloc(sizeof(jep_json_token));

    if (tok == NULL)
    {
        return NULL;
    }

    tok->type = type;
    tok->next = NULL;
    tok->tail = NULL;
    tok->str = NULL;

    return tok;
}

static void destroy_json_token(jep_json_token *tok)
{
    if (tok == NULL)
    {
        return;
    }

    if (tok->str != NULL)
    {
        jep_destroy_string(tok->str);
    }

    free(tok);
}

static void add_json_token(jep_json_token *list, jep_json_token *next)
{
    if (list == NULL || next == NULL)
    {
        return;
    }

    if (list->tail == NULL)
    {
        list->next = next;
        list->tail = next;

        next->prev = list;
        next->head = list;
        next->tail = next;

        return;
    }

    next->prev = list->tail;
    next->head = list;
    next->tail = next;

    list->tail->next = next;
    list->tail = next;
}

/*-----------------------------------------------------------------*/
/*             State Management Function Implementations           */
/*-----------------------------------------------------------------*/

static jep_json_state *create_json_state(int state)
{
    jep_json_state *st = (jep_json_state *)malloc(sizeof(jep_json_state));

    if (st == NULL)
    {
        return NULL;
    }

    st->state = state;
    st->prev = NULL;
    st->top = st;

    return st;
}

static void destroy_json_state(jep_json_state *state)
{
    if (state == NULL)
    {
        return;
    }

    free(state);
}

static void push_state(jep_json_state *stack, jep_json_state *state)
{
    if (stack == NULL || state == NULL)
    {
        return;
    }

    state->prev = stack->top;
    stack->top = state;
}

static jep_json_state *pop_state(jep_json_state *stack)
{
    if (stack == NULL)
    {
        return NULL;
    }

    if (stack->top->state == JSON_BEGIN)
    {
        return stack->top;
    }

    jep_json_state *old_top = stack->top;
    jep_json_state *current = stack->top;
    jep_json_state *new_top = current->prev;

    while (current != NULL)
    {
        current->top = new_top;
        current = current->prev;
    }

    return old_top;
}

static void destroy_json_state_stack(jep_json_state *stack)
{
    if (stack == NULL)
    {
        return;
    }

    jep_json_state *state = stack->top;
    jep_json_state *prev;

    while (state != NULL)
    {
        prev = state->prev;
        destroy_json_state(state);
        state = prev;
    }
}

/*-----------------------------------------------------------------*/
/*              Token Value Function Implementations               */
/*-----------------------------------------------------------------*/

static int is_whitespace(jep_char c)
{
    return c == 0x20            // ' '
                   || c == 0x09 // '\t'
                   || c == 0x0A // '\n'
                   || c == 0x0D // '\r'
               ? 1
               : 0;
}

static int is_escape(jep_char c)
{
    return c == 0x5C            // '\\'
                   || c == 0x74 // 't'
                   || c == 0x6E // 'n'
                   || c == 0x72 // 'r'
                   || c == 0x62 // 'b'
                   || c == 0x66 // 'f'
                   || c == 0x22 // '"'
               ? 1
               : 0;
}

static jep_char unescape(jep_char c)
{
    return c == 0x5C   ? 0x5C // '\\'
           : c == 0x74 ? 0x09 // 't'
           : c == 0x6E ? 0x0A // 'n'
           : c == 0x72 ? 0x0D // 'r'
           : c == 0x62 ? 0x07 // 'b'
           : c == 0x66 ? 0x0C // 'f'
           : c == 0x22 ? 0x22 // '"'
                       : 0;
}

static int is_number(jep_char c, int base)
{
    if (c > 0xFF)
    {
        return 0;
    }

    uint8_t lo = c & 0xFF;

    if (base == 10 || base == 16)
    {
        if (lo >= 0x30 && lo <= 0x39)
        {
            return 1;
        }
    }

    if (base == 16)
    {
        // 'A' .. 'F'
        if (lo >= 0x41 && lo <= 0x46)
            return 1;

        // 'a' .. 'f'
        if (lo >= 0x61 && lo <= 0x66)
            return 1;

        // 'x' or 'X'
        if (lo == 0x78 || lo == 0x58)
            return 1;
    }

    if (base == 10)
    {
        // check for .
        return lo == 0x2E ? 1 : 0;
    }

    return 0;
}

static int is_number_start(jep_char c)
{
    // if (c.b3 > 0x00 || c.b2 > 0x00)
    if (c > 0xFF)
    {
        return 0;
    }

    uint8_t lo = c & 0xFF;

    /* check for the range '0' .. '9' */
    if (lo >= 0x30 && lo <= 0x39)
    {
        return 1;
    }

    /* check for '-' */
    return lo == 0x2D ? 1 : 0;
}

static int is_boolean(jep_char c)
{
    if (c > 0xFF)
    {
        return 0;
    }

    uint8_t lo = c & 0xFF;

    switch (lo)
    {
    case 0x74: /* t */
    case 0x72: /* r */
    case 0x75: /* u */
    case 0x65: /* e */
    case 0x66: /* f */
    case 0x61: /* a */
    case 0x6C: /* l */
    case 0x73: /* s */
        return 1;
    default:
        return 0;
    }
}

static int is_boolean_start(jep_char c)
{
    // check for 't' or 'f'
    return (c == 0x74 || c == 0x66) ? 1 : 0;
}

static int is_null(jep_char c)
{
    // check for 'u' or 'l'
    return (c == 0x75 || c == 0x6C) ? 1 : 0;
}

static int is_null_start(jep_char c)
{
    // check for 'n'
    return c == 0x6E ? 1 : 0;
}

/*-----------------------------------------------------------------*/
/*             Object Creation Function Implementations            */
/*-----------------------------------------------------------------*/

static jep_json_object *create_json_object()
{
    jep_json_object *obj;

    obj = (jep_json_object *)malloc(sizeof(jep_json_object));

    if (obj == NULL)
    {
        return NULL;
    }

    obj->fields = NULL;
    obj->tail = NULL;

    return obj;
}

static jep_json_field *create_json_field()
{
    jep_json_field *field;

    field = (jep_json_field *)malloc(sizeof(jep_json_field));

    if (field == NULL)
    {
        return NULL;
    }

    field->name = NULL;
    field->value = NULL;
    field->next = NULL;

    return field;
}

static jep_json_array *create_json_array()
{
    jep_json_array *arr;

    arr = (jep_json_array *)malloc(sizeof(jep_json_array));

    if (arr == NULL)
    {
        return NULL;
    }

    arr->values = NULL;
    arr->tail = NULL;

    return arr;
}

static jep_json_value *create_json_value()
{
    jep_json_value *val;

    val = (jep_json_value *)malloc(sizeof(jep_json_value));

    if (val == NULL)
    {
        return NULL;
    }

    val->type = 0;
    val->next = NULL;

    return val;
}

static void destroy_json_field(jep_json_field *f)
{
    if (f == NULL)
    {
        return;
    }

    destroy_json_value(f->value);

    if (f->name != NULL)
    {
        jep_destroy_string(f->name);
    }

    free(f);
}

static void destroy_json_array(jep_json_array *a)
{
    if (a == NULL)
    {
        return;
    }

    jep_json_value *v = a->values;
    jep_json_value *next = NULL;

    while (v != NULL)
    {
        next = v->next;
        destroy_json_value(v);
        v = next;
    }

    free(a);
}

static void destroy_json_value(jep_json_value *v)
{
    if (v == NULL)
    {
        return;
    }

    if (v->type == JSON_VALUE_OBJECT)
    {
        jep_destroy_json_object(v->data.obj);
    }
    else if (v->type == JSON_VALUE_NUMBER ||
             v->type == JSON_VALUE_BOOLEAN ||
             v->type == JSON_VALUE_NULL ||
             v->type == JSON_VALUE_STRING)
    {

        jep_destroy_string(v->data.raw);
    }
    else if (v->type == JSON_VALUE_ARRAY)
    {
        destroy_json_array(v->data.arr);
    }

    free(v);
}

static void append_value(jep_json_array *arr, jep_json_value *val)
{
    if (arr == NULL)
    {
        return;
    }

    if (arr->values == NULL)
    {
        arr->values = val;
    }
    else
    {
        jep_json_value *v = arr->values;

        while (v->next != NULL)
        {
            v = v->next;
        }

        v->next = val;
    }

    arr->tail = val;
}

static void append_field(jep_json_object *obj, jep_json_field *field)
{
    if (obj == NULL)
    {
        return;
    }

    if (obj->fields == NULL)
    {
        obj->fields = field;
    }
    else
    {
        jep_json_field *f = obj->fields;

        while (f->next != NULL)
        {
            f = f->next;
        }

        f->next = field;
    }

    obj->tail = field;
}

/*-----------------------------------------------------------------*/
/*                    Tokenization Error Logic                     */
/*-----------------------------------------------------------------*/

static int is_begin_object_err(jep_json_state *state, jep_json_token *list)
{
    if (state->top->state != JSON_BEGIN &&
        state->top->state != JSON_BEGIN_ARRAY &&
        list->tail->type != JSON_TOKEN_COLON)
    {
        return JSON_TOK_ERR_BEGIN_OBJECT;
    }
    else if (state->top->state == JSON_BEGIN_ARRAY &&
             list->tail->type != JSON_TOKEN_LSQUARE &&
             list->tail->type != JSON_TOKEN_COLON &&
             list->tail->type != JSON_TOKEN_COMMA)
    {
        return JSON_TOK_ERR_BEGIN_OBJECT;
    }

    return 0;
}

static int is_end_object_err(jep_json_state *state, jep_json_token *list)
{
    if (state->top->state == JSON_BEGIN)
    {
        return JSON_TOK_ERR_END_OBJECT;
    }
    else if (state->top->state != JSON_BEGIN_OBJECT ||
             list->tail->type == JSON_TOKEN_COLON ||
             list->tail->type == JSON_TOKEN_COMMA)
    {
        return JSON_TOK_ERR_END_OBJECT;
    }

    return 0;
}

static int is_begin_array_error(jep_json_state *state, jep_json_token *list)
{
    if (state->top->state == JSON_BEGIN)
    {
        return JSON_TOK_ERR_BEGIN_ARRAY;
    }
    else if (state->top->state != JSON_BEGIN &&
             list->tail->type != JSON_TOKEN_COMMA &&
             list->tail->type != JSON_TOKEN_COLON)
    {
        return JSON_TOK_ERR_BEGIN_ARRAY;
    }

    return 0;
}

static int is_end_array_error(jep_json_state *state, jep_json_token *list)
{
    if (state->top->state == JSON_BEGIN)
    {
        return JSON_TOK_ERR_END_ARRAY;
    }
    else if (state->top->state != JSON_BEGIN_ARRAY ||
             list->tail->type == JSON_TOKEN_COLON ||
             list->tail->type == JSON_TOKEN_COMMA)
    {
        return JSON_TOK_ERR_END_ARRAY;
    }

    return 0;
}

static int is_begin_string_error(jep_json_state *state, jep_json_token *list)
{
    if (state->top->state == JSON_BEGIN)
    {
        return JSON_TOK_ERR_BEGIN_STRING;
    }
    else if (state->top->state == JSON_BEGIN_OBJECT)
    {
        if (list->tail->type != JSON_TOKEN_LBRACE &&
            list->tail->type != JSON_TOKEN_LSQUARE &&
            list->tail->type != JSON_TOKEN_COLON &&
            list->tail->type != JSON_TOKEN_COMMA)
        {
            return JSON_TOK_ERR_BEGIN_STRING;
        }
    }
    else if (state->top->state == JSON_BEGIN_ARRAY)
    {
        if (list->tail->type != JSON_TOKEN_COMMA && list->tail->type != JSON_TOKEN_LSQUARE)
        {
            return JSON_TOK_ERR_BEGIN_STRING;
        }
    }

    return 0;
}

static int is_colon_error(jep_json_state *state, jep_json_token *list)
{
    if (state->top->state == JSON_BEGIN)
    {
        return JSON_TOK_ERR_COLON;
    }
    else if (list->tail->type != JSON_TOKEN_STRING)
    {
        return JSON_TOK_ERR_COLON;
    }

    return 0;
}

static int is_comma_error(jep_json_state *state, jep_json_token *list)
{
    if (state->top->state == JSON_BEGIN)
    {
        return JSON_TOK_ERR_COMMA;
    }
    else if (list->tail->type != JSON_TOKEN_RBRACE &&
             list->tail->type != JSON_TOKEN_RSQUARE &&
             list->tail->type != JSON_TOKEN_STRING &&
             list->tail->type != JSON_TOKEN_NUMBER &&
             list->tail->type != JSON_TOKEN_BOOLEAN &&
             list->tail->type != JSON_TOKEN_NULL)
    {
        return JSON_TOK_ERR_COMMA;
    }

    return 0;
}

static int is_number_error(jep_json_state *state, jep_json_token *list)
{
    if (state->top->state == JSON_BEGIN)
    {
        return JSON_TOK_ERR_BEGIN_NUMBER;
    }
    else if (state->top->state == JSON_BEGIN_OBJECT)
    {
        if (list->tail->type != JSON_TOKEN_LBRACE && (list)->tail->type != JSON_TOKEN_COLON)
        {
            return JSON_TOK_ERR_BEGIN_NUMBER;
        }
    }
    else if (state->top->state == JSON_BEGIN_ARRAY)
    {
        if (list->tail->type != JSON_TOKEN_LSQUARE && list->tail->type != JSON_TOKEN_COMMA)
        {
            return JSON_TOK_ERR_BEGIN_NUMBER;
        }
    }

    return 0;
}

static int is_boolean_error(jep_json_state *state, jep_json_token *list)
{
    if (state->top->state == JSON_BEGIN)
    {
        return JSON_TOK_ERR_BEGIN_BOOLEAN;
    }
    else if (state->top->state == JSON_BEGIN_OBJECT)
    {
        if (list->tail->type != JSON_TOKEN_LBRACE && list->tail->type != JSON_TOKEN_COLON)
        {
            return JSON_TOK_ERR_BEGIN_BOOLEAN;
        }
    }
    else if (state->top->state == JSON_BEGIN_ARRAY)
    {
        if (list->tail->type != JSON_TOKEN_LSQUARE && list->tail->type != JSON_TOKEN_COMMA)
        {
            return JSON_TOK_ERR_BEGIN_BOOLEAN;
        }
    }

    return 0;
}

static int is_null_error(jep_json_state *state, jep_json_token *list)
{
    if (state->top->state == JSON_BEGIN)
    {
        return JSON_TOK_ERR_BEGIN_NULL;
    }
    else if (state->top->state == JSON_BEGIN_OBJECT)
    {
        if (list->tail->type != JSON_TOKEN_LBRACE && list->tail->type != JSON_TOKEN_COLON)
        {
            return JSON_TOK_ERR_BEGIN_NULL;
        }
    }
    else if (state->top->state == JSON_BEGIN_ARRAY)
    {
        if (list->tail->type != JSON_TOKEN_LSQUARE && list->tail->type != JSON_TOKEN_COMMA)
        {
            return JSON_TOK_ERR_BEGIN_NULL;
        }
    }

    return 0;
}

/*-----------------------------------------------------------------*/
/*                 Parsing Function Implementations                */
/*-----------------------------------------------------------------*/

static jep_json_token *tokenize_json(jep_string *raw, int *err)
{
    jep_json_token *list = create_json_token(JSON_BEGIN);

    size_t i = 0;

    if (list == NULL || raw == NULL)
    {
        return NULL;
    }

    jep_json_state *state = create_json_state(JSON_BEGIN);

    if (state == NULL)
    {
        destroy_json_token_list(list);
        return NULL;
    }

    int error = 0;
    int enc = JEP_ENCODING_UTF_8;
    jep_byte lbrc = JEP_CHAR_LBRC;
    jep_byte rbrc = JEP_CHAR_RBRC;
    jep_byte lsqr = JEP_CHAR_LSQR;
    jep_byte rsqr = JEP_CHAR_RSQR;
    jep_byte comma = JEP_CHAR_COMMA;
    jep_byte colon = JEP_CHAR_COLON;

    for (i = 0; i < raw->size && !error; i++)
    {
        // left Curly Brace '{'
        if (!jep_char_cmp(raw->chars[i], JEP_CHAR_LBRC))
        {
            error = is_begin_object_err(state, list);

            if (!error)
            {
                jep_json_state *s;
                jep_json_token *tok;

                s = create_json_state(JSON_BEGIN_OBJECT);
                push_state(state, s);

                tok = create_json_token(JSON_TOKEN_LBRACE);
                tok->str = jep_bytes_to_string(&lbrc, enc, 1);

                add_json_token(list, tok);
            }
        }

        // right Curly Brace '}'
        else if (!jep_char_cmp(raw->chars[i], JEP_CHAR_RBRC))
        {
            error = is_end_object_err(state, list);

            if (!error)
            {
                jep_json_state *s;
                jep_json_token *tok;

                s = pop_state(state);
                destroy_json_state(s);

                tok = create_json_token(JSON_TOKEN_RBRACE);
                tok->str = jep_bytes_to_string(&rbrc, enc, 1);

                add_json_token(list, tok);
            }
        }

        // left square bracket '['
        else if (!jep_char_cmp(raw->chars[i], JEP_CHAR_LSQR))
        {
            error = is_begin_array_error(state, list);

            if (!error)
            {
                jep_json_state *s;
                jep_json_token *tok;

                s = create_json_state(JSON_BEGIN_ARRAY);
                push_state(state, s);

                tok = create_json_token(JSON_TOKEN_LSQUARE);
                tok->str = jep_bytes_to_string(&lsqr, enc, 1);

                add_json_token(list, tok);
            }
        }

        // right square bracket ']'
        else if (!jep_char_cmp(raw->chars[i], JEP_CHAR_RSQR))
        {
            error = is_end_array_error(state, list);

            if (!error)
            {
                jep_json_state *s;
                jep_json_token *tok;

                s = pop_state(state);
                destroy_json_state(s);

                tok = create_json_token(JSON_TOKEN_RSQUARE);
                tok->str = jep_bytes_to_string(&rsqr, enc, 1);

                add_json_token(list, tok);
            }
        }

        // colon ':'
        else if (!jep_char_cmp(raw->chars[i], JEP_CHAR_COLON))
        {
            error = is_colon_error(state, list);

            if (!error)
            {
                jep_json_token *tok;

                tok = create_json_token(JSON_TOKEN_COLON);
                tok->str = jep_bytes_to_string(&colon, enc, 1);

                add_json_token(list, tok);
            }
        }

        // comma ','
        else if (!jep_char_cmp(raw->chars[i], JEP_CHAR_COMMA))
        {
            error = is_comma_error(state, list);

            if (!error)
            {
                jep_json_token *tok;

                tok = create_json_token(JSON_TOKEN_COMMA);
                tok->str = jep_bytes_to_string(&comma, enc, 1);

                add_json_token(list, tok);
            }
        }

        // string
        else if (!jep_char_cmp(raw->chars[i], JEP_CHAR_QUOTE))
        {
            error = is_begin_string_error(state, list);

            if (!error)
            {
                jep_json_state *s;
                jep_json_token *stok;
                jep_char_buffer *cb;

                s = create_json_state(JSON_BEGIN_STRING);
                push_state(state, s);

                stok = create_json_token(JSON_TOKEN_STRING);
                cb = jep_create_char_buffer();

                while (!error && i < raw->size - 1 && state->top->state == JSON_BEGIN_STRING)
                {
                    if (!jep_char_cmp(raw->chars[++i], JEP_CHAR_QUOTE))
                    {
                        jep_json_state *s = pop_state(state);
                        destroy_json_state(s);
                    }
                    else if (!jep_char_cmp(raw->chars[i], JEP_CHAR_BSLASH))
                    {
                        if (is_escape(jep_char_at(raw, i + 1)))
                        {
                            jep_append_char(cb, unescape(raw->chars[++i]));
                        }
                        else
                        {
                            error = JSON_TOK_ERR_ESCAPE;
                        }
                    }
                    else
                    {
                        jep_append_char(cb, raw->chars[i]);
                    }

                    if (i == raw->size - 1 && state->top->state == JSON_BEGIN_STRING)
                    {
                        error = JSON_TOK_ERR_OPEN_STRING;
                    }
                }

                stok->str = jep_chars_to_string(cb->buffer, cb->size);
                jep_destroy_char_buffer(cb);

                add_json_token(list, stok);
            }
        }

        // number, boolean, null
        else
        {
            if (is_number_start(raw->chars[i]))
            {
                error = is_number_error(state, list);

                if (!error)
                {
                    jep_json_state *s;
                    jep_json_token *ntok;
                    jep_char_buffer *cb;

                    s = create_json_state(JSON_BEGIN_NUMBER);
                    push_state(state, s);

                    ntok = create_json_token(JSON_TOKEN_NUMBER);

                    cb = jep_create_char_buffer();

                    jep_append_char(cb, raw->chars[i]);

                    int hex = 0;
                    int dec = 0;
                    int base = 10;

                    if (i < raw->size - 2 && !jep_char_cmp(raw->chars[i], JEP_CHAR_0) && (!jep_char_cmp(raw->chars[i + 1], JEP_CHAR_x) || !jep_char_cmp(raw->chars[i + 1], JEP_CHAR_X)))
                    {
                        base = 16;
                    }

                    while (!error && i < raw->size - 1 && state->top->state == JSON_BEGIN_NUMBER)
                    {
                        if (is_number(raw->chars[++i], base))
                        {
                            if (!jep_char_cmp(raw->chars[i], JEP_CHAR_PERIOD))
                            {
                                if (!dec)
                                {
                                    dec = 1;
                                }
                                else
                                {
                                    error = JSON_TOK_ERR_DECIMAL;
                                }
                            }

                            if ((!jep_char_cmp(raw->chars[i], JEP_CHAR_x) || !jep_char_cmp(raw->chars[i], JEP_CHAR_X)))
                            {
                                if (!hex)
                                {
                                    hex = 1;
                                }
                                else
                                {
                                    error = JSON_TOK_ERR_HEX_X;
                                }
                            }

                            jep_append_char(cb, raw->chars[i]);
                        }
                        else
                        {
                            jep_json_state *s = pop_state(state);
                            destroy_json_state(s);
                        }

                        if (i == raw->size - 1 && state->top->state == JSON_BEGIN_NUMBER)
                        {
                            error = JSON_TOK_ERR_END_NUMBER;
                        }
                    }

                    ntok->str = jep_chars_to_string(cb->buffer, cb->size);
                    jep_destroy_char_buffer(cb);

                    add_json_token(list, ntok);

                    i--;
                }
            }

            else if (is_boolean_start(raw->chars[i]))
            {
                error = is_boolean_error(state, list);

                if (!error)
                {
                    jep_json_state *s;
                    jep_json_token *btok;
                    jep_char_buffer *cb;

                    s = create_json_state(JSON_BEGIN_BOOLEAN);
                    push_state(state, s);

                    btok = create_json_token(JSON_TOKEN_BOOLEAN);

                    cb = jep_create_char_buffer();

                    jep_append_char(cb, raw->chars[i]);

                    while (!error && i < raw->size - 1 && state->top->state == JSON_BEGIN_BOOLEAN)
                    {
                        if (is_boolean(raw->chars[++i]))
                        {
                            jep_append_char(cb, raw->chars[i]);
                        }
                        else
                        {
                            jep_json_state *s = pop_state(state);
                            destroy_json_state(s);
                        }

                        if (i == raw->size - 1 && state->top->state == JSON_BEGIN_BOOLEAN)
                        {
                            error = JSON_TOK_ERR_END_BOOLEAN;
                        }
                    }

                    btok->str = jep_chars_to_string(cb->buffer, cb->size);
                    jep_destroy_char_buffer(cb);

                    add_json_token(list, btok);

                    i--;
                }
            }

            else if (is_null_start(raw->chars[i]))
            {
                error = is_null_error(state, list);

                if (!error)
                {
                    jep_json_state *s;
                    jep_json_token *ntok;
                    jep_char_buffer *cb;

                    s = create_json_state(JSON_BEGIN_NULL);
                    push_state(state, s);

                    ntok = create_json_token(JSON_TOKEN_NULL);

                    cb = jep_create_char_buffer();

                    jep_append_char(cb, raw->chars[i]);

                    while (!error && i < raw->size - 1 && state->top->state == JSON_BEGIN_NULL)
                    {
                        if (is_null(raw->chars[++i]))
                        {
                            jep_append_char(cb, raw->chars[i]);
                        }
                        else
                        {
                            jep_json_state *s = pop_state(state);
                            destroy_json_state(s);
                        }

                        if (i == raw->size - 1 && state->top->state == JSON_BEGIN_NULL)
                        {
                            error = JSON_TOK_ERR_END_NULL;
                        }
                    }

                    ntok->str = jep_chars_to_string(cb->buffer, cb->size);
                    jep_destroy_char_buffer(cb);

                    add_json_token(list, ntok);

                    i--;
                }
            }

            else if (!is_whitespace(raw->chars[i]))
            {
                error = JSON_TOK_ERR_UNEXPECTED;
            }
        }
    }

    destroy_json_state(state);

    if (error)
    {
        *err = error;
        destroy_json_token_list(list);
        list = NULL;
    }

    return list;
}

#define JSON_PARSE_OBJECT 1
#define JSON_PARSE_ARRAY 2

static jep_json_object *parse_json(jep_json_token **tokens, int parse_type, jep_json_array **out_arr)
{
    jep_json_object *root = NULL;
    jep_json_object *cur_obj = NULL;
    jep_json_field *cur_field = NULL;
    jep_json_field arr_field;
    jep_json_value arr_value;

    if (parse_type == JSON_PARSE_ARRAY)
    {
        cur_field = &arr_field;
        arr_value.data.arr = create_json_array();
        cur_field->value = &arr_value;
    }
    else
    {
        root = create_json_object();

        if (root == NULL)
        {
            return NULL;
        }

        cur_obj = root;
    }

    jep_json_token *t = *tokens;
    jep_json_token *end = NULL;

    jep_json_state *state = create_json_state(JSON_BEGIN);

    while (t != NULL && end == NULL)
    {
        /* objects */
        if (t->type == JSON_TOKEN_LBRACE)
        {
            if (state->top->state == JSON_BEGIN)
            {
                jep_json_state *s = create_json_state(JSON_BEGIN_OBJECT);
                push_state(state, s);
            }
            else if (state->top->state == JSON_BEGIN_FIELD && cur_field != NULL)
            {
                jep_json_object *o = parse_json(&t, JSON_PARSE_OBJECT, NULL);

                cur_field->value->type = JSON_VALUE_OBJECT;
                cur_field->value->data.obj = o;

                append_field(cur_obj, cur_field);

                jep_json_state *s = pop_state(state);
                destroy_json_state(s);
            }
            else if (state->top->state == JSON_BEGIN_ARRAY && cur_field != NULL)
            {
                jep_json_object *o = parse_json(&t, JSON_PARSE_OBJECT, NULL);

                jep_json_value *v = create_json_value();
                v->type = JSON_VALUE_OBJECT;
                v->data.obj = o;

                append_value(cur_field->value->data.arr, v);
            }
        }
        else if (t->type == JSON_TOKEN_RBRACE)
        {
            if (state->top->state == JSON_BEGIN_OBJECT)
            {
                jep_json_state *s = pop_state(state);
                destroy_json_state(s);
            }

            end = t;
        }

        /* arrays */
        else if (t->type == JSON_TOKEN_LSQUARE)
        {
            if (state->top->state == JSON_BEGIN)
            {
                jep_json_state *s = create_json_state(JSON_BEGIN_ARRAY);
                push_state(state, s);
            }
            else if (state->top->state == JSON_BEGIN_FIELD && cur_field != NULL)
            {
                jep_json_array *a;
                parse_json(&t, JSON_PARSE_ARRAY, &a);

                cur_field->value->type = JSON_VALUE_ARRAY;
                cur_field->value->data.arr = a;

                append_field(cur_obj, cur_field);

                jep_json_state *s = pop_state(state);
                destroy_json_state(s);
            }
            else if (state->top->state == JSON_BEGIN_ARRAY && cur_field != NULL)
            {
                jep_json_array *a;
                parse_json(&t, JSON_PARSE_ARRAY, &a);

                jep_json_value *v = create_json_value();
                v->type = JSON_VALUE_ARRAY;
                v->data.arr = a;

                append_value(cur_field->value->data.arr, v);
            }
        }
        else if (t->type == JSON_TOKEN_RSQUARE)
        {
            if (state->top->state == JSON_BEGIN_ARRAY)
            {
                jep_json_state *s = pop_state(state);
                destroy_json_state(s);
            }

            end = t;
        }

        /* values */
        else if (t->type == JSON_TOKEN_STRING)
        {
            if (state->top->state == JSON_BEGIN_OBJECT)
            {
                jep_json_state *s;
                jep_json_field *f;
                jep_json_value *v;
                jep_string *raw;

                s = create_json_state(JSON_BEGIN_FIELD);
                push_state(state, s);

                f = create_json_field();

                v = create_json_value();

                raw = jep_create_string(0);
                jep_strcpy(t->str, raw);

                f->name = raw;
                f->value = v;

                cur_field = f;
            }
            else if (state->top->state == JSON_BEGIN_ARRAY && cur_field != NULL)
            {
                jep_json_value *v;
                jep_string *raw;

                raw = jep_create_string(0);
                jep_strcpy(t->str, raw);

                v = create_json_value();
                v->type = JSON_VALUE_STRING;
                v->data.raw = raw;

                append_value(cur_field->value->data.arr, v);
            }
            else if (state->top->state == JSON_BEGIN_FIELD && cur_field != NULL)
            {
                jep_string *raw;
                jep_json_state *s;

                raw = jep_create_string(0);
                jep_strcpy(t->str, raw);

                cur_field->value->type = JSON_VALUE_STRING;
                cur_field->value->data.raw = raw;

                append_field(cur_obj, cur_field);

                s = pop_state(state);
                destroy_json_state(s);
            }
        }
        else if (t->type == JSON_TOKEN_NUMBER || t->type == JSON_TOKEN_BOOLEAN || t->type == JSON_TOKEN_NULL)
        {
            if (state->top->state == JSON_BEGIN_FIELD && cur_field != NULL)
            {
                if (t->type == JSON_TOKEN_NUMBER)
                {
                    cur_field->value->type = JSON_VALUE_NUMBER;
                }
                else if (t->type == JSON_TOKEN_BOOLEAN)
                {
                    cur_field->value->type = JSON_VALUE_BOOLEAN;
                }
                else if (t->type == JSON_TOKEN_NULL)
                {
                    cur_field->value->type = JSON_VALUE_NULL;
                }

                jep_string *raw;
                jep_json_state *s;

                raw = jep_create_string(0);

                jep_strcpy(t->str, raw);

                cur_field->value->data.raw = raw;

                append_field(cur_obj, cur_field);

                cur_field = NULL;

                s = pop_state(state);
                destroy_json_state(s);
            }
            else if (state->top->state == JSON_BEGIN_ARRAY && cur_field != NULL)
            {
                jep_json_value *v;
                jep_string *raw;

                v = create_json_value();

                if (t->type == JSON_TOKEN_NUMBER)
                    v->type = JSON_VALUE_NUMBER;
                else if (t->type == JSON_TOKEN_BOOLEAN)
                    v->type = JSON_VALUE_BOOLEAN;
                else if (t->type == JSON_TOKEN_NULL)
                    v->type = JSON_VALUE_NULL;

                raw = jep_create_string(0);

                jep_strcpy(t->str, raw);

                v->data.raw = raw;

                append_value(cur_field->value->data.arr, v);
            }
        }

        t = t->next;
    }

    destroy_json_state_stack(state);

    *tokens = end;

    if (parse_type == JSON_PARSE_ARRAY && out_arr != NULL)
    {
        *out_arr = cur_field->value->data.arr;
    }

    return root;
}

static void destroy_json_token_list(jep_json_token *list)
{
    if (list == NULL)
    {
        return;
    }

    jep_json_token *tok = list;
    jep_json_token *next;

    while (tok != NULL)
    {
        next = tok->next;
        destroy_json_token(tok);
        tok = next;
    }
}

/*-----------------------------------------------------------------*/
/*                    json.h Implementation                        */
/*-----------------------------------------------------------------*/

JEP_UTILS_API int JEP_UTILS_CALL
jep_parse_json_string(jep_string *raw, jep_json_object **obj)
{
    if (raw == NULL)
    {
        return JSON_PARSE_ERR_NULL;
    }

    int err = 0;

    jep_json_token *tokens = tokenize_json(raw, &err);

    if (tokens == NULL)
    {
        return err;
    }

    jep_json_object *o = parse_json(&tokens, JSON_PARSE_OBJECT, NULL);

    destroy_json_token_list(tokens);

    *obj = o;

    return 0;
}

JEP_UTILS_API void JEP_UTILS_CALL
jep_destroy_json_object(jep_json_object *o)
{
    if (o == NULL)
    {
        return;
    }

    jep_json_field *f = o->fields;
    jep_json_field *next = NULL;

    while (f != NULL)
    {
        next = f->next;
        destroy_json_field(f);
        f = next;
    }

    free(o);
}

JEP_UTILS_API jep_json_field *JEP_UTILS_CALL
jep_get_json_field(jep_json_object *obj, jep_string *name)
{
    if (obj == NULL || name == NULL)
        return NULL;

    jep_json_field *f = obj->fields;

    while (f != NULL)
    {
        if (!jep_strcmp(name, f->name))
        {
            return f;
        }

        f = f->next;
    }

    return NULL;
}
