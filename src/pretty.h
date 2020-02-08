#ifndef PRETTY_H
#define PRETTY_H

#include "tree.h"

void prettySTMT(STMT *s);
void prettySTMT_LIST(STMT_LIST *s);
void prettyEXP(EXP *e);
void prettyIFSTMT(IFSTMT *s);
void prettyTYPE(Type t);

#endif