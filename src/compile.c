#include <libgen.h>

#include "cii/list.h"
#include "token.h"
#include "ast.h"
#include "codegen.h"


int main(int argc, char *argv[])
{
    char *input_src = argv[1];
    List_T tokens = tokenize(input_src);
    List_T es = parse(tokens);

    char *module_name = basename(input_src);
    InitEnv(module_name);
    codegen(es);
    DumpResult();
    
    return 0;
}
