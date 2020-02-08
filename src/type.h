#include "tree.h"

void checkAssignment(Type t_type_left, Type t_type_right, int lineno);
void checkIsBool(Type t_type, int lineno);
Type resolveBinaryMath(Type t_type_left, Type t_type_right, int lineno);
void checkUnaryMinus(Type t_type, int lineno);
void checkUnaryLogicNot(Type t_type, int lineno);
void checkBinaryComparison(Type t_type_left, Type t_type_right, int lineno);
void checkBinaryLogic(Type t_type_left, Type t_type_right, int lineno);
void typeCheckEXP(EXP *exp);