#include <string.h>
#include <stddef.h>

#include "cii/assert.h"
#include "cii/except.h"
#include "cii/mem.h"
#include "cii/table.h"

#include "codegen.h"


#define TABLE_HINT_SIZE 20

#define NEW_TABLE(T) do { \
                       T = Table_new(TABLE_HINT_SIZE, cmp_var, hash_var); \
                     } while (0)


typedef LLVMValueRef (*BinLLVMBuildFuncType) (LLVMBuilderRef,
                                              LLVMValueRef,
                                              LLVMValueRef,
                                              const char *);

typedef struct Closure
{
    int index;
    void *array;
    void *extra;
} Closure;

Except_T Unknown_Expr = { "Unknown Expr" };
Except_T Unknown_Var_Name = { "Unknown Variable Name" };
Except_T Unknown_Arith_Operator = { "Unknown Arith Operator" };
Except_T Unknown_Func_Name = { "Unknown Function Name" };
Except_T Incorrect_Num_Args = { "Incorrect Number of Args" };
Except_T Function_Redefine = { "Function cannot be redefined" };

static LLVMBuilderRef Builder;
static LLVMContextRef Context;
static LLVMModuleRef Module;
static Table_T NamedValues;

static int INIT_CALLED = 0;

static int cmp_var(const void *x, const void *y);
static unsigned int hash_var(const void *key);
static void set_call_args(void **x, void *cl);
static LLVMValueRef handle_num(NumExprAst expr_ast);
static LLVMValueRef handle_var(VarExprAst expr_ast);
static LLVMValueRef handle_bin(BinExprAst expr_ast);
static LLVMValueRef handle_call(CallExprAst expr_ast);
static LLVMValueRef handle_prototype(PrototypeAst expr_ast);
static LLVMValueRef handle_function(FunctionAst expr_ast);


void
InitEnv(const char *module_name)
{
    assert(INIT_CALLED == 0);
    
    Context = LLVMGetGlobalContext();
    assert(Context);
    
    Builder = LLVMCreateBuilderInContext(Context);
    assert(Builder);
    
    Module = LLVMModuleCreateWithNameInContext(module_name, Context);
    assert(Module);

    NamedValues = Table_new(TABLE_HINT_SIZE, cmp_var, hash_var);
    assert(NamedValues);

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
    case PrototypeAst_T:
        value = handle_prototype((PrototypeAst)expr_ast);
        break;
    case FunctionAst_T:
        value = handle_function((FunctionAst)expr_ast);
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
set_call_args(void **x, void *cl)
{
    Closure *cls = (Closure*)cl;
    ExprAst expr_ast = *((ExprAst*)x);
    LLVMValueRef value = codegen(expr_ast);
    LLVMValueRef *Args = (LLVMValueRef *)(cls->array);
    Args[cls->index++] = value;
}

static void
set_arg_name(void **x, void *cl)
{
    Closure *cls = (Closure*)cl;
    VarExprAst var = *((VarExprAst*)x);
    LLVMValueRef *funcP = cls->extra;
    LLVMValueRef param = LLVMGetParam(*funcP, cls->index++);
    LLVMSetValueName(param, var->name);
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
    if (!lhs || !rhs)
        return NULL;

    value = bin_build_func(Builder, lhs, rhs, name);
    
    return value;
}

static LLVMValueRef
handle_call(CallExprAst expr_ast)
{
    LLVMValueRef func;
    char *callee = expr_ast->callee;
    LLVMValueRef value;
    int i;

    func = LLVMGetNamedFunction(Module, callee);
    if (func == NULL)
        RAISE(Unknown_Func_Name);

    List_T args = expr_ast->args;
    if (List_length(args) != LLVMCountParams(func))
        RAISE(Incorrect_Num_Args);

    LLVMValueRef *Args;
    Closure cl;
    cl.index = 0;
    Args = ALLOC(sizeof(LLVMValueRef) * List_length(args));
    cl.array = (void*)Args;
    List_map(args, set_call_args, &cl);

    value = LLVMBuildCall(Builder, func, Args, List_length(args), "func_call");
    return value;
}

static LLVMValueRef
handle_prototype(PrototypeAst expr_ast)
{
    LLVMTypeRef func_type;
    LLVMValueRef func;
    int i;
    
    List_T args = expr_ast->args;
    int arg_num = List_length(args);
    LLVMTypeRef *param_types;
    
    param_types = ALLOC(sizeof(LLVMTypeRef) * arg_num);
    for (i = 0; i < arg_num; i++)
        param_types[i] = LLVMDoubleType();

    func_type = LLVMFunctionType(LLVMDoubleType(), param_types, arg_num, 0);
    func = LLVMAddFunction(Module, expr_ast->function_name, func_type);
    LLVMSetLinkage(func, LLVMExternalLinkage);

    Closure cl;
    cl.index = 0;
    cl.extra = (void*)&func;
    List_map(args, set_arg_name, &cl);

    return func;
}

static LLVMValueRef
handle_function(FunctionAst expr_ast)
{
    LLVMValueRef func;
    func = LLVMGetNamedFunction(Module, expr_ast->proto->function_name);
    if (!func)
        func = handle_prototype(expr_ast->proto);
    
    if (!func)
        return NULL;

    if (LLVMCountBasicBlocks(func) > 0)
        RAISE(Function_Redefine);

    LLVMBasicBlockRef basic_block;
    basic_block = LLVMAppendBasicBlockInContext(Context, func, "entry");
    LLVMPositionBuilderAtEnd(Builder, basic_block);
    
    Table_free(&NamedValues);
    NEW_TABLE(NamedValues);

    unsigned int arg_num = LLVMCountParams(func);
    unsigned int i;

    LLVMValueRef *params;
    params = ALLOC(sizeof(LLVMValueRef) * arg_num);
    LLVMGetParams(func, params);
    for (i = 0; i < arg_num; i++)
        Table_put(NamedValues, LLVMGetValueName(params[i]), params[i]);

    LLVMValueRef body = codegen(expr_ast->body);
    if (body)
    {
        LLVMBuildRet(Builder, body);
        LLVMVerifyFunction(func, LLVMAbortProcessAction);
        return func;
    }

    LLVMDeleteFunction(func);
    return NULL;
}

