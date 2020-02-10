#include <stdio.h>
#include "tree.h"
int num_tabs = 0;

void prettySTMT(STMT *s);
void prettySTMT_LIST(STMT_LIST *s);
void prettyEXP(EXP *e);
void prettyTYPE(Type t);
void prettyIFSTMT(IFSTMT *s);
void print_tabs();


void prettySTMT_LIST(STMT_LIST *s)
{   
    if (s != NULL)
    {
        prettySTMT(s->currentStmt);
        prettySTMT_LIST(s->nextStmtList);
    }
}

void prettySTMT(STMT *s)
{   
    print_tabs();
    switch (s->kind)
    {
        case k_statementKind_read:
            printf("read(%s);\n", s->val.read.identifier);
            break;
        case k_statementKind_print:
            printf("print(");
            prettyEXP(s->val.print.exp);
            printf(");\n");
            break;
        case k_statementKind_assignment:
            printf("%s = ", s->val.assignment.identifier);
            prettyEXP(s->val.assignment.exp);
            printf(";\n");
            break;
        case k_statementKind_initStrictType:
            printf("var %s: ", s->val.initStrictType.identifier);
            prettyTYPE(s->val.initStrictType.type);
            printf(" = ");
            prettyEXP(s->val.initStrictType.exp);
            printf(";\n");
            break;
        case k_statementKind_initLoose:
            printf("var %s = ", s->val.initLooseType.identifier);
            prettyEXP(s->val.initLooseType.exp);
            printf(";\n");
            break;
        case k_statementKind_whileLoop:

            printf("while(");
            prettyEXP(s->val.whileLoop.exp);
            printf(") {\n");

            num_tabs++;
            prettySTMT_LIST(s->val.whileLoop.stmtList);
            num_tabs--;

            printf("}\n");
            break;
        case k_statementKind_ifStmt:
            prettyIFSTMT(s->val.ifStmt);
            break;
    }
}

void prettyIFSTMT(IFSTMT *s)
{   
    switch (s->kind)
    {
        case k_ifStatementKind_if:
            printf("if (");
            prettyEXP(s->val.ifStmt.exp);
            printf(") {\n");

            num_tabs++;
            prettySTMT_LIST(s->val.ifStmt.ifPart);
            num_tabs--;

            print_tabs();
            printf("}\n");
            break;
        case k_ifStatementKind_ifElse:
            printf("if (");
            prettyEXP(s->val.ifElse.exp);
            printf(")\n{\n");

            num_tabs++;
            prettySTMT_LIST(s->val.ifElse.ifPart);
            num_tabs--;
            printf("}\n");

            printf("else {\n");
            num_tabs++;
            prettySTMT_LIST(s->val.ifElse.elsePart);
            num_tabs--;

            printf("}\n");
            break;
        case k_ifStatementKind_ifElseIf:
            printf("if (");
            prettyEXP(s->val.ifElseIf.exp);
            printf(") {\n");
            num_tabs++;
            prettySTMT_LIST(s->val.ifElseIf.ifPart);
            num_tabs--;
            printf("}\n");
            printf("else ");
            prettyIFSTMT(s->val.ifElseIf.ifStmt);
            break;
    }
}

void prettyEXP(EXP *e)
{   
    switch (e->kind)
    {
        case k_expressionKind_identifier:
            printf("%s", e->val.identifier);
            break;
        case k_expressionKind_intLiteral:
            printf("%d", e->val.intLiteral);
            break;
        case k_expressionKind_floatLiteral:
            printf("%f", e->val.floatLiteral);
            break;
        case k_expressionKind_stringLiteral:
            printf("%s", e->val.stringLiteral);
            break;
        case k_expressionKind_boolLiteral:
            if (e->val.boolLiteral)
            {
                printf("True");
            }
            else
            {
                printf("False");
            }
            break;
        case k_expressionKind_addition:
            prettyEXP(e->val.binary.left);
            printf(" + ");
            prettyEXP(e->val.binary.right);
            break;
        case k_expressionKind_subtraction:
            prettyEXP(e->val.binary.left);
            printf(" - ");
            prettyEXP(e->val.binary.right);
            break;
        case k_expressionKind_multiplication:
            prettyEXP(e->val.binary.left);
            printf(" * ");
            prettyEXP(e->val.binary.right);
            break;
        case k_expressionKind_division:
            prettyEXP(e->val.binary.left);
            printf(" / ");
            prettyEXP(e->val.binary.right);
            break;
        case k_expressionKind_unaryMinus:
            printf("-");
            prettyEXP(e->val.unary);
            break;
        case k_expressionKind_unaryLogicNot:
            printf("!");
            prettyEXP(e->val.unary);
            break;
        case k_expressionKind_equal:
            prettyEXP(e->val.binary.left);
            printf(" == ");
            prettyEXP(e->val.binary.right);
            break;
        case k_expressionKind_notEqual:
            prettyEXP(e->val.binary.left);
            printf(" != ");
            prettyEXP(e->val.binary.right);
            break;
        case k_expressionKind_GT:
            prettyEXP(e->val.binary.left);
            printf(" > ");
            prettyEXP(e->val.binary.right);
            break;
        case k_expressionKind_GTE:
            prettyEXP(e->val.binary.left);
            printf(" >= ");
            prettyEXP(e->val.binary.right);
            break;
        case k_expressionKind_ST:
            prettyEXP(e->val.binary.left);
            printf(" < ");
            prettyEXP(e->val.binary.right);
            break;
        case k_expressionKind_STE:
            prettyEXP(e->val.binary.left);
            printf(" <= ");
            prettyEXP(e->val.binary.right);
            break;
        case k_expressionKind_logicOr:
            prettyEXP(e->val.binary.left);
            printf(" || ");
            prettyEXP(e->val.binary.right);
            break;
        case k_expressionKind_logicAnd:
            prettyEXP(e->val.binary.left);
            printf(" && ");
            prettyEXP(e->val.binary.right);
            break;
        case k_expressionKind_withParantheses:
            printf("(");
            prettyEXP(e->val.unary);
            printf(")");
            break;
    }
}

void prettyTYPE(Type t)
{
    switch (t)
    {
        case t_bool:
            printf("bool");
            break;
        case t_string:
            printf("string");
            break;
        case t_int:
            printf("int");
            break;
        case t_float:
            printf("float");
            break;
    }
}

void print_tabs()
{
    for (int i = 0; i < num_tabs; i++)
    {
        printf("\t");
    }
}