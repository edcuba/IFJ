/* ifj-inter.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Author: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include "ifj-inter.h"


/**
 * Interpeter constructor
 * @return new, uninitialised interpreter structure
 */
static ifjInter* _ifj_inter_construct()
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
    ifjInter *interpreter = _ifj_inter_construct();
    ifj_inter_init(interpreter);
    return interpreter;
}
