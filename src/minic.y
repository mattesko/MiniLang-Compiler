// Code section
%{
// #include "lex.yy.h"
#include <stdio.h>
#include <stdlib.h>

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

%union {
    int int_val;
    char *string_val;
    float float_val;
    unsigned short int bool_val;
}

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

%%
program: stmt_list;

stmt_list:
    | stmt stmt_list
    ;

stmt: tREAD '(' tIDENTIFIER ')' ';'
    | tPRINT '(' exp ')' ';'
    | tIDENTIFIER '=' exp ';'
    | tWHILE '(' exp ')' '{' stmt_list '}'
    | tVAR tIDENTIFIER ':' type '=' exp ';'
    | tVAR tIDENTIFIER '=' exp ';'
    | if_stmt
    ;

if_stmt: tIF '(' exp ')' '{' stmt_list '}'
    | tIF '(' exp ')' '{' stmt_list '}' tELSE '{' stmt_list '}'
    | tIF '(' exp ')' '{' stmt_list '}' tELSE if_stmt
    ;

    | exp tEQUAL exp
    | exp tNEQUAL exp
    | exp tGT exp
    | exp tGTE exp
    | exp tST exp
    | exp tSTE exp
    | exp tLOGIC_OR exp
    | exp tLOGIC_AND exp
    | tIDENTIFIER
    | tBOOL_LITERAL
    | tSTRING_LITERAL
    | tINT_LITERAL
    | tFLOAT_LITERAL
    ;

type: tBOOL
    | tSTRING
    | tINT
    | tFLOAT
    ;
%%
