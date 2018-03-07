#include <stddef.h>

#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest.h"

#include "cii/list.h"

#include "token.h"
#include "tests/utils.h"


char *test_data_path = NULL;


CTEST(suite1, test1) {
    char *fn = path_join(test_data_path, "test_tok");
    
    List_T l = tokenize(fn);
    /* def */
    ASSERT_TRUE(IS_TOKEN(l->first, DefunToken));
    ASSERT_STR(TOK_TEXT(l->first), "def");
    /* var */
    ASSERT_TRUE(IS_TOKEN(l->rest->first, VarToken));
    ASSERT_STR(TOK_TEXT(l->rest->first), "f");
    /* ( */
    ASSERT_TRUE(IS_TOKEN(l->rest->rest->first, LBracketToken));
    ASSERT_STR(TOK_TEXT(l->rest->rest->first), "(");
    /* Num */
    ASSERT_TRUE(IS_TOKEN(l->rest->rest->rest->first, NumToken));
    ASSERT_STR(TOK_TEXT(l->rest->rest->rest->first), "1.2");
    ASSERT_DBL_NEAR(((NumToken)(l->rest->rest->rest->first))->num, 1.2);
    /* , */
    ASSERT_TRUE(IS_TOKEN(l->rest->rest->rest->rest->first, CommaToken));
    ASSERT_STR(TOK_TEXT(l->rest->rest->rest->rest->first), ",");
    /* Op */
    ASSERT_TRUE(IS_TOKEN(l->rest->rest->rest->rest->rest->first, OpToken));
    ASSERT_STR(TOK_TEXT(l->rest->rest->rest->rest->rest->first), "-");
    ASSERT_EQUAL(((OpToken)(l->rest->rest->rest->rest->rest->first))->op, '-');
    /* ) */
    ASSERT_TRUE(IS_TOKEN(l->rest->rest->rest->rest->rest->rest->first, RBracketToken));
    ASSERT_STR(TOK_TEXT(l->rest->rest->rest->rest->rest->rest->first), ")");
}


int main(int argc, const char *argv[])
{
    test_data_path = argv[1];
    int result = ctest_main(0, NULL);
    return result;
}
