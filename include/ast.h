#ifndef AST_H
#define AST_H

#include "cii/list.h"

enum Tag
{
    NumExprAst_T,
    VarExprAst_T,
    BinExprAst_T,
    CallExprAst_T,
    PrototypeAst_T,
    FunctionAst_T
};

struct ExprAst
{
    enum Tag tag;
};

struct NumExprAst
{
    enum Tag tag;
    double val;
};

struct VarExprAst
{
    enum Tag tag;
    const char *name;
};

struct BinExprAst
{
    enum Tag tag;
    char op;
    struct ExprAst *LHS;
    struct ExprAst *RHS;
};

struct CallExprAst
{
    enum Tag tag;
    struct ExprAst *callee;
    List_T args;
};

struct PrototypeAst
{
    enum Tag tag;
    const char *function_name;
    List_T args;
};

struct FunctionAst
{
    enum Tag tag;
    struct PrototypeAst *proto;
    struct ExprAst *body;
};

struct ParseResult
{
    struct ExprAst *expr_ast;
    List_T list_result;
    List_T Rem_tokens;
};

typedef struct ParseResult* ParseResult;
typedef struct ExprAst* ExprAst;
typedef struct NumExprAst* NumExprAst;
typedef struct VarExprAst* VarExprAst;
typedef struct BinExprAst* BinExprAst;
typedef struct CallExprAst *CallExprAst;
typedef struct PrototypeAst* PrototypeAst;
typedef struct FunctionAst* FunctionAst;

#define TAG(expr_ptr) (((ExprAst)(expr_ptr))->tag)
#define IsA(expr_ptr,_type_) (TAG(expr_ptr) == _type_##_T)

extern ParseResult parse_exp(List_T Tokens);

#endif
