
#include <assert.h>     /* assert() */
#include <stdlib.h>     /* NULL, strtod() */

#include "leptjson.h"

typedef struct _lept_context {
    const char* json;
} lept_context;

static void lept_parse_whitespace(lept_context* c);

static LEPT_PARSE_STATUS lept_parse_value(
       lept_context* c, lept_value* v);

static LEPT_PARSE_STATUS lept_parse_literal(
       lept_context* c, lept_value* v,
       const char* literal, lept_type type);

static LEPT_PARSE_STATUS lept_parse_number(
       lept_context* c, lept_value* v);


/* ------------------------------ */
/* API functions definition start */
/* ------------------------------ */

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
extern lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}

double lept_get_number(const lept_value* v) {
    assert((v != NULL) && (v->type == LEPT_NUMBER));
    return v->n;
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
        return lept_parse_literal(c, v, "true", LEPT_TRUE);
    case 'f':
        return lept_parse_literal(c, v, "false", LEPT_FALSE);
    case '1': case '2': case '3': case '4': case '5':
    case '6': case '7': case '8': case '9': 
    case '0':
    case '-':
        return lept_parse_number(c, v);
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
