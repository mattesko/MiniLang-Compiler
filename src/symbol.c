#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symbol.h"
#include "type.h"

#define SCANF_BUFFSIZE 1024

Symbol *putSymbol(SymbolTable *t, char *name, Type type);
Symbol *getSymbol(SymbolTable *t, char *name);
Symbol *getSymbolFromScope(SymbolTable *t, char *name);
SymbolTable *initSymbolTable();
SymbolTable *scopeSymbolTable(SymbolTable *parent);

void symSTMT_LIST(STMT_LIST *s, SymbolTable *table);
void symSTMT(STMT *stmt, SymbolTable *table);
void symIFSTMT(IFSTMT *ifStmt, SymbolTable *table);
Type symEXP(EXP *exp, SymbolTable *table);

void printSymbolTableRow(char *id, SymbolTable *scope, Type t_type);
void throwErrorUndefinedId(int lineno, char *id);
void throwErrorRedeclaredId(char *id);

char *chooseScanfFormat(Type t_type);

int print_sym_table = 0;

void makeSymbolTable(STMT_LIST *root)
{
    SymbolTable *global_scope = initSymbolTable();

    if (print_sym_table)
    {
        printf("================== Symbol Table =================\n");
        printf("|\tName\t|\tScope\t|\tType\t|\n");
        printf("=================================================\n");
    }
    if (root != NULL)
    {
        symSTMT(root->currentStmt, global_scope);
        symSTMT_LIST(root->nextStmtList, global_scope);
    }
}

void symSTMT_LIST(STMT_LIST *s, SymbolTable *table)
{
    if (s == NULL) return;
    symSTMT(s->currentStmt, table);
    symSTMT_LIST(s->nextStmtList, table);
}

void symSTMT(STMT *stmt, SymbolTable *table)
{
    Symbol *sym;
    char *id;
    char *scanf_format;
    Type t_type_exp;
    SymbolTable *innerScope;
    union
    {
        char *s;
        int i;
        float f;
        bool b;
    } read_value;
    

    switch (stmt->kind)
    {
        case k_statementKind_read:

            id = stmt->val.read.identifier;
            sym = getSymbol(table, id);

            if (sym == NULL) throwErrorUndefinedId(stmt->lineno, id);

            scanf_format = chooseScanfFormat(sym->type);

            scanf(scanf_format, &read_value);
            // TODO continue for generated code
            break;

        case k_statementKind_print:
            symEXP(stmt->val.print.exp, table);
            break;

        case k_statementKind_assignment:

            id = stmt->val.assignment.identifier;
            sym = getSymbol(table, id);

            if (sym == NULL) throwErrorUndefinedId(stmt->lineno, id);

            t_type_exp = symEXP(stmt->val.assignment.exp, table);
            checkAssignment(sym->type, t_type_exp, stmt->lineno);
            
            break;

        case k_statementKind_initStrictType:

            id = stmt->val.initStrictType.identifier;

            // Want to allow redeclaring variables if it's in a nested scope
            sym = getSymbolFromScope(table, id);
            if (sym != NULL) throwErrorRedeclaredId(id);

            t_type_exp = symEXP(stmt->val.initStrictType.exp, table);
            checkAssignment(stmt->val.initStrictType.type, t_type_exp, stmt->lineno);

            putSymbol(table, id, t_type_exp);
            if (print_sym_table) printSymbolTableRow(id, table, t_type_exp);
            
            break;

        case k_statementKind_initLoose:

            id = stmt->val.initLooseType.identifier;

            // Want to allow redeclaring variables if it's in a nested scope
            sym = getSymbolFromScope(table, id);
            if (sym != NULL) throwErrorRedeclaredId(id);

            t_type_exp = symEXP(stmt->val.initLooseType.exp, table);

            putSymbol(table, id, t_type_exp);
            if (print_sym_table) printSymbolTableRow(id, table, t_type_exp);

            break;

        case k_statementKind_declaration:

            id = stmt->val.declaration.identifier;
            sym = getSymbol(table, id);

            if (sym != NULL) throwErrorRedeclaredId(id);
            
            // TODO check if var a : int; is valid!!
            // this never happens lol
            // putSymbol(table, id, NULL);
            if (print_sym_table) printSymbolTableRow(id, table, t_type_exp);
            break;

        case k_statementKind_whileLoop:

            t_type_exp = symEXP(stmt->val.whileLoop.exp, table);
            checkIsBool(t_type_exp, stmt->lineno);

            innerScope = scopeSymbolTable(table);
            symSTMT_LIST(stmt->val.whileLoop.stmtList, innerScope);
            break;

        case k_statementKind_ifStmt:

            symIFSTMT(stmt->val.ifStmt, table);
            break;
    }
}

void symIFSTMT(IFSTMT *ifStmt, SymbolTable *table)
{
    Type t_type;
    SymbolTable *scopeIfPart;
    SymbolTable *scopeElsePart;
    switch (ifStmt->kind)
    {
        case k_ifStatementKind_if:

            t_type = symEXP(ifStmt->val.ifStmt.exp, table);
            checkIsBool(t_type, ifStmt->lineno);

            scopeIfPart = scopeSymbolTable(table);
            symSTMT_LIST(ifStmt->val.ifStmt.ifPart, scopeIfPart);

            break;

        case k_ifStatementKind_ifElse:

            t_type = symEXP(ifStmt->val.ifElse.exp, table);
            checkIsBool(t_type, ifStmt->lineno);

            scopeIfPart = scopeSymbolTable(table);
            scopeElsePart = scopeSymbolTable(table);
            symSTMT_LIST(ifStmt->val.ifElse.ifPart, scopeIfPart);
            symSTMT_LIST(ifStmt->val.ifElse.elsePart, scopeElsePart);

            break;

        case k_ifStatementKind_ifElseIf:
            
            t_type = symEXP(ifStmt->val.ifElseIf.exp, table);
            checkIsBool(t_type, ifStmt->lineno);

            scopeIfPart = scopeSymbolTable(table);
            scopeElsePart = scopeSymbolTable(table);
            symSTMT_LIST(ifStmt->val.ifElseIf.ifPart, scopeIfPart);
            symIFSTMT(ifStmt->val.ifElseIf.ifStmt, scopeElsePart);

            break;

    }
}

Type symEXP(EXP *exp, SymbolTable *table)
{
    char *id;
    Symbol *sym;
    Type t_type;
    Type t_type_left;
    Type t_type_right;

    switch (exp->kind)
    {
        case k_expressionKind_identifier:

            id = exp->val.identifier;
            sym = getSymbol(table, exp->val.identifier);
            if (sym == NULL) throwErrorUndefinedId(exp->lineno, id);

            return sym->type;
            break;

        case k_expressionKind_addition:
        case k_expressionKind_subtraction:
        case k_expressionKind_multiplication:
        case k_expressionKind_division:

            t_type_left = symEXP(exp->val.binary.left, table);
            t_type_right = symEXP(exp->val.binary.right, table);

            t_type = resolveBinaryMath(t_type_left, t_type_right, exp->lineno);
            return t_type;
            break;

        case k_expressionKind_unaryMinus:

            t_type = symEXP(exp->val.unary, table);
            checkUnaryMinus(t_type, exp->lineno);
            return t_type;
            break;

        case k_expressionKind_unaryLogicNot:

            t_type = symEXP(exp->val.unary, table);
            checkUnaryLogicNot(t_type, exp->lineno);
            return t_type;
            break;

        case k_expressionKind_equal:
        case k_expressionKind_notEqual:
        case k_expressionKind_GT:
        case k_expressionKind_GTE:
        case k_expressionKind_ST:
        case k_expressionKind_STE:
            t_type_left = symEXP(exp->val.binary.left, table);
            t_type_right = symEXP(exp->val.binary.right, table);
            checkBinaryComparison(t_type_left, t_type_right, exp->lineno);
            return t_bool;
            break;

        case k_expressionKind_logicOr:
        case k_expressionKind_logicAnd:
            t_type_left = symEXP(exp->val.binary.left, table);
            t_type_right = symEXP(exp->val.binary.right, table);
            checkBinaryLogic(t_type_left, t_type_right, exp->lineno);
            return t_bool;
            break;
        case k_expressionKind_withParantheses:
            return symEXP(exp->val.unary, table);
            break;

        // Can directly resolve types from literals
        case k_expressionKind_intLiteral:
            return t_int;
            break;

        case k_expressionKind_floatLiteral:
            return t_float;
            break;

        case k_expressionKind_stringLiteral:
            return t_string;
            break;

        case k_expressionKind_boolLiteral:
            return t_bool;
            break;
    }
}

SymbolTable *initSymbolTable()
{
    SymbolTable *t = malloc(sizeof(SymbolTable));

    for (int i = 0; i < HASH_SIZE; i++)
    {   
        t->table[i] = NULL;
    }
    t->parent = NULL;
    return t;
}

SymbolTable *scopeSymbolTable(SymbolTable *parent)
{
    SymbolTable *scope = initSymbolTable();
    scope->parent = parent;
    return scope;
}

int Hash(char *str)
{
    unsigned int hash = 0;
    while (*str) hash = (hash << 1) + *str++;
    return hash % HASH_SIZE;
}

Symbol *getSymbol(SymbolTable *t, char *name)
{
    int hash = Hash(name);

    for (Symbol *s = t->table[hash]; s; s = s->next) 
    {
        if (strcmp(s->name, name) == 0) return s;
    }

    if (t->parent == NULL) return NULL;
    return getSymbol(t->parent, name);
}

Symbol *getSymbolFromScope(SymbolTable *t, char *name)
{
    int hash = Hash(name);

    for (Symbol *s = t->table[hash]; s; s = s->next) 
    {
        if (strcmp(s->name, name) == 0) return s;
    }
    return NULL;
}

Symbol *putSymbol(SymbolTable *t, char *name, Type type)
{
    int hash = Hash(name);
    for (Symbol *s = t->table[hash]; s; s->next)
    {   
        if (strcmp(s->name, name) == 0) throwErrorRedeclaredId(s->name);
    }

    Symbol *s = malloc(sizeof(Symbol));
    s->name = name;
    s->type = type;
    s->next = t->table[hash];
    t->table[hash] = s;
    return s;
}

char *chooseScanfFormat(Type t_type)
{
    switch (t_type)
    {
        case t_int:
            return "%d";
            break;
        case t_float:
            return "%f";
            break;
        case t_string:
            return "%s";
            break;
        case t_bool:
            return "%s"; // TODO change scanned boolean to type bool
            break;
    }
}

void printSymbolTableRow(char *id, SymbolTable *scope, Type t_type)
{
    char *type = typeToString(t_type);
    printf("|%s\t\t|%p\t|%s\t\t|\n", id, scope, type);
}

void throwErrorUndefinedId(int lineno, char *id)
{
    fprintf(stderr, "Error: (line %d) \"%s\" is not declared\n", lineno, id);
    exit(1);
}

void throwErrorRedeclaredId(char *id)
{
    fprintf(stderr, "Error: \"%s\" is already declared\n", id);
    exit(1);
}