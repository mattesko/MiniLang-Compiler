#include <stdbool.h>
#ifndef TREE_H
#define TREE_H

typedef enum {
    k_statementKind_read,
    k_statementKind_print,
    k_statementKind_assignment,
    k_statementKind_initStrictType,
    k_statementKind_initLoose,
    k_statementKind_whileLoop,
    k_statementKind_ifStmt
} StatementKind;

typedef enum {
    k_expressionKind_identifier,
    k_expressionKind_intLiteral,
    k_expressionKind_floatLiteral,
    k_expressionKind_stringLiteral,
    k_expressionKind_boolLiteral,
    k_expressionKind_addition,
    k_expressionKind_subtraction,
    k_expressionKind_multiplication,
    k_expressionKind_division,
    k_expressionKind_unaryMinus,
    k_expressionKind_unaryLogicNot,
    k_expressionKind_equal,
    k_expressionKind_notEqual,
    k_expressionKind_GT,
    k_expressionKind_GTE,
    k_expressionKind_ST,
    k_expressionKind_STE,
    k_expressionKind_logicOr,
    k_expressionKind_logicAnd,
    k_expressionKind_withParantheses
} ExpressionKind;

typedef enum {
    t_bool,
    t_string,
    t_int,
    t_float
} Type;

typedef enum {
    k_ifStatementKind_if,
    k_ifStatementKind_ifElse,
    k_ifStatementKind_ifElseIf
} IfStatementKind;

typedef struct STMT_LIST STMT_LIST;
typedef struct IFSTMT IFSTMT;
typedef struct STMT STMT;
typedef struct EXP EXP;
typedef struct TYPE TYPE;

struct STMT {
    int lineno;
    StatementKind kind;

    union {

        struct {
            char *identifier;
            Type type;
        } read;

        struct {
            EXP *exp;
        } print;

        struct {
            char *identifier;
            EXP *exp;
        } assignment;

        struct
        {
            char *identifier;
            Type type;
            EXP *exp;
        } initStrictType;

        struct
        {
            char *identifier;
            EXP *exp;
        } initLooseType;

        struct {
            EXP *exp;
            STMT_LIST *stmtList;
        } whileLoop;

        IFSTMT *ifStmt;

    } val;
    
};

struct STMT_LIST {
    int lineno;
    STMT *currentStmt;
    STMT_LIST *nextStmtList;
};

struct EXP {
    int lineno;
    ExpressionKind kind;
    Type type;

    union {
        char *identifier;
        
        bool boolLiteral;
        char *stringLiteral;
        int intLiteral;
        float floatLiteral;

        struct {
            EXP *left;
            EXP *right; 
        } binary ;

        EXP *unary;
    } val;

};

struct IFSTMT {
    int lineno;
    IfStatementKind kind;

    union {

        struct {
            EXP *exp;
            STMT_LIST *ifPart;
        } ifStmt;

        struct {
            EXP *exp;
            STMT_LIST *ifPart;
            STMT_LIST *elsePart;
        } ifElse;

        struct
        {
            EXP *exp;
            STMT_LIST *ifPart;
            IFSTMT *ifStmt;
        } ifElseIf;
                
    } val;
    
};

struct TYPE {
    int lineno;
    Type type;
};

STMT_LIST *makeSTMT_LIST(STMT *currentStmt, STMT_LIST *nextStmtList);

STMT *makeSTMT_skip();
STMT *makeSTMT_read(char *identifier);
STMT *makeSTMT_print(EXP *exp);
STMT *makeSTMT_assignment(char *identifier, EXP *exp);
STMT *makeSTMT_initStrictType(char *identifier, TYPE *type, EXP *exp);
STMT *makeSTMT_initLooseType(char *identifier, EXP *exp);
STMT *makeSTMT_whileLoop(EXP *exp, STMT_LIST *stmtList);
STMT *makeSTMT_ifStmt(IFSTMT *ifStmt);

EXP *makeEXP_identifier(char *identifier);
EXP *makeEXP_boolLiteral(bool boolLiteral);
EXP *makeEXP_stringLiteral(char *stringLiteral);
EXP *makeEXP_intLiteral(int intLiteral);
EXP *makeEXP_floatLiteral(float floatLiteral);
EXP *makeEXP_binary(ExpressionKind kind, EXP *left, EXP *right);
EXP *makeEXP_unary(ExpressionKind kind, EXP *unary);
EXP *makeEXP_withParantheses(EXP *exp);

IFSTMT *makeIFSTMT_if(EXP *exp, STMT_LIST *ifPart);
IFSTMT *makeIFSTMT_ifElse(EXP *exp, STMT_LIST *ifPart, STMT_LIST *elsePart);
IFSTMT *makeIFSTMT_ifElseIf(EXP *exp, STMT_LIST *ifPart, IFSTMT *ifStmt);

TYPE *makeTYPE(Type type);
char *typeToString(Type type);

#endif