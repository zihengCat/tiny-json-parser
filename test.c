#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leptjson.h"

static int main_ret   = 0;
static int test_count = 0;
static int test_pass  = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do{                                                  \
        test_count++;                                    \
        if(equality){                                    \
            test_pass++;                                 \
        }else{                                           \
            fprintf(stderr, "%s:%d: expect: " format     \
                            " actual: " format "\n",     \
                            __FILE__, __LINE__, expect, actual); \
            main_ret = 1;                                \
        }                                                \
    } while(0)

#define EXPECT_EQ_INT(expect, actual)   \
    EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

#define EXPECT_EQ_DOUBLE(expect, actual) \
    EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%.17g")

#define TEST_NUMBER(expect, json)                               \
    do {                                                        \
            lept_value v;                                       \
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

    /* must underflow */
    TEST_NUMBER(0.0, "1e-10000");

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
    v.type = LEPT_TRUE;
    EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "null"));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
}
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

#define TEST_ERROR(error, json) \
    do{                         \
        lept_value v;           \
        v.type = LEPT_FALSE;    \
        EXPECT_EQ_INT(error, lept_parse(&v, json));  \
        EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v)); \
    } while(0)

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
    test_parse_true();
    test_parse_false();
    test_parse_expert_value();
	test_parse_invalid_value();
    test_parse_root_not_singular();
    test_parse_number();

    /* ... */
}

int main(void){
    test_parse();
    /* printf("main_ret: %d\n", main_ret); */
    printf("%d/%d (%3.2f%%) passed\n", 
           test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}
