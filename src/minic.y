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
    char *identifier;
}

%token VAR FLOAT INT BOOL STRING PRINT WHILE IF ELSE READ
%token GT GTE ST STE COMPARE LOGIC_AND LOGIC_OR NOT_COMPARE

%token <bool_val> BOOL_VAL
%token <string_val> STRING_VAL
%token <int_val> INT_VAL
%token <float_val> FLOAT_VAL
%token <identifier> IDENTIFIER

// Top = Lowest Precedence. Bottom = Highest Precedence
%left LOGIC_OR
%left LOGIC_AND
%left COMPARE NOT_COMPARE
%left GTE STE GT ST
%left '+' '-'
%left '*' '/'
// %nonassoc '{' '}' '(' ')'

%start program

%locations
%define parse.error verbose

%%
program: stmt stmt_list;

stmt_list:
    | stmt stmt_list
    ;

stmt: READ '(' IDENTIFIER ')' ';'
    | PRINT '(' exp ')' ';'
    | IDENTIFIER '=' exp ';'
    | IF '(' exp ')' '{' stmt_list '}'
    | IF '(' exp ')' '{' stmt_list '}' ELSE '{' stmt_list '}'
    | IF '(' exp ')' '{' stmt_list '}' ELSE IF '(' exp ')' '{' stmt_list '}'
    | IF '(' exp ')' '{' stmt_list '}' ELSE IF '(' exp ')' '{' stmt_list '}' ELSE '{' stmt_list '}'
    | WHILE '(' exp ')' '{' stmt_list '}'
    | VAR IDENTIFIER ':' type '=' exp ';'
    | VAR IDENTIFIER '=' exp ';'
    ;

exp:  exp '+' exp
    | exp '-' exp
    | exp '*' exp
    | exp '/' exp
    | '(' exp ')'
    | '-' exp
    | exp COMPARE exp
    | exp NOT_COMPARE exp
    | exp GT exp
    | exp GTE exp
    | exp ST exp
    | exp STE exp
    | exp LOGIC_OR exp
    | exp LOGIC_AND exp
    | IDENTIFIER
    | BOOL_VAL
    | STRING_VAL
    | INT_VAL
    | FLOAT_VAL
    ;

type: BOOL
    | STRING
    | INT
    | FLOAT
    ;
%%
