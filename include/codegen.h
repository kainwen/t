#ifndef CODEGEN_H
#define CODEGEN_H

#include "llvm-c/Core.h"
#include "ast.h"


extern void InitEnv(const char *module_name);
extern LLVMValueRef codegen(ExprAst expr_ast);

#endif
