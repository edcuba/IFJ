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

#include <stdio.h>
#include "ifj-syna.h"
#include "ifj-lexa.h"
#include "ial.h"

/**
 * Struct for storing tokens in symbol table
 */
struct _token
{
    const char *name;
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
    FILE *inputFile;
    symbol_table *table;
    int     ( *load )( int, char**, ifjInter* );
    token  *( *lexa )( ifjInter* );
    int     ( *sema )( ifjInter* );
    int     ( *syna )( ifjInter* );
    //TODO double linked list for token pointers
};

ifjInter    *ifj_inter_new();
token       *ifj_token_new();

int ifj_load(int argc, char **argv, ifjInter* inter);

#endif
