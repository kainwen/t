#ifndef TOKEN_H
#define TOKEN_H

#include "cii/list.h"

#define MAX_TOKEN_LEN 31

enum Token_Tag
{
    NumToken_T=17,
    VarToken_T,
    OpToken_T,
    LBracketToken_T,
    RBracketToken_T,
    LBraceToken_T,
    RBraceToken_T,
    CommaToken_T,
    DefunToken_T,
    IfToken_T,
    ElseToken_T,
    LSqBraceToken_T,
    RSqBraceToken_T
};

struct Token
{
    enum Token_Tag tag;
    char raw_text[MAX_TOKEN_LEN];
};

struct NumToken
{
    enum Token_Tag tag;
    char raw_text[MAX_TOKEN_LEN];
    double num;
};

struct VarToken
{
    enum Token_Tag tag;
    char raw_text[MAX_TOKEN_LEN];
};

struct OpToken
{
    enum Token_Tag tag;
    char raw_text[MAX_TOKEN_LEN];
};

struct LBracketToken
{
    enum Token_Tag tag;
    char raw_text[MAX_TOKEN_LEN];
};

struct RBracketToken
{
    enum Token_Tag tag;
    char raw_text[MAX_TOKEN_LEN];
};

struct LBraceToken
{
    enum Token_Tag tag;
    char raw_text[MAX_TOKEN_LEN];
};

struct RBraceToken
{
    enum Token_Tag tag;
    char raw_text[MAX_TOKEN_LEN];
};

struct CommaToken
{
    enum Token_Tag tag;
    char raw_text[MAX_TOKEN_LEN];
};

struct DefunToken
{
    enum Token_Tag tag;
    char raw_text[MAX_TOKEN_LEN];
};

struct IfToken
{
    enum Token_Tag tag;
    char raw_text[MAX_TOKEN_LEN];
};

struct ElseToken
{
    enum Token_Tag tag;
    char raw_text[MAX_TOKEN_LEN];
};

struct LSqBraceToken
{
    enum Token_Tag tag;
    char raw_text[MAX_TOKEN_LEN];
};

struct RSqBraceToken
{
    enum Token_Tag tag;
    char raw_text[MAX_TOKEN_LEN];
};

typedef struct Token *Token;
typedef struct NumToken *NumToken;
typedef struct VarToken *VarToken;
typedef struct OpToken *OpToken;
typedef struct LBracketToken *LBracketToken;
typedef struct RBracketToken *RBracketToken;
typedef struct LBraceToken *LBraceToken;
typedef struct RBraceToken *RBraceToken;
typedef struct CommaToken *CommaToken;
typedef struct DefunToken *DefunToken;
typedef struct IfToken *IfToken;
typedef struct ElseToken *ElseToken;
typedef struct LSqBraceToken *LSqBraceToken;
typedef struct RSqBraceToken *RSqBraceToken;

#define TOK_TAG(t) (((Token)(t))->tag)
#define TOK_TEXT(t) (((Token)(t))->raw_text)
#define IS_TOKEN(t,_type_) (TOK_TAG(t) == _type_##_T)

extern List_T tokenize(const char *fn);

#endif
