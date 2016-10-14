/* ifj-inter.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Author: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include "ifj-inter.h"

/**
 * Drop interpreter including all tables
 * @param self interpreter
 */
void ifj_inter_free(ifjInter *self)
{
    if(self->table)
    {
        self->table->drop(self->table);
        if(self->table->row)
            free(self->table->row);
        free(self->table);
    }
    self->table = NULL;
    free(self);
}

/**
 * Interpeter constructor
 * @return new, uninitialised interpreter structure
 */
static ifjInter* ifj_inter_construct()
{
    ifjInter *interpreter = (ifjInter *) malloc ( sizeof( struct _ifjInter ));
    if(!interpreter)
    {
        fprintf(stderr,"ERROR: allocating interpreter!\n");
        exit(99);
    }
    return interpreter;
}

/**
 * interpreter initialiser
 * @param self interpreter
 */
static void ifj_inter_init( ifjInter *self )
{
    self->debugMode = 0;
    self->inputFile = NULL;
    self->load = &ifj_load;
    self->lexa = &lexa_next_token;
    self->syna = &syna_run;
    self->table = ial_symbol_table_new();
}


/**
 * Create new interpreter structure
 *  - contains default methods and attributes
 * @return  interpreter
 */
ifjInter* ifj_inter_new   ()
{
    ifjInter *interpreter = ifj_inter_construct();
    ifj_inter_init(interpreter);
    return interpreter;
}

/**
 * Drop token not connected to hash structure
 * @param item token
 */
void ifj_token_free( token *item)
{
    if( !item )
        return;

    if (item->value)
        free( (void*) item->value);

    free(item);
}

/**
 * token constructor
 * @return new, uninitialised token structure
 */
static token* ifj_token_construct()
{
    token *item = (token *) malloc ( sizeof( struct _token ));
    if(!item)
    {
        fprintf(stderr,"ERROR: allocating token!\n");
        exit(99);
    }
    return item;
}

/**
 * token initialiser
 * @param self token
 */
static void ifj_token_init( token *self )
{
    self->type = 0;
    self->value = NULL;
    self->next = NULL;
}

/**
 * Create new token structure
 * @return  token
 */
token * ifj_token_new   ()
{
    token *item = ifj_token_construct();
    ifj_token_init(item);
    return item;
}
