#include <stdio.h>
#include <string.h>

extern int print_token;
extern yylex();
extern yyparse();

void run_mode(char *mode);
char* parse_args(int argc, char *argv[]);

int main(int argc, char *argv[]) {

    char *mode = parse_args(argc, argv);
    run_mode(mode);
    return 0;
}

char* parse_args(int argc, char *argv[]) {
    if (argc == 1) {
        return "default";
    }
    else if (argc == 2) {
        return argv[1];
    }
    else if (argc > 2) {
        printf("Too many arguments");
        return "";
    }
}

void run_mode(char *mode) {
    if (strcmp(mode, "scan") == 0) {
        print_token = 0;
        while(yylex());
        printf("OK\n");
    }
    else if (strcmp(mode, "tokens") == 0) {
        print_token = 1;
        while(yylex());
    }
    else if (strcmp(mode, "parse") == 0) {
        yyparse();
        printf("OK\n");
    }
    else if (strcmp(mode, "default") == 0) {
        print_token = 1;
        while(yylex());
        printf("OK\n");
    }
    else {
        printf("No such mode '%s' supported", mode);
    }
}
