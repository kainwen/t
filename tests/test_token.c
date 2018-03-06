#include <stdio.h>

#include "cii/list.h"
#include "token.h"

#include "ctest.h"


CTEST(suite1, test1) {
    char *fn = "/tmp/test_tok";
    FILE* f = fopen(fn, "w");
    fprintf(f, "def f(1.2, -)\n");
    fclose(f);

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
    /* , */
    ASSERT_TRUE(IS_TOKEN(l->rest->rest->rest->rest->rest->rest->first, RBracketToken));
    ASSERT_STR(TOK_TEXT(l->rest->rest->rest->rest->rest->rest->first), ")");
}
