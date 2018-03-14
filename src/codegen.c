#include <string.h>

#include "cii/assert.h"
#include "cii/except.h"
#include "cii/mem.h"
#include "cii/table.h"

#include "codegen.h"

#define TABLE_HINT_SIZE 20

typedef LLVMValueRef (*BinLLVMBuildFuncType) (LLVMBuilderRef,
                                              LLVMValueRef,
                                              LLVMValueRef,
                                              const char *);

typedef struct Closure
{
    int index;
    void *array;
} Closure;

Except_T Unknown_Expr = { "Unknown Expr" };
Except_T Unknown_Var_Name = { "Unknown Variable Name" };
Except_T Unknown_Arith_Operator = { "Unknown Arith Operator" };
Except_T Unknown_Func_Name = { "Unknown Function Name" };
Except_T Incorrect_Num_Args = { "Incorrect Number of Args" };

static LLVMBuilderRef Builder;
static LLVMContextRef Context;
static LLVMModuleRef Module;
static Table_T NamedValues;

static int INIT_CALLED = 0;

static int cmp_var(const void *x, const void *y);
static unsigned int hash_var(const void *key);
static void set_Args(void **x, void *cl);
static LLVMValueRef handle_num(NumExprAst expr_ast);
static LLVMValueRef handle_var(VarExprAst expr_ast);
static LLVMValueRef handle_bin(BinExprAst expr_ast);
static LLVMValueRef handle_call(CallExprAst expr_ast);


void
InitEnv(const char *module_name)
{
    assert(INIT_CALLED == 0);
    
    Context = LLVMGetGlobalContext();
    Builder = LLVMCreateBuilderInContext(Context);
    Module = LLVMModuleCreateWithNameInContext(module_name, Context);

    NamedValues = Table_new(TABLE_HINT_SIZE, cmp_var, hash_var);

    INIT_CALLED = 1;
}

void
DumpResult(void)
{
    assert(INIT_CALLED == 1);
    LLVMDumpModule(Module);
}

LLVMValueRef
codegen(ExprAst expr_ast)
{
    LLVMValueRef value = NULL;
    
    assert(INIT_CALLED == 1);
    
    switch(expr_ast->tag)
    {
    case NumExprAst_T:
        value = handle_num((NumExprAst)expr_ast);
        break;
    case VarExprAst_T:
        value = handle_var((VarExprAst)expr_ast);
        break;
    case BinExprAst_T:
        value = handle_bin((BinExprAst)expr_ast);
        break;
    case CallExprAst_T:
        value = handle_call((CallExprAst)expr_ast);
        break;
    default:
        RAISE(Unknown_Expr);
    }

    return value;
}

static int
cmp_var(const void *x, const void *y)
{
    assert(x);
    assert(y);
    return strcmp((char*)x, (char*)y);
}

static unsigned int
hash_var(const void *key)
{
    char *k = (char *)key;
    unsigned hashVal = 0;

	while (*k != '\0') {
		hashVal = (hashVal << 4) + *k++;
		unsigned g = hashVal & 0xF0000000L;
		if (g != 0)
            hashVal ^= g >> 24;
		hashVal &= ~g;
	}
	return hashVal;
}

static void
set_Args(void **x, void *cl)
{
    Closure *cls = (Closure*)cl;
    ExprAst expr_ast = *((ExprAst*)x);
    LLVMValueRef value = codegen(expr_ast);
    LLVMValueRef *Args = (LLVMValueRef *)(cls->array);
    Args[cls->index++] = value;
}

static LLVMValueRef
handle_num(NumExprAst expr_ast)
{
    return LLVMConstReal(LLVMDoubleType(), expr_ast->val);
}

static LLVMValueRef
handle_var(VarExprAst expr_ast)
{
    void *value;
    
    value = Table_get(NamedValues, (void *)(expr_ast->name));
    if (value == NULL)
        RAISE(Unknown_Var_Name);

    return (LLVMValueRef)value;
}

static LLVMValueRef
handle_bin(BinExprAst expr_ast)
{
    char *name;
    char *op = expr_ast->op;
    LLVMValueRef value, lhs, rhs;
    BinLLVMBuildFuncType bin_build_func = NULL;
    
    if (strcmp(op, "+") == 0)
    {
        bin_build_func = LLVMBuildFAdd;
        name = "addtmp";
    }
    else if (strcmp(op, "-") == 0)
    {
        bin_build_func = LLVMBuildFSub;
        name = "subtmp";
    }
    else if (strcmp(op, "*") == 0)
    {
        bin_build_func = LLVMBuildFMul;
        name = "multmp";
    }
    else if (strcmp(op, "/") == 0)
    {
        bin_build_func = LLVMBuildFDiv;
        name = "divtmp";
    }
    else
        RAISE(Unknown_Arith_Operator);

    lhs = codegen(expr_ast->LHS);
    rhs = codegen(expr_ast->RHS);
    value = bin_build_func(Builder, lhs, rhs, name);

    return value;
}

static LLVMValueRef
handle_call(CallExprAst expr_ast)
{
    void *func_val;
    char *callee = expr_ast->callee;
    LLVMValueRef func;
    LLVMValueRef value;
    int i;

    func_val = Table_get(NamedValues, (void *)callee);
    if (func_val == NULL)
        RAISE(Unknown_Func_Name);

    func = (LLVMValueRef)func_val;

    List_T args = expr_ast->args;
    if (List_length(args) != LLVMCountParams(func))
        RAISE(Incorrect_Num_Args);

    LLVMValueRef *Args;
    Closure cl;
    cl.index = 0;
    cl.array = (void*)Args;
    Args = ALLOC(sizeof(LLVMValueRef) * List_length(args));
    List_map(args, set_Args, &cl);

    value = LLVMBuildInvoke(Builder, func, Args, List_length(args),
                            NULL, NULL, "func_call");
    return value;
}
