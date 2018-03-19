#ifndef AST_H
#define AST_H

#include "cii/list.h"

enum Tag
{
    NumExprAst_T=97,
    VarExprAst_T,
    BinExprAst_T,
    CallExprAst_T,
    PrototypeAst_T,
    FunctionAst_T,
    IfExprAst_T,
    CmpExprAst_T
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
    char *name;
};

struct BinExprAst
{
    enum Tag tag;
    char* op;
    struct ExprAst *LHS;
    struct ExprAst *RHS;
};

struct CallExprAst
{
    enum Tag tag;
    char *callee;
    List_T args;
};

struct PrototypeAst
{
    enum Tag tag;
    char *function_name;
    List_T args;
};

struct FunctionAst
{
    enum Tag tag;
    struct PrototypeAst *proto;
    struct ExprAst *body;
};

struct IfExprAst
{
    enum Tag tag;
    struct CmpExprAst *cmp;
    struct ExprAst *then_body;
    struct ExprAst *else_body;
};

struct CmpExprAst
{
    enum Tag tag;
    char *op;
    struct ExprAst *C1;
    struct ExprAst *C2;
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
typedef struct IfExprAst* IfExprAst;
typedef struct CmpExprAst* CmpExprAst;

#define TAG(expr_ptr) (((ExprAst)(expr_ptr))->tag)
#define IsA(expr_ptr,_type_) (TAG(expr_ptr) == _type_##_T)

extern List_T parse(List_T Tokens);

#endif
