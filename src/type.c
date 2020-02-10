#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include "type.h"

void checkAssignment(Type t_type_left, Type t_type_right, int lineno)
{
    char *type_left = typeToString(t_type_left);
    char *type_right = typeToString(t_type_right);

    if ((t_type_left == t_float && t_type_right == t_float) ||
        (t_type_left == t_float && t_type_right == t_int))
    {
        // Do nothing. Allow this kind of assignment
    }
    else if (t_type_left != t_type_right)
    {
        fprintf(stderr, "Error: (line %d) cannot assign type '%s' to type '%s'\n", lineno, type_right, type_left);
        exit(EXIT_FAILURE);
    }
    // else if (t_type_left)
    // else
    // {
    //     fprintf(stderr, "Error: (line %d) unsupported assignment", lineno);
    // }
}

void checkIsBool(Type t_type, int lineno)
{   
    char *str = typeToString(t_type);
    if (t_type != t_bool)
    {
        fprintf(stderr, "Error: (line %d) expecting type 'bool', got type '%s' instead\n", lineno, str);
        exit(EXIT_FAILURE);
    }
}

Type resolveBinaryMath(Type t_type_left, Type t_type_right, int lineno)
{
    if (t_type_left == t_int && t_type_right == t_int)
    {
        return t_int;
    }
    else if ((t_type_left == t_float && t_type_right == t_float) || 
        (t_type_left == t_float && t_type_right == t_int) || 
        (t_type_left == t_int && t_type_right == t_float))
    {
        return t_float;
    }
    else if (t_type_left == t_string && t_type_right == t_string)
    {
        return t_string;
    }
    else 
    {
        fprintf(stderr, "Error: (line %d) cannot express a binary Mathematical expression with a pair of types '%s' and '%s'\n", lineno, typeToString(t_type_left), typeToString(t_type_right));
        exit(EXIT_FAILURE);
    }
}

void checkUnaryMinus(Type t_type, int lineno)
{
    if (t_type != t_int && t_type != t_float)
    {
        fprintf(stderr, "Error: (line %d) cannot express type '%s' with '-' operator\n", lineno, typeToString(t_type));
        exit(EXIT_FAILURE);
    }
}

void checkUnaryLogicNot(Type t_type, int lineno)
{
    if (t_type != t_bool)
    {
        fprintf(stderr, "Error: (line %d) cannot express type '%s' with '!' operator\n", lineno, typeToString(t_type));
        exit(EXIT_FAILURE);
    }
}

void checkBinaryComparison(Type t_type_left, Type t_type_right, int lineno)
{
    if (!((t_type_left == t_type_right) ||
        (t_type_left == t_int && t_type_right == t_float) ||
        (t_type_left == t_float && t_type_right == t_int)))
    {
        fprintf(stderr, "Error: (line %d) cannot express a binary comparison with a pair of types '%s' and '%s'\n", lineno, typeToString(t_type_left), typeToString(t_type_right));
        exit(EXIT_FAILURE);
    }
}

void checkBinaryLogic(Type t_type_left, Type t_type_right, int lineno)
{
    if (t_type_left != t_bool && t_type_right != t_bool)
    {
        fprintf(stderr, "Error: (line %d) cannot express a binary logic expression with a pair of types '%s' and '%s'\n", lineno, typeToString(t_type_left), typeToString(t_type_right));
        exit(EXIT_FAILURE);
    }
}
