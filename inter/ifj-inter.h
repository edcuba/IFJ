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
    int type;
    const void *value;
    symbolTable *childTable;
    token *next;
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
    void *op1;
    void *op2;
    void *op3;
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

int ifj_load(int argc, char **argv, ifjInter* inter);
char *strdup (const char *s1); //form POSIX

#endif
