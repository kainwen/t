#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"

#include "llvm-c/Core.h"
#include "llvm-c/Analysis.h"

extern void InitEnv(const char *module_name);
extern void codegen(List_T expr_asts);
extern void DumpResult(void);

#endif
