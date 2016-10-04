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

#include "ifj-syna.h"
#include "ifj-lexa.h"
#include "ial.h"

struct _token
{
    void *ptr;
};

struct _ifjInter
{
    char debugMode;
    symbol_table *table;
    int     ( *load )( int, char** );
    token  *( *lexa )( ifjInter* );
    int     ( *sema )( ifjInter* );
    int     ( *syna )( ifjInter* );
};

ifjInter* ifj_inter_new();

int ifj_load   (   int argc,
                    char **argv);

#endif