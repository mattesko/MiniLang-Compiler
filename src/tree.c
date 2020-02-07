#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"

extern int yylineno;

// STMT *makeSTMT_
// {
//     STMT *s = malloc(sizeof(STMT));
//     s->lineno = yylineno;
//     s->kind = 
//     s->val
//     return s;
// }

PROGRAM *makePROGRAM(STMT_LIST *stmtList)
{
    PROGRAM *p = malloc(sizeof(PROGRAM));
    p->lineno = yylineno;
    p->stmtList = stmtList;
    return p;
}

STMT_LIST *makeSTMT_LIST(STMT *currentStmt, STMT_LIST *nextStmtList)
{
    STMT_LIST *s = malloc(sizeof(STMT_LIST));
    s->lineno = yylineno;
    s->currentStmt = currentStmt;
    s->nextStmtList = nextStmtList;
    return s;
}

STMT *makeSTMT_read(char *identifier)
{
    STMT *s = malloc(sizeof(STMT));
    s->lineno = yylineno;
    s->kind = k_statementKind_read;
    s->val.read.identifier = identifier;
    return s;
}

STMT *makeSTMT_print(EXP *exp)
{
    STMT *s = malloc(sizeof(STMT));
    s->lineno = yylineno;
    s->kind = k_statementKind_print;
    s->val.print.exp = exp;
    return s;
}

STMT *makeSTMT_assignment(char *identifier, EXP *exp)
{
    STMT *s = malloc(sizeof(STMT));
    s->lineno = yylineno;
    s->kind = k_statementKind_assignment;
    s->val.assignment.identifier = identifier;
    s->val.assignment.exp = exp;
    return s;
}

STMT *makeSTMT_initStrictType(char *identifier, TYPE *type, EXP *exp)
{
    STMT *s = malloc(sizeof(STMT));
    s->lineno = yylineno;
    s->kind = k_statementKind_initStrictType;
    s->val.initStrictType.identifier = identifier;
    s->val.initStrictType.type = type;
    s->val.initStrictType.exp = exp;
    return s;
}

STMT *makeSTMT_initLooseType(char *identifier, EXP *exp)
{
    STMT *s = malloc(sizeof(STMT));
    s->lineno = yylineno;
    s->kind = k_statementKind_initLoose;
    s->val.initLooseType.identifier = identifier;
    s->val.initLooseType.exp = exp;
    return s;
}

STMT *makeSTMT_declaration(char *identifier, TYPE *type)
{
    STMT *s = malloc(sizeof(STMT));
    s->lineno = yylineno;
    s->kind = k_statementKind_declaration;
    s->val.declaration.identifier = identifier;
    s->val.declaration.type = type;
    return s;
}

STMT *makeSTMT_whileLoop(EXP *exp, STMT_LIST *stmtList)
{
    STMT *s = malloc(sizeof(STMT));
    s->lineno = yylineno;
    s->kind = k_statementKind_whileLoop;
    s->val.whileLoop.exp = exp;
    s->val.whileLoop.stmtList = stmtList;
    return s;
}

// EXP *makeEXP_()
// {
//     EXP *e = malloc(sizeof(EXP));
//     e->lineno = yylineno;
//     e->kind = 
//     e->val = 
//     return e;
// }

EXP *makeEXP_identifier(char *identifier)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionKind_identifier;
	e->val.identifier = identifier;
	return e;
}

EXP *makeEXP_boolLiteral(bool boolLiteral)
{
    EXP *e = malloc(sizeof(EXP));
    e->lineno = yylineno;
    e->kind = k_expressionKind_boolLiteral;
    e->val.boolLiteral = boolLiteral;
    return e;
}

EXP *makeEXP_stringLiteral(char *stringLiteral)
{
    EXP *e = malloc(sizeof(EXP));
    e->lineno = yylineno;
    e->kind = k_expressionKind_stringLiteral;
    e->val.stringLiteral = stringLiteral;
    return e;
}

EXP *makeEXP_intLiteral(int intLiteral) 
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = k_expressionKind_intLiteral;
    e->val.intLiteral = intLiteral;
    return e;
}

EXP *makeEXP_floatLiteral(float floatLiteral)
{
    EXP *e = malloc(sizeof(EXP));
    e->lineno = yylineno;
    e->kind = k_expressionKind_floatLiteral;
    e->val.floatLiteral = floatLiteral;
    return e;
}

EXP *makeEXP_binary(ExpressionKind kind, EXP *left, EXP *right)
{
    EXP *e = malloc(sizeof(EXP));
    e->lineno = yylineno;
    e->kind = kind;
    e->val.binary.left = left;
    e->val.binary.right = right;
    return e;
}

EXP *makeEXP_unary(ExpressionKind kind, EXP *unary)
{
    EXP *e = malloc(sizeof(EXP));
    e->lineno = yylineno;
    e->kind = kind;
    e->val.unary = unary;
    return e;
}

IFSTMT *makeIFSTMT_if(EXP *exp, STMT_LIST *ifPart) 
{
    IFSTMT *c = malloc(sizeof(IFSTMT));
    c->lineno = yylineno;
    c->kind = k_ifStatementKind_if;
    c->val.ifStmt.exp = exp;
    c->val.ifStmt.ifPart = ifPart;
    return c;
}

IFSTMT *makeIFSTMT_ifElse(EXP *exp, STMT_LIST *ifPart, STMT_LIST *elsePart)
{
    IFSTMT *c = malloc(sizeof(IFSTMT));
    c->lineno = yylineno;
    c->kind = k_ifStatementKind_ifElse;
    c->val.ifElse.exp = exp;
    c->val.ifElse.ifPart = ifPart;
    c->val.ifElse.elsePart = elsePart;
    return c;
}

IFSTMT *makeIFSTMT_ifElseIf(EXP *exp, STMT_LIST *ifPart, IFSTMT *elsePart)
{
    IFSTMT *c = malloc(sizeof(IFSTMT));
    c->lineno = yylineno;
    c->kind = k_ifStatementKind_ifElseIf;
    c->val.ifElseIf.exp = exp;
    c->val.ifElseIf.ifPart = ifPart;
    c->val.ifElseIf.elsePart = elsePart;
    return c;
}

TYPE *makeTYPE(TypeKind kind) 
{
    TYPE *t = malloc(sizeof(TYPE));
    t->lineno = yylineno;
    t->kind = kind;
    return t;
}
