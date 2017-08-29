#ifndef _LEPTJSON_H_
#define _LEPTJSON_H_

#include <stdlib.h>

typedef enum _lept_type {   /* JSON types */
    LEPT_NULL = 0,
    LEPT_BOOLEAN,
    LEPT_NUMBER,
    LEPT_STRING,
    LEPT_ARRAY,
    LEPT_OBJECT
} lept_type;

/* lept_value forward declare */
typedef struct _lept_value  lept_value;

/* lept_member forward declare */
typedef struct _lept_member lept_member;

#define N 1024
typedef struct _json_string {
    char   json_str[N];     /* JSON String Value */
    size_t json_len;        /* JSON String Length */ 
} s_json;
#undef N

typedef struct _json_array {
    lept_value *e;          /* JSON Array Element */
    size_t      size;       /* JSON Array Length */
} a_json;

typedef struct _json_object {
    lept_member  *m;        /* JSON Object Key-Value */
    size_t     size;        /* JSON Object Size */
} o_json;

struct _lept_value {         /* JSON Node */
    o_json o;                /* JSON Object (object struct type) */
    a_json a;                /* JSON Array (array struct type) */
    s_json s;                /* JSON String (string struct type) */
    double n;                /* JSON Number (C double) */
    int    b;                /* JSON Boolean (C int) */
    /* null */               /* JSON NULL (Type NULL) */
    lept_type type;          /* JSON Type */
};

struct _lept_member {
    lept_value m_s;         /* JSON Object Key String */
    lept_value m_v;         /* JSON Object Key Value */
};

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
extern size_t      lept_get_string_length(const lept_value* v);

/* lept get array function */
extern size_t      lept_get_array_size(const lept_value* v);
extern lept_value* lept_get_array_element(const lept_value* v, size_t index);

/* lept get object function */
extern size_t      lept_get_object_size(const lept_value* v);
extern lept_value* lept_get_object_key(const lept_value* v, size_t index);
extern size_t      lept_get_object_key_length(const lept_value* v, size_t index);
extern lept_value* lept_get_object_value(const lept_value* v, size_t index);

/* ... */

#endif  /* _LEPTJSON_H_ */

