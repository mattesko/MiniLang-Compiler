#include <stdio.h>
#include <string.h>
#include "pretty.h"

extern int print_token;
extern int yylex();
extern int yyparse();

STMT_LIST *root;

void run_mode(char *mode);
char* parse_args(int argc, char *argv[]);

int main(int argc, char *argv[]) {

    char *mode = parse_args(argc, argv);
    run_mode(mode);
    return 0;
}

/* Parse input arguments to retrieve the program's mode
    Args:
        int argc: number of arguments
        char *argv[]: array of arguments
    Returns:
        char *: mode to run compiler in
*/
char* parse_args(int argc, char *argv[]) {
    if (argc == 2) {
        return argv[1];
    }
    else {
        fprintf(stderr, "Error: Incorrect number of arguments");
    }
}

/* Run the compiler in a given mode
    Args:
        char *mode: mode to run the compiler in
*/
void run_mode(char *mode) {
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
    else {
        printf("No such mode '%s' supported", mode);
    }
}
