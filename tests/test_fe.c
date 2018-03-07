#include <stddef.h>

#define CTEST_MAIN
#define CTEST_SEGFAULT
#define CTEST_COLOR_OK

#include "ctest.h"

#include "cii/list.h"

#include "token.h"
#include "ast.h"
#include "tests/utils.h"


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
}

CTEST(suite_fe, test_parse1) {
    char *fn = path_join(test_data_path, "test_parse.1");
    List_T tokens = tokenize(fn);
    ExprAst e = parse(tokens);

    ASSERT_TRUE(e->tag == NumExprAst_T);
    ASSERT_DBL_NEAR(((NumExprAst)e)->val, 1123.56);
}

CTEST(suite_fe, test_parse2) {
    char *fn = path_join(test_data_path, "test_parse.2");
    List_T tokens = tokenize(fn);
    ExprAst e = parse(tokens);

    ASSERT_TRUE(e->tag == VarExprAst_T);
    ASSERT_STR(((VarExprAst)e)->name, "asd123");
}

CTEST(suite_fe, test_parse3) {
    char *fn = path_join(test_data_path, "test_parse.3");
    List_T tokens = tokenize(fn);
    ExprAst e = parse(tokens);

    ASSERT_TRUE(e->tag == BinExprAst_T);
    ASSERT_STR(((BinExprAst)e)->op, "-");

    ExprAst LHS = ((BinExprAst)e)->LHS;
    ExprAst RHS = ((BinExprAst)e)->RHS;

    ASSERT_TRUE(LHS->tag == NumExprAst_T);
    ASSERT_DBL_NEAR(((NumExprAst)LHS)->val, 3.0);

    ASSERT_TRUE(RHS->tag == BinExprAst_T);
    ASSERT_STR(((BinExprAst)RHS)->op, "*");

    ExprAst LHS1 = ((BinExprAst)RHS)->LHS;
    ExprAst RHS1 = ((BinExprAst)RHS)->RHS;

    ASSERT_TRUE(LHS1->tag == NumExprAst_T);
    ASSERT_DBL_NEAR(((NumExprAst)LHS1)->val, 5.0);
    
    ASSERT_TRUE(RHS1->tag == NumExprAst_T);
    ASSERT_DBL_NEAR(((NumExprAst)RHS1)->val, 2.0);
}

CTEST(suite_fe, test_parse4) {
    char *fn = path_join(test_data_path, "test_parse.4");
    List_T tokens = tokenize(fn);
    ExprAst e = parse(tokens);

    ASSERT_TRUE(e->tag == CallExprAst_T);
    
    ExprAst callee = ((CallExprAst)e)->callee;

    ASSERT_TRUE(callee->tag == VarExprAst_T);
    ASSERT_STR(((VarExprAst)callee)->name, "f");

    List_T Args = ((CallExprAst)e)->args;
    
    ASSERT_TRUE(List_length(Args) == 3);

    NumExprAst n1 = (NumExprAst)(Args->first);
    NumExprAst n2 = (NumExprAst)(Args->rest->first);
    NumExprAst n3 = (NumExprAst)(Args->rest->rest->first);

    ASSERT_TRUE(n1->tag == NumExprAst_T);
    ASSERT_TRUE(n2->tag == NumExprAst_T);
    ASSERT_TRUE(n3->tag == NumExprAst_T);
    
    ASSERT_DBL_NEAR(n1->val, 1.0);
    ASSERT_DBL_NEAR(n2->val, 2.0);
    ASSERT_DBL_NEAR(n3->val, 3.0);
}

CTEST(suite_fe, test_parse5) {
    char *fn = path_join(test_data_path, "test_parse.5");
    List_T tokens = tokenize(fn);
    ExprAst e = parse(tokens);

    ASSERT_TRUE(e->tag == FunctionAst_T);

    PrototypeAst prototype = ((FunctionAst)e)->proto;
    
    ASSERT_TRUE(prototype->tag == PrototypeAst_T);
    ASSERT_STR(prototype->function_name, "f");

    List_T paras = prototype->args;

    ASSERT_EQUAL(List_length(paras), 2);

    VarExprAst para1 = (VarExprAst)(paras->first);

    ASSERT_TRUE(para1->tag == VarExprAst_T);
    ASSERT_STR(para1->name, "x");
    
    VarExprAst para2 = (VarExprAst)(paras->rest->first);

    ASSERT_TRUE(para2->tag == VarExprAst_T);
    ASSERT_STR(para2->name, "y");
    
    ExprAst body = ((FunctionAst)e)->body;
    
    ASSERT_TRUE(body->tag == BinExprAst_T);
    ASSERT_STR(((BinExprAst)body)->op, "+");

    ExprAst LHS = ((BinExprAst)body)->LHS;
    
    ASSERT_TRUE(LHS->tag == VarExprAst_T);
    ASSERT_STR(((VarExprAst)LHS)->name, "x");

    ExprAst RHS = ((BinExprAst)body)->RHS;
    
    ASSERT_TRUE(RHS->tag == CallExprAst_T);

    ExprAst callee = ((CallExprAst)RHS)->callee;

    ASSERT_TRUE(callee->tag == VarExprAst_T);
    ASSERT_STR(((VarExprAst)callee)->name, "f");

    List_T args = ((CallExprAst)RHS)->args;
    
    ASSERT_EQUAL(List_length(args), 2);
    
    VarExprAst arg1 = (VarExprAst)(args->first);

    ASSERT_TRUE(arg1->tag == VarExprAst_T);
    ASSERT_STR(arg1->name, "y");
    
    VarExprAst arg2 = (VarExprAst)(args->rest->first);

    ASSERT_TRUE(arg2->tag == VarExprAst_T);
    ASSERT_STR(arg2->name, "x");
}

CTEST(suite_fe, test_parse6) {
    char *fn = path_join(test_data_path, "test_parse.6");
    List_T tokens = tokenize(fn);
    ExprAst e = parse(tokens);

    ASSERT_TRUE(e->tag == CmpExprAst_T);
    ASSERT_STR(((CmpExprAst)e)->op, "<=");

    ExprAst C1 = ((CmpExprAst)e)->C1;
    ExprAst C2 = ((CmpExprAst)e)->C2;

    ASSERT_TRUE(C1->tag == NumExprAst_T);
    ASSERT_TRUE(C2->tag == NumExprAst_T);
    ASSERT_DBL_NEAR(((NumExprAst)C1)->val, 1.0);
    ASSERT_DBL_NEAR(((NumExprAst)C2)->val, 2.0);
}

int main(int argc, char *argv[])
{
    test_data_path = argv[1];
    int result = ctest_main(0, NULL);
    return result;
}
