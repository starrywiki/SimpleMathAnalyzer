#ifndef SYNTAX_H
#define SYNTAX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum Unop Unop;
typedef enum Binop Binop;
typedef enum Bracket Bracket;
typedef enum ExprType ExprType;
typedef struct Expr Expr;

enum Unop{
    OP_COS, OP_SIN, OP_TAN, OP_COT, OP_LN, OP_SQRT //要cot吗？
};

enum Binop{
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_POW
};

// enum Bracket{
//     LEFT_BRACKET, RIGHT_BRACKET
// };//树应该可以本身就表明明括号的存在，不需要额外的符号？

enum ExprType{
    E_INT,
    E_VAR,
    E_BINOP,
    E_UNOP
};

struct Expr{
    ExprType type;
    union{
        int const_val;
        char var_name;
        // struct{
        //     Bracket bracket;
        //     Expr *expr1;
        // } bracket_symbol;
        struct{
            Binop binop;
            Expr *expr1, *expr2;
        } binop_symbol;
        struct{
            Unop unop;
            Expr *expr1;
        } unop_symbol;
    }expr;
};

#endif
