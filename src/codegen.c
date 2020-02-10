#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "codegen.h"

#define SCANF_BUFFSIZE 1024
#define BUILTIN_CONCAT "__builtin_concat"
#define BUITLIN_READBOOL "__builtin_readBool"
#define BUILTIN_PRINTBOOL "__builtin_printBool"

FILE *targetFile;
int num_tabs_gen = 0;

void genSTMT(STMT *s);
void genIFSTMT(IFSTMT *s);
void genSTMT_LIST(STMT_LIST *s);
void genEXP(EXP *e);
void genTYPE(Type t);

void print_tabs_gen();
char *choosePrintfSpecifier(Type t);
char *chooseScanfFormat(Type t);
char *str_replace(char *orig, char *rep, char *with);

void genBuiltin_concat();
void genBuiltin_readBool();
void genBuiltin_printBool();
void genSTMT_readBool(char *id);

void genProgram(STMT_LIST *root, char *name)
{   
    char *minilang_suffix = ".min";
    char *c_suffix = ".c";
    char *filename = str_replace(name, minilang_suffix, c_suffix);
    char *headers = "#include <stdbool.h>\n#include <stdlib.h>\n#include <stdio.h>\n#include <string.h>\n";

    targetFile = fopen(filename, "w");

    if (root != NULL)
    {
        if (root->currentStmt != NULL)
        {
            fprintf(targetFile, headers);
            genBuiltin_concat();
            genBuiltin_readBool();
            genBuiltin_printBool();
            fprintf(targetFile, "\nvoid main()\n{\n");

            num_tabs_gen++;
            genSTMT(root->currentStmt);
            genSTMT_LIST(root->nextStmtList);

            // print_tabs_gen();
            // fprintf(targetFile, "return 0;\n");
            fprintf(targetFile, "\n}\n");
        }
    }
}

void genSTMT_LIST(STMT_LIST *s)
{
    if (s != NULL)
    {
        genSTMT(s->currentStmt);
        genSTMT_LIST(s->nextStmtList);
    }
}

void genSTMT_readBool(char *id)
{
    fprintf(targetFile, "%s(&%s);\n", BUITLIN_READBOOL, id);
}

void genSTMT_printBool(EXP *e)
{
    fprintf(targetFile, "%s(", BUILTIN_PRINTBOOL);
    genEXP(e);
    fprintf(targetFile, ");\n");
}

void genSTMT(STMT *s)
{
    char *specifier;
    char *scanf_format;
    print_tabs_gen();
    switch (s->kind)
    {
        case k_statementKind_read:
            scanf_format = chooseScanfFormat(s->val.read.type);
            if (s->val.read.type == t_bool)
            {
                genSTMT_readBool(s->val.read.identifier);
            }
            else 
            {
                fprintf(targetFile, "scanf(\"%s\", &%s);\n", scanf_format, s->val.read.identifier);
            }
            break;

        case k_statementKind_print:

            if (s->val.print.exp->type == t_bool) genSTMT_printBool(s->val.print.exp); 
            else
            {
                specifier = choosePrintfSpecifier(s->val.print.exp->type);
                fprintf(targetFile, "printf(\"%s\\n\", ", specifier);
                genEXP(s->val.print.exp);
                fprintf(targetFile, ");\n");
            }
            break;

        case k_statementKind_assignment:

            fprintf(targetFile, " %s = ", s->val.assignment.identifier);
            genEXP(s->val.assignment.exp);
            fprintf(targetFile, ";\n");
            break;

        case k_statementKind_initStrictType:

            genTYPE(s->val.initStrictType.type);
            fprintf(targetFile, " %s = ", s->val.initStrictType.identifier);
            genEXP(s->val.initStrictType.exp);
            fprintf(targetFile, ";\n");
            break;

        case k_statementKind_initLoose:

            genTYPE(s->val.initLooseType.exp->type);
            fprintf(targetFile, " %s = ", s->val.initLooseType.identifier);
            genEXP(s->val.initLooseType.exp);
            fprintf(targetFile, ";\n");
            break;

        case k_statementKind_whileLoop:

            fprintf(targetFile, "while (");
            genEXP(s->val.whileLoop.exp);
            fprintf(targetFile, ")\n");
            print_tabs_gen();
            fprintf(targetFile, "{\n");

            num_tabs_gen++;
            genSTMT_LIST(s->val.whileLoop.stmtList);
            num_tabs_gen--;

            print_tabs_gen();
            fprintf(targetFile, "}\n");
            break;

        case k_statementKind_ifStmt:

            genIFSTMT(s->val.ifStmt);
            break;
    }
}

void genIFSTMT(IFSTMT *s)
{   

    switch (s->kind)
    {
        case k_ifStatementKind_if:

            fprintf(targetFile, "if (");
            genEXP(s->val.ifStmt.exp);
            fprintf(targetFile, ")\n");

            print_tabs_gen();
            fprintf(targetFile, "{\n");

            num_tabs_gen++;
            genSTMT_LIST(s->val.ifStmt.ifPart);
            num_tabs_gen--;

            print_tabs_gen();
            fprintf(targetFile, "}\n");
            break;

        case k_ifStatementKind_ifElse:

            fprintf(targetFile, "if (");
            genEXP(s->val.ifElse.exp);
            fprintf(targetFile, ")\n");

            print_tabs_gen();
            fprintf(targetFile, "{\n");
            num_tabs_gen++;
            genSTMT_LIST(s->val.ifElse.ifPart);
            num_tabs_gen--;
            print_tabs_gen();
            fprintf(targetFile, "}\n");

            print_tabs_gen();
            fprintf(targetFile, "else {\n");
            num_tabs_gen++;
            genSTMT_LIST(s->val.ifElse.elsePart);
            num_tabs_gen--;
            print_tabs_gen();
            fprintf(targetFile, "}\n");
            break;

        case k_ifStatementKind_ifElseIf:

            fprintf(targetFile, "if (");
            genEXP(s->val.ifElseIf.exp);
            fprintf(targetFile, ")\n");

            print_tabs_gen();
            fprintf(targetFile, "{\n");
            num_tabs_gen++;
            genSTMT_LIST(s->val.ifElseIf.ifPart);
            num_tabs_gen--;
            print_tabs_gen();
            fprintf(targetFile, "}\n");

            print_tabs_gen();
            fprintf(targetFile, "else ");
            genIFSTMT(s->val.ifElseIf.ifStmt);
            break;
    }
}

void genEXP_stringConcat(EXP *left, EXP *right)
{
    fprintf(targetFile, "%s(", BUILTIN_CONCAT);
    genEXP(left);
    fprintf(targetFile, ", ");
    genEXP(right);
    fprintf(targetFile, ")");
}

void genEXP(EXP *e)
{
    char *literal;
    switch (e->kind)
    {
        case k_expressionKind_identifier:
            fprintf(targetFile, "%s", e->val.identifier);
            break;
        case k_expressionKind_intLiteral:
            fprintf(targetFile, "%d", e->val.intLiteral);
            break;
        case k_expressionKind_floatLiteral:
            fprintf(targetFile, "%f", e->val.floatLiteral);
            break;
        case k_expressionKind_stringLiteral:
            if (strstr(e->val.stringLiteral, "\\") != NULL) literal = str_replace(e->val.stringLiteral, "\\", "\\\\");
            else literal = e->val.stringLiteral;
            fprintf(targetFile, "%s", literal);
            break;
        case k_expressionKind_boolLiteral:
            if (e->val.boolLiteral) fprintf(targetFile, "true");
            else fprintf(targetFile, "false");
            break;
        case k_expressionKind_addition:
            if (e->type == t_string)
            {
                genEXP_stringConcat(e->val.binary.left, e->val.binary.right);
            }
            else
            {
                genEXP(e->val.binary.left);
                fprintf(targetFile, " + ");
                genEXP(e->val.binary.right);
            }
            break;

        case k_expressionKind_subtraction:
            genEXP(e->val.binary.left);
            fprintf(targetFile, " - ");
            genEXP(e->val.binary.right);
            break;
        case k_expressionKind_multiplication:
            genEXP(e->val.binary.left);
            fprintf(targetFile, " * ");
            genEXP(e->val.binary.right);
            break;
        case k_expressionKind_division:
            genEXP(e->val.binary.left);
            fprintf(targetFile, " / ");
            genEXP(e->val.binary.right);
            break;
        case k_expressionKind_unaryMinus:
            fprintf(targetFile, "-");
            genEXP(e->val.unary);
            break;
        case k_expressionKind_unaryLogicNot:
            fprintf(targetFile, "!");
            genEXP(e->val.unary);
            break;
        case k_expressionKind_equal:
            genEXP(e->val.binary.left);
            fprintf(targetFile, " == ");
            genEXP(e->val.binary.right);
            break;
        case k_expressionKind_notEqual:
            genEXP(e->val.binary.left);
            fprintf(targetFile, " != ");
            genEXP(e->val.binary.right);
            break;
        case k_expressionKind_GT:
            genEXP(e->val.binary.left);
            fprintf(targetFile, " > ");
            genEXP(e->val.binary.right);
            break;
        case k_expressionKind_GTE:
            genEXP(e->val.binary.left);
            fprintf(targetFile, " >= ");
            genEXP(e->val.binary.right);
            break;
        case k_expressionKind_ST:
            genEXP(e->val.binary.left);
            fprintf(targetFile, " < ");
            genEXP(e->val.binary.right);
            break;
        case k_expressionKind_STE:
            genEXP(e->val.binary.left);
            fprintf(targetFile, " <= ");
            genEXP(e->val.binary.right);
            break;
        case k_expressionKind_logicOr:
            genEXP(e->val.binary.left);
            fprintf(targetFile, " || ");
            genEXP(e->val.binary.right);
            break;
        case k_expressionKind_logicAnd:
            genEXP(e->val.binary.left);
            fprintf(targetFile, " && ");
            genEXP(e->val.binary.right);
            break;
        case k_expressionKind_withParantheses:
            fprintf(targetFile, "(");
            genEXP(e->val.unary);
            fprintf(targetFile, ")");
            break;
    }
}

void genTYPE(Type t)
{
    switch (t)
    {
        case t_bool:
            fprintf(targetFile, "bool");
            break;
        case t_string:
            fprintf(targetFile, "char *");
            break;
        case t_int:
            fprintf(targetFile, "int");
            break;
        case t_float:
            fprintf(targetFile, "double");
            break;
    }
}

void genBuiltin_concat()
{
    fprintf(targetFile, "char *%s(char *str1, char *str2)\n", BUILTIN_CONCAT);
    char *code = 
    "{\n"
    "\tchar *res = malloc(sizeof(str1)+sizeof(str2));\n"
    "\tstrcat(res, str1);\n"
    "\tstrcat(res, str2);\n"
    "\treturn res;\n"
    "}\n";
    fprintf(targetFile, code);
}

void genBuiltin_printBool()
{
    fprintf(targetFile, "void %s(bool b)\n{\n", BUILTIN_PRINTBOOL);
    char *code = 
    "\tif (b) printf(\"%%s\", \"True\");\n"
    "\telse printf(\"%%s\", \"False\");\n"
    "}\n";
    fprintf(targetFile, code);
}

void genBuiltin_readBool()
{   
    char *format = chooseScanfFormat(t_bool);
    fprintf(targetFile, "void %s(bool *b)\n{\n", BUITLIN_READBOOL);
    
    num_tabs_gen++;
    print_tabs_gen();
    fprintf(targetFile, "char *__bool_str_tmp = malloc(2);\n");
    
    print_tabs_gen();
    fprintf(targetFile, "scanf(\"%s\", __bool_str_tmp);\n", format);

    print_tabs_gen();
    fprintf(targetFile,"if (strcmp(__bool_str_tmp, \"True\") == 0) *b = true;\n");

    print_tabs_gen();
    fprintf(targetFile,"else if (strcmp(__bool_str_tmp, \"False\") == 0) *b = false;\n");
    
    print_tabs_gen();
    fprintf(targetFile,"else {fprintf(stderr, \"Error: invalid value for type bool\"); exit(EXIT_FAILURE);}\n");

    print_tabs_gen();
    fprintf(targetFile,"free(__bool_str_tmp);\n");
    fprintf(targetFile, "}\n");
    num_tabs_gen--;
}

char *chooseScanfFormat(Type t)
{
    char *format = malloc(2);
    switch (t)
    {
        case t_int:
            sprintf(format, "%%%dd", SCANF_BUFFSIZE);
            return format;
            break;
        case t_float:
            sprintf(format, "%%%dlf", SCANF_BUFFSIZE);
            return format;
            break;
        case t_bool:
        case t_string:
            sprintf(format, "%%%ds", SCANF_BUFFSIZE);
            return format;
            break;
    }
}

char *choosePrintfSpecifier(Type t)
{
    switch (t)
    {
        case t_bool:
            return "%s";
            break;
        case t_int:
            return "%d";
            break;
        case t_string:
            return "%s";
            break;
        case t_float:
            return "%f";
            break;
    }
}

void print_tabs_gen()
{
    for (int i = 0; i < num_tabs_gen; i++)
    {
        fprintf(targetFile, "\t");
    }
}

char *str_replace(char *orig, char *rep, char *with)
{
    char *result;
    char *ins;
    char *tmp;
    int len_rep;
    int len_with;
    int len_front;
    int count;

    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}
