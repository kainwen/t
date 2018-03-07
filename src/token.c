#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

#include "cii/mem.h"
#include "cii/except.h"
#include "cii/assert.h"


#define NEW_TOKEN(t)   t tok; \
                       NEW0(tok); \
                       tok->tag = t##_T; \
                       strcpy(tok->raw_text, yytext)



Except_T Unknow_Token = { "Unknown token" };

static void *make_token(int token_tag);

List_T
tokenize(const char *fn)
{
    extern FILE *yyin;
    extern int yylex();
    List_T l = NULL;
    
    yyin = fopen(fn, "r");
    assert(yyin);

    int token_tag = yylex();
    while (token_tag)
    {
        void *token;
        token = make_token(token_tag);
        l = List_push(l, token);
        token_tag = yylex();
    }
    
    l = List_reverse(l);
    return l;
}

static void *
make_token(int token_tag)
{
    void *result;
    extern char *yytext;
    
    switch(token_tag)
    {
    case NumToken_T:
    {
        NEW_TOKEN(NumToken);
        tok->num = atof(yytext);
        result = tok;
        break;
    }
    case VarToken_T:
    {
        NEW_TOKEN(VarToken);
        result = tok;
        break;
    }
    case OpToken_T:
    {
        NEW_TOKEN(OpToken);
        tok->op = yytext[0];
        result = tok;
        break;
    }
    case LBracketToken_T:
    {
        NEW_TOKEN(LBracketToken);
        result = tok;
        break;
    }
    case RBracketToken_T:
    {
        NEW_TOKEN(RBracketToken);
        result = tok;
        break;
    }
    case LBraceToken_T:
    {
        NEW_TOKEN(LBraceToken);
        result = tok;
        break;
    }
    case RBraceToken_T:
    {
        NEW_TOKEN(RBraceToken);
        result = tok;
        break;
    }
    case CommaToken_T:
    {
        NEW_TOKEN(CommaToken);
        result = tok;
        break;
    }
    case DefunToken_T:
    {
        NEW_TOKEN(DefunToken);
        result = tok;
        break;
    }
    default:
        RAISE(Unknow_Token);
    }
    return result;
}
