#include <stdio.h>
#include <string.h>
#include "pretty.h"
#include "symbol.h"
#include "codegen.h"

extern int print_token;
extern int print_sym_table;
extern int yylex();
extern int yyparse();

STMT_LIST *root;

void run_mode(char *mode);
char* parse_args(int argc, char *argv[]);

int main(int argc, char *argv[]) {

    char *mode;
    if (argc == 2 || argc == 3) {
        mode = argv[1];
    }
    else {
        fprintf(stderr, "Error: incorrect number of arguments");
    }
    
    if (strcmp(mode, "scan") == 0) {
        print_token = 0;
        while(yylex());
        printf("OK\n");
    }
    else if (strcmp(mode, "tokens") == 0) {
        print_token = 1;
        while(yylex());
        printf("OK\n");
    }
    else if (strcmp(mode, "parse") == 0) {
        yyparse();
        printf("OK\n");
    }
    else if (strcmp(mode, "pretty") == 0) {
        yyparse();
        prettySTMT_LIST(root);
    }
    else if (strcmp(mode, "symbol") == 0) {
        yyparse();
        print_sym_table = 1;
        makeSymbolTable(root);
        printf("=================================================\n");
    }
    else if (strcmp(mode, "typecheck") == 0) {
        yyparse();
        makeSymbolTable(root);
        printf("OK\n");
    }
    else if (strcmp(mode, "codegen") == 0) {
        yyparse();
        makeSymbolTable(root);
        genProgram(root, argv[2]);
        printf("OK\n");
    }
    else {
        fprintf(stderr, "Error: no such mode '%s' supported", mode);
    }

    return 0;
}