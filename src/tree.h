#include <stdbool.h>
#ifndef TREE_H
#define TREE_H

typedef enum {
    k_read,
    k_print,
    k_assignment,
    k_initStrictType,
    k_initLoose,
    k_declaration
} StatementKind;

typedef enum {
    k_identifier,
    k_intLiteral,
    k_floatLiteral,
    k_stringLiteral,
    k_boolLiteral,
    k_addition,
    k_subtraction,
    k_multiplication,
    k_division,
    k_unaryMinus,
    k_unaryLogicNot,
    k_equal,
    k_notEqual,
    k_GT,
    k_GTE,
    k_ST,
    k_STE,
    k_logicOr,
    k_logicAnd
} ExpressionKind;

typedef enum {
    k_bool,
    k_string,
    k_int,
    k_float
} TypeKind;

typedef enum {
    k_while,
    k_if,
    k_ifElse
} ControlFlowKind;

typedef struct PROGRAM PROGRAM;
typedef struct STMT_LIST STMT_LIST;
typedef struct CTRL_FLOW CTRL_FLOW;
typedef struct STMT STMT;
typedef struct EXP EXP;
typedef struct TYPE TYPE;

struct STMT {
    int lineno;
    StatementKind kind;

    union {

        struct {
            char *identifier;
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
            TYPE *type;
            EXP *exp;
        } initStrictType;

        struct
        {
            char *identifier;
            EXP *exp;
        } initLooseType;


        struct {
            char *identifier;
            TYPE *type;
        } declaration;
        
    } val;
    
};

struct STMT_LIST {
    int lineno;
    STMT *currentStmt;
    STMT_LIST *nextStmtList;
};

struct PROGRAM {
    int lineno;
    STMT_LIST *stmtList;
};

struct EXP {
    int lineno;
    ExpressionKind kind;

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

// TODO THIS WILL BREAK
// 'if else while' breaks
struct CTRL_FLOW {
    int lineno;
    ControlFlowKind kind;

    union {

        struct {
            EXP *exp;
            STMT_LIST *stmtList;
        } whileLoop;

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
            CTRL_FLOW *elsePart;
        } ifElseIf;
                
    } val;
    
};

struct TYPE {
    int lineno;
    TypeKind kind;
};

PROGRAM *makePROGRAM(STMT_LIST *stmtList);

STMT_LIST *makeSTMT_LIST(STMT *currentStmt, STMT_LIST *nextStmtList);

STMT *makeSTMT_skip();
STMT *makeSTMT_read(char *identifier);
STMT *makeSTMT_print(EXP *exp);
STMT *makeSTMT_assignment(char *identifier, EXP *exp);
STMT *makeSTMT_initStrictType(char *identifier, TYPE *type, EXP *exp);
STMT *makeSTMT_initLooseType(char *identifier, EXP *exp);
STMT *makeSTMT_declaration(char *identifier, TYPE *type);

EXP *makeEXP_identifier(char *identifier);
EXP *makeEXP_boolLiteral(bool boolLiteral);
EXP *makeEXP_stringLiteral(char *stringLiteral);
EXP *makeEXP_intLiteral(int intLiteral);
EXP *makeEXP_floatLiteral(float floatLiteral);
EXP *makeEXP_binary(ExpressionKind kind, EXP *left, EXP *right);
EXP *makeEXP_unary(ExpressionKind kind, EXP *unary);

CTRL_FLOW *makeCTRL_FLOW_if(EXP *exp, STMT_LIST *ifPart);
CTRL_FLOW *makeCTRL_FLOW_ifElse(EXP *exp, STMT_LIST *ifPart, STMT_LIST *elsePart);
CTRL_FLOW *makeCTRL_FLOW_ifElseIf(EXP *exp, STMT_LIST *ifPart, CTRL_FLOW *elsePart);
CTRL_FLOW *makeCTRL_FLOW_while(EXP *exp, STMT_LIST *stmtList);

TYPE *makeTYPE(TypeKind kind);

#endif