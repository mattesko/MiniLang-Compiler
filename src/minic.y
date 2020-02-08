// Code section
%{
// #include "lex.yy.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"

extern STMT_LIST *root;
extern int yylex();
extern int yylineno;

// The yyerror function is called on any syntax error. When using %define parse.error verbose, this function
// takes a single argument (a string) which contains a detailed explanation of the error.
// Otherwise it takes no arguments and it is up to the user to format the error using a minimal
// description provided in yytext.
void yyerror(const char *s) {
	fprintf(stderr, "Error: (line %d) %s\n", yylineno, s);
	exit(1);
}
void yyerror(const char *s);
%}

%code requires {
	#include "tree.h"
}

// Union part: Define kind of data values can hold
%union {
    int int_val;
    char *string_val;
    float float_val;
    bool bool_val;
    struct EXP *exp;
    struct STMT_LIST *stmt_list;
    struct STMT *stmt;
    struct TYPE *type;
    struct IFSTMT *if_stmt;
}

%type <exp> exp
%type <stmt_list> stmt_list program
%type <stmt> stmt
%type <if_stmt> if_stmt
%type <type> type

%token tVAR tFLOAT tINT tBOOL tSTRING tPRINT tWHILE tIF tELSE tREAD
%token tGT tGTE tST tSTE tEQUAL tLOGIC_AND tLOGIC_OR tNEQUAL

%token <bool_val> tBOOL_LITERAL
%token <string_val> tSTRING_LITERAL
%token <int_val> tINT_LITERAL
%token <float_val> tFLOAT_LITERAL
%token <string_val> tIDENTIFIER

// Top = Lowest Precedence. Bottom = Highest Precedence
%left tLOGIC_OR
%left tLOGIC_AND
%left tEQUAL tNEQUAL
%left tGTE tSTE tGT tST
%left '+' '-'
%left '*' '/'
%left '!'

%nonassoc '{' '}' '(' ')'
%nonassoc tELSE

%start program

%locations
%define parse.error verbose

// Grammar Section
%%
program: stmt_list  {root = $1;}
    ;

stmt_list:              {$$ = NULL;}
    | stmt stmt_list    {$$ = makeSTMT_LIST($1, $2);}
    ;

stmt: tREAD '(' tIDENTIFIER ')' ';'         {$$ = makeSTMT_read($3);}
    | tPRINT '(' exp ')' ';'                {$$ = makeSTMT_print($3);}
    | tIDENTIFIER '=' exp ';'               {$$ = makeSTMT_assignment($1, $3);}
    | tWHILE '(' exp ')' '{' stmt_list '}'  {$$ = makeSTMT_whileLoop($3, $6);}
    | tVAR tIDENTIFIER ':' type '=' exp ';' {$$ = makeSTMT_initStrictType($2, $4, $6);}
    | tVAR tIDENTIFIER '=' exp ';'          {$$ = makeSTMT_initLooseType($2, $4);}
    | if_stmt                               {$$ = makeSTMT_ifStmt($1);}
    ;

if_stmt: tIF '(' exp ')' '{' stmt_list '}' {$$ = makeIFSTMT_if($3, $6);}
    | tIF '(' exp ')' '{' stmt_list '}' tELSE '{' stmt_list '}'  {$$ = makeIFSTMT_ifElse($3, $6, $10);}
    | tIF '(' exp ')' '{' stmt_list '}' tELSE if_stmt {$$ = makeIFSTMT_ifElseIf($3, $6, $9);}
    ;

exp:  exp '+' exp           {$$ = makeEXP_binary(k_expressionKind_addition, $1, $3);}
    | exp '-' exp           {$$ = makeEXP_binary(k_expressionKind_subtraction, $1, $3);}
    | exp '*' exp           {$$ = makeEXP_binary(k_expressionKind_multiplication, $1, $3);}
    | exp '/' exp           {$$ = makeEXP_binary(k_expressionKind_division, $1, $3);}
    | '(' exp ')'           {$$ = makeEXP_withParantheses($2);}
    | '-' exp               {$$ = makeEXP_unary(k_expressionKind_unaryMinus, $2);}
    | '!' exp               {$$ = makeEXP_unary(k_expressionKind_unaryLogicNot, $2);}
    | exp tEQUAL exp        {$$ = makeEXP_binary(k_expressionKind_equal, $1, $3);}
    | exp tNEQUAL exp       {$$ = makeEXP_binary(k_expressionKind_notEqual, $1, $3);}
    | exp tGT exp           {$$ = makeEXP_binary(k_expressionKind_GT, $1, $3);}
    | exp tGTE exp          {$$ = makeEXP_binary(k_expressionKind_GTE, $1, $3);}
    | exp tST exp           {$$ = makeEXP_binary(k_expressionKind_ST, $1, $3);}
    | exp tSTE exp          {$$ = makeEXP_binary(k_expressionKind_STE, $1, $3);}
    | exp tLOGIC_OR exp     {$$ = makeEXP_binary(k_expressionKind_logicOr, $1, $3);}
    | exp tLOGIC_AND exp    {$$ = makeEXP_binary(k_expressionKind_logicAnd, $1, $3);}
    | tIDENTIFIER           {$$ = makeEXP_identifier($1);}
    | tBOOL_LITERAL         {$$ = makeEXP_boolLiteral($1);}
    | tSTRING_LITERAL       {$$ = makeEXP_stringLiteral($1);}
    | tINT_LITERAL          {$$ = makeEXP_intLiteral($1);}
    | tFLOAT_LITERAL        {$$ = makeEXP_floatLiteral($1);}
    ;

type: tBOOL     {$$ = makeTYPE(t_bool);}
    | tSTRING   {$$ = makeTYPE(t_string);}
    | tINT      {$$ = makeTYPE(t_int);}
    | tFLOAT    {$$ = makeTYPE(t_float);}
    ;
%%
