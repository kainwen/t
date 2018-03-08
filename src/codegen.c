#include <string.h>

#include "cii/assert.h"
#include "cii/except.h"
#include "cii/table.h"

#include "codegen.h"

#define TABLE_HINT_SIZE 20


Except_T Unknown_Expr = { "Unknown Expr" };
Except_T Unknown_Var_Name = { "Unknown Variable Name" };

static LLVMBuilderRef Builder;
static LLVMContextRef Context;
static LLVMModuleRef Module;
static Table_T NamedValues;

static int INIT_CALLED = 0;

static int cmp_var(const void *x, const void *y);
static unsigned int hash_var(const void *key);
static LLVMValueRef handle_num(NumExprAst expr_ast);
static LLVMValueRef handle_var(VarExprAst expr_ast);


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
