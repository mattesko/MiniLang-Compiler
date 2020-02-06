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
    s->kind = k_read;
    s->val.read.identifier = identifier;
    return s;
}

STMT *makeSTMT_print(EXP *exp)
{
    STMT *s = malloc(sizeof(STMT));
    s->lineno = yylineno;
    s->kind = k_print;
    s->val.print.exp = exp;
    return s;
}

STMT *makeSTMT_assignment(char *identifier, EXP *exp)
{
    STMT *s = malloc(sizeof(STMT));
    s->lineno = yylineno;
    s->kind = k_assignment;
    s->val.assignment.exp = exp;
    return s;
}

STMT *makeSTMT_initStrictType(char *identifier, TYPE *type, EXP *exp)
{
    STMT *s = malloc(sizeof(STMT));
    s->lineno = yylineno;
    s->kind = k_initStrictType;
    s->val.initStrictType.type = type;
    s->val.initStrictType.exp = exp;
    return s;
}

STMT *makeSTMT_initLooseType(char *identifier, EXP *exp)
{
    STMT *s = malloc(sizeof(STMT));
    s->lineno = yylineno;
    s->kind = k_initLoose;
    s->val.initLooseType.exp = exp;
    return s;
}

STMT *makeSTMT_declaration(char *identifier, TYPE *type)
{
    STMT *s = malloc(sizeof(STMT));
    s->lineno = yylineno;
    s->kind = k_declaration;
    s->val.declaration.type = type;
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
	e->kind = k_identifier;
	e->val.identifier = identifier;
	return e;
}

EXP *makeEXP_boolLiteral(bool boolLiteral)
{
    EXP *e = malloc(sizeof(EXP));
    e->lineno = yylineno;
    e->kind = k_boolLiteral;
    e->val.boolLiteral = boolLiteral;
    return e;
}

EXP *makeEXP_stringLiteral(char *stringLiteral)
{
    EXP *e = malloc(sizeof(EXP));
    e->lineno = yylineno;
    e->kind = k_stringLiteral;
    e->val.stringLiteral = stringLiteral;
    return e;
}

EXP *makeEXP_intLiteral(int intLiteral) 
{
    EXP *e = malloc(sizeof(EXP));
    e->kind = k_intLiteral;
    e->val.intLiteral = intLiteral;
    return e;
}

EXP *makeEXP_floatLiteral(float floatLiteral)
{
    EXP *e = malloc(sizeof(EXP));
    e->lineno = yylineno;
    e->kind = k_floatLiteral;
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

CTRL_FLOW *makeCTRL_FLOW_if(EXP *exp, STMT_LIST *ifPart) 
{
    CTRL_FLOW *c = malloc(sizeof(CTRL_FLOW));
    c->lineno = yylineno;
    c->kind = k_if;
    c->val.ifStmt.exp = exp;
    c->val.ifStmt.ifPart = ifPart;
    return c;
}

CTRL_FLOW *makeCTRL_FLOW_ifElse(EXP *exp, STMT_LIST *ifPart, STMT_LIST *elsePart)
{
    CTRL_FLOW *c = malloc(sizeof(CTRL_FLOW));
    c->lineno = yylineno;
    c->kind = k_ifElse;
    c->val.ifElse.exp = exp;
    c->val.ifElse.ifPart = ifPart;
    c->val.ifElse.elsePart = elsePart;
    return c;
}

CTRL_FLOW *makeCTRL_FLOW_ifElseIf(EXP *exp, STMT_LIST *ifPart, CTRL_FLOW *elsePart)
{
    CTRL_FLOW *c = malloc(sizeof(CTRL_FLOW));
    c->lineno = yylineno;
    c->kind = k_ifElse;
    c->val.ifElseIf.exp = exp;
    c->val.ifElseIf.ifPart = ifPart;
    c->val.ifElseIf.elsePart = elsePart;
    return c;
}

CTRL_FLOW *makeCTRL_FLOW_while(EXP *exp, STMT_LIST *stmtList)
{
    CTRL_FLOW *c = malloc(sizeof(CTRL_FLOW));
    c->lineno = yylineno;
    c->kind = k_while;
    c->val.whileLoop.exp = exp;
    c->val.whileLoop.stmtList = stmtList;
    return c;
}

TYPE *makeTYPE(TypeKind kind) 
{
    TYPE *t = malloc(sizeof(TYPE));
    t->lineno = yylineno;
    t->kind = kind;
    return t;
}
