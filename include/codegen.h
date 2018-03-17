#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"

#include "llvm-c/Core.h"
#include "llvm-c/Analysis.h"

extern void InitEnv(const char *module_name);
extern LLVMValueRef codegen(ExprAst expr_ast);
extern void DumpResult(void);

#endif
