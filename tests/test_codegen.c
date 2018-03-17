#include <stdio.h>

#define CTEST_MAIN
#define CTEST_SEGFAULT
#define CTEST_COLOR_OK

#include "ctest.h"

#include "cii/list.h"

#include "token.h"
#include "ast.h"
#include "codegen.h"

#include "tests/utils.h"


char *test_data_path = NULL;


CTEST(suite_codegen, test_codegen1)
{
    char *fn = path_join(test_data_path, "test_codegen.1");
    List_T tokens = tokenize(fn);
    ExprAst e = parse(tokens);
    
    InitEnv("test_module1");
    codegen(e);

    freopen("/tmp/haha", "w", stderr);
    
    DumpResult();
}

int main(int argc, char *argv[])
{
    test_data_path = argv[1];
    int result = ctest_main(0, NULL);
    return result;
}
