/* Code section */
%{
#include "minic.tab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// #define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;
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
"-"         if (print_token) {printf("MINUS\n");} return '-';
"=="        if (print_token) {printf("COMPARE\n");} return COMPARE;
"="         if (print_token) {printf("ASSIGN\n");} return '=';
"!="        if (print_token) {printf("NOT_COMPARE\n");} return NOT_COMPARE;
">="        if (print_token) {printf("GTE\n");} return GTE;
"<="        if (print_token) {printf("STE\n");} return STE;
">"         if (print_token) {printf("GT\n");} return GT;
"<"         if (print_token) {printf("ST\n");} return ST;
"&&"        if (print_token) {printf("LOGIC_AND\n");} return LOGIC_AND;
"||"        if (print_token) {printf("LOGIC_OR\n");} return LOGIC_OR;
"*"         if (print_token) {printf("ASTERISK\n");} return '*';
"/"         if (print_token) {printf("SLASH\n");} return '/';

"("         if (print_token) {printf("OPEN_PAR\n");} return '(';
")"         if (print_token) {printf("CLOSE_PAR\n");} return ')';
"{"			if (print_token) {printf("OPEN_BRACE\n");} return '{';
"}"			if (print_token) {printf("CLOSE_BRACE\n");} return '}';
";"         if (print_token) {printf("SEMICOLON\n");} return ';';
":"         if (print_token) {printf("COLON\n");} return ':';

"var"		if (print_token) {printf("VAR\n");} return VAR;
"float"		if (print_token) {printf("FLOAT\n");} return FLOAT;
"int"		if (print_token) {printf("INT\n");} return INT;
"bool"		if (print_token) {printf("BOOL\n");} return BOOL;
"string"	if (print_token) {printf("STRING\n");} return STRING;
"print"		if (print_token) {printf("PRINT\n");} return PRINT;
"while"		if (print_token) {printf("WHILE\n");} return WHILE;
"if"		if (print_token) {printf("IF\n");} return IF;
"else"		if (print_token) {printf("ELSE\n");} return ELSE;
"read"		if (print_token) {printf("READ\n");} return READ;

"True"|"False" {
		if (print_token) {printf("BOOL_VAL\n");}
		if (strcmp("True", yytext) == 0) {yylval.bool_val = 0;}
		else {yylval.bool_val = 1;}
		return BOOL_VAL;
}

\".*\" {
		if (print_token) {printf("STRING_VAL: %s\n", yytext);}
		yylval.string_val = strdup(yytext);
		return STRING_VAL;
}

#.* {
        if (print_token) {printf("COMMENT: %s\n", yytext);}
}

0|([1-9]{DIGIT}*) {
	    if (print_token) {printf("INT_VAL: %s\n", yytext);}
		yylval.int_val = atoi(yytext);
		return INT_VAL;
}

(([1-9]{DIGIT}*)|0)\.{DIGIT}+ {
	    if (print_token) {printf("FLOAT_VAL: %s\n", yytext);}
		yylval.float_val = atof(yytext);
		return FLOAT_VAL;
}

(_|[A-z])[A-z0-9_]* {
	    if (print_token) {printf("IDENTIFIER: %s\n", yytext);}
		yylval.identifier = strdup(yytext);
		return IDENTIFIER;
}

. { 
		fprintf (stderr, "Error: (line %d) unexpected character '%s'\n", yylineno, yytext); exit(1); 
}
%%
