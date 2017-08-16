#ifndef _LEPTJSON_H_
#define _LEPTJSON_H_

#include <stdlib.h>

typedef enum _lept_type {    /* JSON types */
    LEPT_NULL = 0,
    LEPT_BOOLEAN,
#if 0
    LEPT_TRUE,
    LEPT_FALSE,
#endif
    LEPT_NUMBER,
    LEPT_STRING,
    LEPT_ARRAY,
    LEPT_OBJECT
} lept_type;

#define N 1024
typedef struct _s_json{
    char   json_str[N];           /* JSON String */
    size_t json_len;              /* String Length */ 
} s_json;
#undef N

typedef struct _lept_value { /* JSON node */
    s_json s;                /* String struct */
    double n;                /* C double for JSON Number */
    int b;                   /* C int for JSON Boolean */
    lept_type type;
} lept_value;

#define LEPT_PARSE_STATUS               int
#define LEPT_PARSE_OK                   0
#define LEPT_PARSE_EXPECT_VALUE         1
#define LEPT_PARSE_INVALID_VALUE        2
#define LEPT_PARSE_ROOT_NOT_SINGULAR    3

#if 0
enum _LEPT_PARSE_STATUS {   /* JSON parser return status */
    LEPT_PARSE_OK = 0,
    LEPT_PARSE_EXPECT_VALUE,
    LEPT_PARSE_INVALID_VALUE,
    LEPT_PARSE_ROOT_NOT_SINGULAR,
};
#endif

/* lept parse function */
extern int lept_parse(lept_value* v, const char* json);

/* lept get type function */
extern lept_type lept_get_type(const lept_value* v);

/* lept get boolean function */
extern int lept_get_boolean(const lept_value* v);

/* lept get number function */
extern double lept_get_number(const lept_value* v);

/* lept get string function */
extern const char* lept_get_string(const lept_value* v);
extern size_t lept_get_string_length(const lept_value* v);

#endif  /* _LEPTJSON_H_ */

