/* ifj-inter.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Author: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include "ifj-inter.h"
#include <string.h>

/**
 * Drop interpreter including all tables
 * @param self interpreter
 */
void ifj_inter_free(ifjInter *self)
{
    if(self->table)
    {
        self->table->drop(self->table);
    }
    ifj_lexa_free(self->lexa_module);
    ifj_drop_list(self->code);
    self->table = NULL;
    free(self);
}

/**
 * Create new interpreter structure
 *  - contains initialized default methods and attributes
 * @return interpreter
 */
ifjInter* ifj_inter_new   ()
{
    ifjInter *self = calloc ( 1, sizeof( ifjInter ));
    if(!self)
    {
        fprintf(stderr,"ERROR: allocating interpreter!\n");
        exit(99);
    }
    self->debugMode = 0;
    self->lexa_module = ifj_lexa_init();
    self->load = &ifj_load;
    self->syna = &syna_run;
    self->table = ial_symbol_table_new();
    self->code = ifj_list_new();
    return self;
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
 * Create new initialized token structure
 * @return token
 */
token * ifj_token_new   ()
{
    token *item = calloc (1, sizeof( token ));
    if(!item)
    {
        fprintf(stderr,"ERROR: allocating token!\n");
        exit(99);
    }
    item->type = 0;
    item->value = NULL;
    item->next = NULL;
    return item;
}

/**
 * Create new initialized instruction structure
 * @return instruction
 */
instruction * ifj_instruction_new   ()
{
    instruction *item = calloc (1, sizeof( instruction ));
    if(!item)
    {
        fprintf(stderr,"ERROR: allocating instruction!\n");
        exit(99);
    }
    item->type = 0;
    item->op1 = NULL;
    item->op2 = NULL;
    item->op3 = NULL;
    return item;
}

char *strdup (const char *s1)
{
    if(!s1)
        return NULL;
    size_t len = strlen(s1);
    char *dupl = malloc((len + 1) * sizeof(char));
    memcpy(dupl, s1, len);
    dupl[len] = 0; //null terminator
    return dupl;
}
