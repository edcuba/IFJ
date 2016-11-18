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

#define IFJ16_PRINT 1001
#define IFJ16_READINT 1002
#define IFJ16_READDOUBLE 1003
#define IFJ16_READSTRING 1004
#define IFJ16_FIND 1005
#define IFJ16_SORT 1006
#define IFJ16_LENGTH 1007
#define IFJ16_SUBSTR 1008
#define IFJ16_COMPARE 1009

#include <stdio.h>
#include "ifj-syna.h"
#include "ifj-exp.h"
#include "ifj-lexa.h"
#include "ifj-token.h"
#include "ial.h"
#include "ifj-util.h"
#include "ifj-sema.h"

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
    token **args;
    
    //variable/return value
    int dataType;
    void *data;
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
    int     ( *syna )( ifjInter* );
};

ifjInter    *ifj_inter_new();
void        ifj_inter_free(ifjInter *self);
token       *ifj_token_new();
void        ifj_token_free( token *item);
instruction * ifj_instruction_new   ();

void ifj_global_symbol_table_init(ifjInter *self);

int ifj_load(int argc, char **argv, ifjInter* inter);
char *strdup (const char *s1); //form POSIX

#endif
