/* ifj-inter.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Author: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#ifndef IFJ_INTER_H
#define IFJ_INTER_H

typedef struct _token_stack token_stack;
typedef struct _token token;
typedef struct _ifjInter ifjInter;
typedef struct _symbolTable symbolTable ;
typedef struct _ifj_lexa ifj_lexa;
typedef struct _instruction instruction;
typedef struct _linear_list linear_list;
typedef struct _ifjSyna ifjSyna;

#define IFJ16_PRINT 1001
#define IFJ16_READINT 1002
#define IFJ16_READDOUBLE 1003
#define IFJ16_READSTRING 1004
#define IFJ16_FIND 1005
#define IFJ16_SORT 1006
#define IFJ16_LENGTH 1007
#define IFJ16_SUBSTR 1008
#define IFJ16_COMPARE 1009

#define E_TYPE 2222

#include <stdio.h>
#include "ifj_syna.h"
#include "ifj_exp.h"
#include "ifj_lexa.h"
#include "ifj_token.h"
#include "ial.h"
#include "ifj_util.h"
#include "ifj_sema.h"

/**
 * Struct for storing tokens in symbol table
 * acessing values:
 * str/id: ((char* )item->value)
 * int: *((int*) item->value)
 * double: *((double*) item->value)
 */
struct _token
{
    //default token parameters
    int type;
    const void *value;
    token *next;

    //method
    symbolTable *childTable; //user-defined block/class
    int method; //prebuild method
    instruction *jump; //first instruction of user-defined block
    token_stack *args;

    //variable/return value
    int dataType;
    void *data;
};

struct _ifjSyna
{
    token *semicolon;
    token *ternary;
    token *t_less;
    token *E;
    token_stack *type_stack;
    token_stack *stack;
    token_stack *help_stack;
    int (*predictExpresion)[11][11];
    int (*predictCondition)[17][17];
};


/**
 * Struct for stroring 3-address instruction
 * Used in instruction list generated in parser
 * @type instruction type
 * @op1 address of first operand
 * @op2 address of second operand
 * @op3 addres of third operand
 */
struct _instruction
{
    int type;
    token *op1;
    token *op2;
    token *op3;
    instruction *next;
};

/**
 * Main interpreter structure
 * - you can store every variable used in multiple modules here
 */
struct _ifjInter
{
    char debugMode;
    symbolTable *table;
    ifj_lexa *lexa_module;
    linear_list *code;
    token_stack *stack;

    int     ( *load )( int, char**, ifjInter* );
    int     ( *sema )( ifjInter* );
    ifjSyna *syna;
    token *pushBack;
    int preLoad;
    int returnCode;
    instruction *bootstrap;
};

ifjInter    *ifj_inter_new();
void        ifj_inter_free(ifjInter *self);
token       *ifj_token_new();
void        ifj_token_free( token *item);
instruction * ifj_instruction_new();

ifjSyna *ifj_syna_new();
// Doplnenie
void ifj_syna_free(ifjSyna *self);

void ifj_global_symbol_table_init(ifjInter *self);
void print_unexpected(ifjInter *self, token *item);
void print_mistyped(ifjInter *self, token *item, token *expected);
void print_defined_void(ifjInter *self, token *active);

int ifj_load(int argc, char **argv, ifjInter* inter);
char *strdup (const char *s1); //form POSIX

#endif
