#include <stddef.h>

#define CTEST_MAIN
#define CTEST_SEGFAULT
#define CTEST_COLOR_OK

#include "ctest.h"

#include "cii/list.h"

#include "token.h"
#include "ast.h"
#include "tests/utils.h"


#define CHECK_VAR(e,s) do \
                     { \
                        ASSERT_TRUE((e)->tag == VarExprAst_T);\
                        ASSERT_STR(((VarExprAst)(e))->name, s);\
                     } while (0)
#define CHECK_NUM(e,v) do \
                       { \
                         ASSERT_TRUE((e)->tag == NumExprAst_T);\
                         ASSERT_DBL_NEAR(((NumExprAst)(e))->val, v);\
                       } while(0)

char *test_data_path = NULL;


CTEST(suite_fe, test_tok) {
    char *fn = path_join(test_data_path, "test_tok");
    
    List_T l = tokenize(fn);
    /* def */
    ASSERT_TRUE(IS_TOKEN(l->first, DefunToken));
    ASSERT_STR(TOK_TEXT(l->first), "def");
    /* var */
    l = l->rest;
    ASSERT_TRUE(IS_TOKEN(l->first, VarToken));
    ASSERT_STR(TOK_TEXT(l->first), "f");
    /* ( */
    l = l->rest;
    ASSERT_TRUE(IS_TOKEN(l->first, LBracketToken));
    ASSERT_STR(TOK_TEXT(l->first), "(");
    /* Num */
    l = l->rest;
    ASSERT_TRUE(IS_TOKEN(l->first, NumToken));
    ASSERT_STR(TOK_TEXT(l->first), "1.2");
    ASSERT_DBL_NEAR(((NumToken)(l->first))->num, 1.2);
    /* , */
    l = l->rest;
    ASSERT_TRUE(IS_TOKEN(l->first, CommaToken));
    ASSERT_STR(TOK_TEXT(l->first), ",");
    /* Op */
    l = l->rest;
    ASSERT_TRUE(IS_TOKEN(l->first, OpToken));
    ASSERT_STR(TOK_TEXT(l->first), "-");
    /* ) */
    l = l->rest;
    ASSERT_TRUE(IS_TOKEN(l->first, RBracketToken));
    ASSERT_STR(TOK_TEXT(l->first), ")");
    /* { */
    l = l->rest;
    ASSERT_TRUE(IS_TOKEN(l->first, LBraceToken));
    ASSERT_STR(TOK_TEXT(l->first), "{");
    /* } */
    l = l->rest;
    ASSERT_TRUE(IS_TOKEN(l->first, RBraceToken));
    ASSERT_STR(TOK_TEXT(l->first), "}");
    /* if */
    l = l->rest;
    ASSERT_TRUE(IS_TOKEN(l->first, IfToken));
    ASSERT_STR(TOK_TEXT(l->first), "if");
    /* else */
    l = l->rest;
    ASSERT_TRUE(IS_TOKEN(l->first, ElseToken));
    ASSERT_STR(TOK_TEXT(l->first), "else");
    /* < */
    l = l->rest;
    ASSERT_TRUE(IS_TOKEN(l->first, OpToken));
    ASSERT_STR(TOK_TEXT(l->first), "<");
    /* > */
    l = l->rest;
    ASSERT_TRUE(IS_TOKEN(l->first, OpToken));
    ASSERT_STR(TOK_TEXT(l->first), ">");
    /* <= */
    l = l->rest;
    ASSERT_TRUE(IS_TOKEN(l->first, OpToken));
    ASSERT_STR(TOK_TEXT(l->first), "<=");
    /* >= */
    l = l->rest;
    ASSERT_TRUE(IS_TOKEN(l->first, OpToken));
    ASSERT_STR(TOK_TEXT(l->first), ">=");
    /* [ */
    l = l->rest;
    ASSERT_TRUE(IS_TOKEN(l->first, LSqBraceToken));
    ASSERT_STR(TOK_TEXT(l->first), "[");
    /* ] */
    l = l->rest;
    ASSERT_TRUE(IS_TOKEN(l->first, RSqBraceToken));
    ASSERT_STR(TOK_TEXT(l->first), "]");

    /* then */
    l = l->rest;
    ASSERT_TRUE(IS_TOKEN(l->first, ThenToken));
    ASSERT_STR(TOK_TEXT(l->first), "then");

    /* ; */
    l = l->rest;
    ASSERT_TRUE(IS_TOKEN(l->first, SemicolonToken));
    ASSERT_STR(TOK_TEXT(l->first), ";");

    /* extern */
    l = l->rest;
    ASSERT_TRUE(IS_TOKEN(l->first, ExternToken));
    ASSERT_STR(TOK_TEXT(l->first), "extern");
}

CTEST(suite_fe, test_parse) {
    ExprAst e;
    char *fn = path_join(test_data_path, "test_parse");

    List_T tokens = tokenize(fn);
    List_T es = parse(tokens);

    /* Number */
    e = es->first;
    CHECK_NUM(e, 1123.56);

    /* Var */
    es = es->rest;
    e = es->first;
    CHECK_VAR(e, "asd123");

    /* BinExp */
    es = es->rest;
    e = es->first;
    ASSERT_TRUE(e->tag == BinExprAst_T);
    ASSERT_STR(((BinExprAst)e)->op, "-");

    ExprAst lhs, rhs;
    lhs = ((BinExprAst)e)->LHS;
    CHECK_NUM(lhs, 3.0);

    rhs = ((BinExprAst)e)->RHS;
    ASSERT_TRUE(rhs->tag == BinExprAst_T);
    ASSERT_STR(((BinExprAst)rhs)->op, "*");
    CHECK_NUM(((BinExprAst)rhs)->LHS, 5.0);
    CHECK_NUM(((BinExprAst)rhs)->RHS, 2.0);

    /* FunCall */
    es = es->rest;
    e = es->first;
    ASSERT_TRUE(e->tag == CallExprAst_T);
    ASSERT_STR(((CallExprAst)e)->callee, "f");
    List_T args = ((CallExprAst)e)->args;
    ASSERT_TRUE(List_length(args) == 3);
    e = args->first;
    CHECK_NUM(e, 1.0);
    args = args->rest;
    e = args->first;
    CHECK_NUM(e, 2.0);
    args = args->rest;
    e = args->first;
    CHECK_NUM(e, 3.0);

    /* Defun */
    es = es->rest;
    e = es->first;
    ASSERT_TRUE(e->tag == FunctionAst_T);
    PrototypeAst proto = ((FunctionAst)e)->proto;
    ExprAst body = ((FunctionAst)e)->body;
    ASSERT_TRUE(proto->tag == PrototypeAst_T);
    ASSERT_STR(proto->function_name, "f");
    List_T params = proto->args;
    ASSERT_TRUE(List_length(params) == 2);
    e = params->first;
    CHECK_VAR(e, "x");
    e = params->rest->first;
    CHECK_VAR(e, "y");
    ASSERT_TRUE(e->tag == BinExprAst_T);

}

int main(int argc, char *argv[])
{
    test_data_path = argv[1];
    int result = ctest_main(0, NULL);
    return result;
}
