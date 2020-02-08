#include <stdlib.h>
#include "tree.h"

#ifndef SYMBOL_H
#define SYMBOL_H

#define HASH_SIZE 317

extern int print_sym_table;

typedef struct Symbol 
{
    char *name;
    Type type;
    struct Symbol *next;

} Symbol;

typedef struct SymbolTable 
{
    Symbol *table[HASH_SIZE];
    struct SymbolTable *parent;

} SymbolTable;

void makeSymbolTable(STMT_LIST *root);

Symbol *putSymbol(SymbolTable *t, char *name, Type type, int lineno);
Symbol *getSymbol(SymbolTable *t, char *name);

SymbolTable *initSymbolTable();
SymbolTable *scopeSymbolTable(SymbolTable *parent);

#endif