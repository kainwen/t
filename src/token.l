%{
#include <stdlib.h>
#include "token.h"

%}

%%
def                  { return DefunToken_T; }
[-+]?[0-9]*\.?[0-9]+ { return NumToken_T; }
[a-zA-Z][0-9a-zA-Z]* { return VarToken_T; }
[+\-*/]              { return OpToken_T; }
[(]                  { return LBracketToken_T; }
[)]                  { return RBracketToken_T; }
[,]                  { return CommaToken_T; }
[ \n\t] ;

%%
