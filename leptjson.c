#include <assert.h>     /* assert() */
#include <stdlib.h>     /* NULL, strtod() */
#include <string.h>     /* strcmp() */

#include "leptjson.h"
#include "jsonstack.h"  /* stack tools */

typedef struct _lept_context {  /* conctext for JSON str */
    const char* json;
} lept_context;

/* --------------------------------------- */
/* static parse functions declaraton begin */
/* --------------------------------------- */

static void lept_parse_whitespace(lept_context* c);

static LEPT_PARSE_STATUS lept_parse_value(
       lept_context* c, lept_value* v);

static LEPT_PARSE_STATUS lept_parse_literal(
       lept_context* c, lept_value* v,
       const char* literal, lept_type type);

static LEPT_PARSE_STATUS lept_parse_number(
       lept_context* c, lept_value* v);

static LEPT_PARSE_STATUS lept_parse_string(
       lept_context* c, lept_value* v);

static LEPT_PARSE_STATUS lept_parse_array(
       lept_context* c, lept_value* v);

/* ------------------------------------- */
/* static parse functions declaraton end */
/* ------------------------------------- */

/* ------------------------------ */
/* API functions definition begin */
/* ------------------------------ */

/* main lept_parse function */
extern int lept_parse(lept_value* v, const char* json) {
    assert(v != NULL);

    lept_context c;
    c.json = json;
    v->type = LEPT_NULL;
    LEPT_PARSE_STATUS ret;

    lept_parse_whitespace(&c);
    if((ret = lept_parse_value(&c, v)) == LEPT_PARSE_OK){
        lept_parse_whitespace(&c);
        if(*(c.json) != '\0'){
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    return ret;
}

/* lept_get_type function */
extern lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}

/* lept_get_boolean function */
extern int lept_get_boolean(const lept_value* v) {
    assert((v != NULL) && v->type == LEPT_BOOLEAN);
    return v->b;
}

/* lept_get_number function */
extern double lept_get_number(const lept_value* v) {
    assert((v != NULL) && (v->type == LEPT_NUMBER));
    return v->n;
}

/* lept_get_string function */
extern const char* lept_get_string(const lept_value* v){
    assert((v != NULL) && (v->type == LEPT_STRING));
    return (v->s).json_str;
}

/* lept_get_string_length function */
extern size_t lept_get_string_length(const lept_value* v){
    assert((v != NULL) && (v->type == LEPT_STRING));
    return (v->s).json_len;
}

/* lept_get_array_size function */
extern size_t lept_get_array_size(const lept_value* v){
    assert((v != NULL) && (v->type == LEPT_ARRAY));
    return (v->a).size;
}

/* lept_get_array_element function */
extern lept_value* lept_get_array_element(const lept_value* v, size_t index){
    assert((v != NULL) && (v->type == LEPT_ARRAY));
    assert(index < (v->a).size);
    return (v->a).e + index;
}

/* lept_get_object_size function */
extern size_t lept_get_object_size(const lept_value* v) {
    assert((v != NULL) && (v->type == LEPT_OBJECT));
    return (v->o).size;
}

/* lept_get_object_key function */
extern lept_value* lept_get_object_key(const lept_value* v, size_t index) {
    assert((v != NULL) && (v->type == LEPT_OBJECT));
    assert(index < (v->o).size);
    return ((v->o).m)->m_s;
}

/* lept_get_object_key_length function */
extern size_t lept_get_object_key_length(const lept_value* v, size_t index) {
    assert((v != NULL) && (v->type == LEPT_OBJECT));
    assert(index < (v->o).size);
    return ((((v->o).m)->m_s)->s).json_len;
}

/* lept_get_object_value function */
extern lept_value* lept_get_object_value(const lept_value* v, size_t index) {
    assert((v != NULL) && (v->type == LEPT_OBJECT));
    assert(index < (v->o).size);
    return ((v->o).m)->m_v;
}

/* ---------------------------- */
/* API functions definition end */
/* ---------------------------- */

static void lept_parse_whitespace(lept_context* c) {
    const char *p = c->json;
    while(*p == ' ' || *p == '\n' || *p == '\r' || *p == '\t'){
        p++;
    }
    c->json = p;
}

static LEPT_PARSE_STATUS lept_parse_value(
       lept_context* c, lept_value* v) 
{
    switch (*(c->json)){
    case 'n':
        return lept_parse_literal(c, v, "null", LEPT_NULL);
    case 't':
        return lept_parse_literal(c, v, "true", LEPT_BOOLEAN);
    case 'f':
        return lept_parse_literal(c, v, "false", LEPT_BOOLEAN);
    case '1': case '2': case '3': case '4': case '5':
    case '6': case '7': case '8': case '9': 
    case '0':
    case '-':
        return lept_parse_number(c, v);
    case '\"':
        return lept_parse_string(c, v);
    case '[':
        return lept_parse_array(c, v);
    case '\0':
        return LEPT_PARSE_EXPECT_VALUE;
    default: 
        return LEPT_PARSE_INVALID_VALUE;
    }
}

static LEPT_PARSE_STATUS lept_parse_literal(
       lept_context* c, lept_value* v,
       const char* literal, lept_type type)
{
    size_t i;
    for(i = 0; literal[i]; i++){
        if((c->json)[i] != literal[i]){
            return LEPT_PARSE_INVALID_VALUE;
        }
    }
    c->json += i;
    v->type = type;
    if(v->type == LEPT_BOOLEAN){    /* for JSON boolean type */
        if(strcmp(literal, "true") == 0){
            v->b = 1;
        }
        if(strcmp(literal, "false") == 0){
            v->b = 0;
        }
    }
    return LEPT_PARSE_OK;
}

#define ISDIGIT(x)      \
    ((x) >= '0' && (x) <= '9')

#define ISDIGIT1TO9(x)  \
    ((x) >= '1' && (x) <= '9')

static LEPT_PARSE_STATUS lept_parse_number(
       lept_context* c, lept_value* v)
{
    const char *p = c->json;
    if(*p == '-'){ p++; }
    if(*p == '0'){ p++; }
    else{
        if (!ISDIGIT1TO9(*p)){
            return LEPT_PARSE_INVALID_VALUE;
        }
        for(p++; ISDIGIT(*p); p++);
    }

    if(*p == '.'){
        p++;
        if (!ISDIGIT(*p)){
            return LEPT_PARSE_INVALID_VALUE;
        }
        for(p++; ISDIGIT(*p); p++);
    }

    if(*p == 'e' || *p == 'E'){
        p++;
        if(*p == '+' || *p == '-'){ p++; }
        if(!ISDIGIT(*p)){
            return LEPT_PARSE_INVALID_VALUE;
        }
        for (p++; ISDIGIT(*p); p++);
    }
    v->n = strtod(c->json, NULL);
    v->type = LEPT_NUMBER;
    c->json = p;
    return LEPT_PARSE_OK;
}
#undef ISDIGIT
#undef ISDIGIT1TO9

static LEPT_PARSE_STATUS lept_parse_string(
       lept_context* c, lept_value* v)
{
    if(*(c->json) != '\"'){
        return LEPT_PARSE_INVALID_VALUE;
    }

    const char *p = c->json;
    p++;
    size_t len = 0;
    char ch;
    int flag = 0;
    while(1){
        ch = *(p++);
        switch (ch) {
        case '\"':
            flag = 1;
            break;
        case '\0':
            return LEPT_PARSE_INVALID_VALUE;
        case '\\':
            switch(*p++){
            case '\\': 
                ((v->s).json_str)[len] = '\\';
                break;
            case '\"':
                ((v->s).json_str)[len] = '\"';
                break;
            case '/':
                ((v->s).json_str)[len] = '/';
                break;
            case 'b':
                ((v->s).json_str)[len] = '\b';
                break;
            case 'f':
                ((v->s).json_str)[len] = '\f';
                break;
            case 'n':
                ((v->s).json_str)[len] = '\n';
                break;
            case 'r':
                ((v->s).json_str)[len] = '\r';
                break;
            case 't':
                ((v->s).json_str)[len] = '\t';
                break;
            default:
                return LEPT_PARSE_INVALID_VALUE;
            }
            break;
        default:
            ((v->s).json_str)[len] = ch;
            break;
    }
        if(flag == 1){
            break;
        }else{
            len++;
        }
    }

    (v->s).json_len = len;
    ((v->s).json_str)[len] = '\0';
    v->type = LEPT_STRING;
    c->json = p;
    return LEPT_PARSE_OK;
}

static LEPT_PARSE_STATUS lept_parse_array(
       lept_context* c, lept_value* v)
{
    if(*(c->json) != '[') {
        return LEPT_PARSE_INVALID_VALUE;
    } else {
        c->json++;
        lept_parse_whitespace(c);
    }

    if(*(c->json) == ']') {
        c->json++;
        v->type = LEPT_ARRAY;
        (v->a).size = 0;
        (v->a).e = NULL;
        return LEPT_PARSE_OK;
    }

    int    ret;
    size_t size = 0;
    st s;
    init_stack(&s);

    while(1){
        lept_value *e = (lept_value*)malloc(sizeof(lept_value) * 1);
        if((ret = lept_parse_value(c, e)) != LEPT_PARSE_OK) {
            return ret;
        } else {
            /* (v->a).e = e; */
            push_stack(&s, e, sizeof(lept_value));
            size++;
            lept_parse_whitespace(c);
        }
  		if (*c->json == ',') {
            c->json++;
            lept_parse_whitespace(c);
        }
  		else if (*c->json == ']') {
            c->json++;
            v->type = LEPT_ARRAY;
            v->a.size = size;
            (v->a).e = my_memcpy(malloc(sizeof(lept_value) * size), 
                                 pop_stack(&s, s.top),
                                 s.top);
            del_stack(&s);
            free(e);
            return LEPT_PARSE_OK;
        }
        else{
            return LEPT_PARSE_INVALID_VALUE;
        }
    }
}

