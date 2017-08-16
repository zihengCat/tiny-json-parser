#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leptjson.h"

static int main_ret   = 0;
static int test_count = 0;
static int test_pass  = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do{                                             \
        test_count++;                               \
        if(equality){                               \
            test_pass++;                            \
        }else{                                      \
            fprintf(stderr, "%s:%d: expect: " format \
                            " actual: " format "\n", \
                            __FILE__, __LINE__, expect, actual); \
            main_ret = 1;                           \
        }                                           \
    }while(0)

#define EXPECT_EQ_INT(expect, actual)   \
    EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

#define EXPECT_EQ_DOUBLE(expect, actual) \
    EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%.17g")
	
#define EXPECT_EQ_STRING(expect, actual, alength)         \
    EXPECT_EQ_BASE( ((strcmp(expect, actual) == 0) &&     \
				     (strlen(expect) == alength)),        \
			          expect, actual, "%s")

#define TEST_STRING(expect, json)   \
    do {							\
        lept_value v;				\
        EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, json)); \
        EXPECT_EQ_INT(LEPT_STRING, lept_get_type(&v));      \
        EXPECT_EQ_STRING(expect, lept_get_string(&v),       \
                         lept_get_string_length(&v));       \
    } while(0)

static void test_parse_string(void) {
    TEST_STRING("", "\"\"");
    TEST_STRING("Hello", "\"Hello\"");
    TEST_STRING("Hello, ziheng", "\"Hello, ziheng\"");
    TEST_STRING("12345", "\"12345\"");

    TEST_STRING("Hello\nWorld", "\"Hello\\nWorld\"");
    TEST_STRING("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
}

#define TEST_NUMBER(expect, json)   \
    do {                        \
            lept_value v;       \
            EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, json)); \
            EXPECT_EQ_INT(LEPT_NUMBER, lept_get_type(&v));      \
            EXPECT_EQ_DOUBLE(expect, lept_get_number(&v));      \
   } while(0)

static void test_parse_number(void){

	TEST_NUMBER(0.0, "0");
    TEST_NUMBER(0.0, "-0");
    TEST_NUMBER(0.0, "-0.0");
    TEST_NUMBER(1.0, "1");
    TEST_NUMBER(-1.0, "-1");
    TEST_NUMBER(1.5, "1.5");
    TEST_NUMBER(-1.5, "-1.5");
    TEST_NUMBER(3.1416, "3.1416");
    TEST_NUMBER(1E10, "1E10");
    TEST_NUMBER(1e10, "1e10");
    TEST_NUMBER(1E+10, "1E+10");
    TEST_NUMBER(1E-10, "1E-10");
    TEST_NUMBER(-1E10, "-1E10");
    TEST_NUMBER(-1e10, "-1e10");
    TEST_NUMBER(-1E+10, "-1E+10");
    TEST_NUMBER(-1E-10, "-1E-10");
    TEST_NUMBER(1.234E+10, "1.234E+10");
    TEST_NUMBER(1.234E-10, "1.234E-10");
    TEST_NUMBER(0.0, "1e-10000"); 			/* must underflow */

	/* the smallest number > 1 */
  	TEST_NUMBER(1.0000000000000002, "1.0000000000000002"); 
	/* minimum denormal */
    TEST_NUMBER( 4.9406564584124654e-324, "4.9406564584124654e-324"); 
    TEST_NUMBER(-4.9406564584124654e-324, "-4.9406564584124654e-324");
	/* Max subnormal double */
    TEST_NUMBER( 2.2250738585072009e-308, "2.2250738585072009e-308");  
    TEST_NUMBER(-2.2250738585072009e-308, "-2.2250738585072009e-308");
	/* Min normal positive double */
    TEST_NUMBER( 2.2250738585072014e-308, "2.2250738585072014e-308");  
    TEST_NUMBER(-2.2250738585072014e-308, "-2.2250738585072014e-308");
	/* Max double */
    TEST_NUMBER( 1.7976931348623157e+308, "1.7976931348623157e+308");  
    TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308");
}

#undef TEST_NUMBER

static void test_parse_null(void){
    lept_value v;
    v.type = LEPT_BOOLEAN;
    EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "null"));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
}

#define TEST_BOOLEAN(expect, json)   \
    do {                             \
            lept_value v;            \
            v.type = LEPT_NULL;      \
            EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, json)); \
            EXPECT_EQ_INT(LEPT_BOOLEAN, lept_get_type(&v));     \
            EXPECT_EQ_INT(expect, lept_get_boolean(&v));     \
   } while(0)

static void test_parse_boolean(void){
    TEST_BOOLEAN(1, "true");
    TEST_BOOLEAN(0, "false");
    TEST_BOOLEAN(1, "\t\rtrue\t\n");
    TEST_BOOLEAN(0, "\t\rfalse\t\n");
#if 0
    lept_value v;
    v.type = LEPT_NULL;

    EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "true"));
    EXPECT_EQ_INT(LEPT_BOOLEAN, lept_get_type(&v));
    EXPECT_EQ_INT(1, lept_get_boolean(&v));

    EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "false"));
    EXPECT_EQ_INT(LEPT_BOOLEAN, lept_get_type(&v));
    EXPECT_EQ_INT(0, lept_get_boolean(&v));
#endif
}
#undef TEST_BOOLEAN

#if 0
static void test_parse_true(void){
    lept_value v;
    v.type = LEPT_NULL;
    EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "true"));
    EXPECT_EQ_INT(LEPT_TRUE, lept_get_type(&v));
}
static void test_parse_false(void){
    lept_value v;
    v.type = LEPT_NULL;
    EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "false"));
    EXPECT_EQ_INT(LEPT_FALSE, lept_get_type(&v));
}
#endif

#define TEST_ERROR(error, json) \
    do{                         \
        lept_value v;           \
        v.type = LEPT_BOOLEAN;    \
        EXPECT_EQ_INT(error, lept_parse(&v, json)); \
        EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v)); \
    }while(0)

static void test_parse_expert_value(void){
    TEST_ERROR(LEPT_PARSE_EXPECT_VALUE, "");
    TEST_ERROR(LEPT_PARSE_EXPECT_VALUE, " ");
    TEST_ERROR(LEPT_PARSE_EXPECT_VALUE, " \t");
    TEST_ERROR(LEPT_PARSE_EXPECT_VALUE, " \t\n");
}
static void test_parse_invalid_value(void) {

    /* error literal test */

    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "\t \n\t truuu \t\n");
    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "truu");
    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "falss");
    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "nul");
    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "?");
    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "tttttt");

    /* error number test */

	TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "+0");
    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "+1");
    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, ".123"); /* at least one digit before '.' */
    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "1.");   /* at least one digit after '.' */
    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "INF");
    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "inf");
    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "NAN");
    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "nan");

    /* error string test */

    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "\"hello \t ");

}
static void test_parse_root_not_singular(void) {
    TEST_ERROR(LEPT_PARSE_ROOT_NOT_SINGULAR, "null x");
    TEST_ERROR(LEPT_PARSE_ROOT_NOT_SINGULAR, "null a");
}

#undef TEST_ERROR

/* ... */

static void test_parse() {
    /* ... */

    test_parse_null();
    test_parse_boolean();
#if 0
    test_parse_true();
    test_parse_false();
#endif
    test_parse_expert_value();
	test_parse_invalid_value();
    test_parse_root_not_singular();
    test_parse_number();
    test_parse_string();

    /* ... */
}

int main(void){
    test_parse();
    /* printf("main_ret: %d\n", main_ret); */
    printf("%d/%d (%3.2f%%) passed\n", 
           test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}

