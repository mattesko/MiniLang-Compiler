/* Code section */
%{
#include "minic.tab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int print_token = 0;
%}

/* Regular Expressions */
DIGIT [0-9]
ALPHA_CHAR [a-zA-Z]
ALPHA_NUM_CHAR [a-zA-Z0-9]

/* Scanner options */
%option header-file="lex.yy.h"
%option yylineno

/* Tokenization Specifications */
%%
[\r\n]+
[ \t]+      if (print_token) {printf("WHITESPACE, length %u\n", yyleng);}

"+"         if (print_token) {printf("PLUS\n");} return '+';
"-"         if (print_token) {printf("tMINUS\n");} return '-';
"=="        if (print_token) {printf("tEQUAL\n");} return tEQUAL;
"="         if (print_token) {printf("tASSIGN\n");} return '=';
"!="        if (print_token) {printf("tNEQUAL\n");} return tNEQUAL;
">="        if (print_token) {printf("tGTE\n");} return tGTE;
"<="        if (print_token) {printf("tSTE\n");} return tSTE;
">"         if (print_token) {printf("tGT\n");} return tGT;
"<"         if (print_token) {printf("tST\n");} return tST;
"&&"        if (print_token) {printf("tLOGIC_AND\n");} return tLOGIC_AND;
"||"        if (print_token) {printf("tLOGIC_OR\n");} return tLOGIC_OR;
"!"			if (print_token) {printf("tEXCLAMATION_POINT\n");} return '!';
"*"         if (print_token) {printf("tASTERISK\n");} return '*';
"/"         if (print_token) {printf("tSLASH\n");} return '/';

"("         if (print_token) {printf("tOPEN_PAR\n");} return '(';
")"         if (print_token) {printf("tCLOSE_PAR\n");} return ')';
"{"			if (print_token) {printf("tOPEN_BRACE\n");} return '{';
"}"			if (print_token) {printf("tCLOSE_BRACE\n");} return '}';
";"         if (print_token) {printf("tSEMICOLON\n");} return ';';
":"         if (print_token) {printf("tCOLON\n");} return ':';

"var"		if (print_token) {printf("tVAR\n");} return tVAR;
"float"		if (print_token) {printf("tFLOAT\n");} return tFLOAT;
"int"		if (print_token) {printf("tINT\n");} return tINT;
"bool"		if (print_token) {printf("tBOOL\n");} return tBOOL;
"string"	if (print_token) {printf("tSTRING\n");} return tSTRING;
"print"		if (print_token) {printf("tPRINT\n");} return tPRINT;
"while"		if (print_token) {printf("tWHILE\n");} return tWHILE;
"if"		if (print_token) {printf("tIF\n");} return tIF;
"else"		if (print_token) {printf("tELSE\n");} return tELSE;
"read"		if (print_token) {printf("tREAD\n");} return tREAD;

"True"|"False" {
		if (print_token) {printf("tBOOL_LITERAL: %s\n", yytext);}
		if (strcmp("True", yytext) == 0) {yylval.bool_val = 0;}
		else {yylval.bool_val = 1;}
		return tBOOL_LITERAL;
}

\"[^"¿]*\" {
		if (print_token) {printf("tSTRING_LITERAL: %s\n", yytext);}
		yylval.string_val = strdup(yytext);
		return tSTRING_LITERAL;
}

#.* {
        if (print_token) {printf("COMMENT: %s\n", yytext);}
}

0|([1-9]{DIGIT}*) {
	    if (print_token) {printf("tINT_LITERAL: %s\n", yytext);}
		yylval.int_val = atoi(yytext);
		return tINT_LITERAL;
}

(([1-9]{DIGIT}*)|0)\.{DIGIT}+ {
	    if (print_token) {printf("tFLOAT_LITERAL: %s\n", yytext);}
		yylval.float_val = atof(yytext);
		return tFLOAT_LITERAL;
}

(_|[A-Za-z])[A-Za-z0-9_]* {
	    if (print_token) {printf("tIDENTIFIER: %s\n", yytext);}
		yylval.string_val = strdup(yytext);
		return tIDENTIFIER;
}

. { 
		fprintf (stderr, "Error: lexical error (line %d) unexpected character '%s'\n", yylineno, yytext); exit(1); 
}
%%
