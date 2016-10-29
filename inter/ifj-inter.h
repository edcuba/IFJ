/* ifj-inter.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Author: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#ifndef IFJ_INTER_H
#define IFJ_INTER_H

typedef struct _token token;
typedef struct _ifjInter ifjInter;
typedef struct _symbol_table symbol_table;
typedef struct _ifj_lexa ifj_lexa;

#include <stdio.h>
#include "ifj-syna.h"
#include "ifj-lexa.h"
#include "ifj-token.h"
#include "ial.h"

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
    token *next;
};

/**
 * Main interpreter structure
 * - you can store every variable used in multiple modules here
 */
struct _ifjInter
{
    char debugMode;
    symbol_table *table;
    ifj_lexa *lexa_module;
    int     ( *load )( int, char**, ifjInter* );
    int     ( *sema )( ifjInter* );
    int     ( *syna )( ifjInter* );
};

ifjInter    *ifj_inter_new();
void        ifj_inter_free(ifjInter *self);
token       *ifj_token_new();
void        ifj_token_free( token *item);

int ifj_load(int argc, char **argv, ifjInter* inter);

#endif
