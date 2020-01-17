/* Code section */
%{
#include <stdio.h>
%}

/* Helper definitions */
DIGIT [0-9]
ALPHA_CHAR [a-zA-Z]
ALPHA_NUM_CHAR [a-zA-Z0-9]

/* Scanner options */
%option yylineno

%%
[\r\n]+
[ \t]+      printf("WHITESPACE, length %lu\n", yyleng);

"+"         printf("PLUS\n");
"-"         printf("MINUS\n");
"=="        printf("COMPARISON\n");
"="         printf("ASSIGNMENT\n");
"!="        printf("NEGATE_COMPARISON\n");
">="        printf("GREATER_THAN_EQUAL\n");
"<="        printf("SMALLER_THAN_EQUAL\n");
">"         printf("GREATER_THAN\n");
"<"         printf("SMALLER_THAN\n");
"&&"        printf("LOGICAL_AND\n");
"||"        printf("LOGICAL_OR\n");
"*"         printf("TIMES\n");
"/"         printf("DIVIDE\n");

"("         printf("LEFT_PARANTHESIS\n");
")"         printf("RIGHT_PARANTHESIS\n");
"{"			printf("LEFT_CURLY_BRACKET\n");
"}"			printf("RIGHT_CURLY_BRACKET\n");

"True"      printf("TRUE_BOOL_LITERAL\n");
"False"     printf("FALSE_BOOL_LITERAL\n");

";"         printf("SEMI_COLON\n");
":"         printf("COLON\n");

"var"		printf("DECLARATION\n");
"float"		printf("FLOAT\n");
"int"		printf("INT\n");
"bool"		printf("BOOL\n");
"string"	printf("STRING\n");
"print"		printf("PRINT\n");
"while"		printf("WHILE\n");
"if"		printf("IF\n");
"else"		printf("ELSE\n");
"read"		printf("READ\n");

\".*\" {
        printf("STRING_LITERAL: %s\n", yytext);
}

#.* {
        printf("COMMENT: %s\n", yytext);
}

0|([1-9]{DIGIT}*) {
	    printf("INT_LITERAL: %s\n", yytext);
}

{DIGIT}+\.{DIGIT}+ {
	    printf("FLOAT_LITERAL: %s\n", yytext);
}

_?{ALPHA_CHAR}+[{ALPHA_NUM_CHAR}_]* {
	    printf("IDENTIFIER: %s\n", yytext);
}

. { fprintf (stderr, "Error: (line %d) unexpected character '%s'\n", yylineno, yytext); exit(1); 
}
%%

int main()
{
	yylex();
    printf("OK\n");
	return 0;
}
