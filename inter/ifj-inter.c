/* ifj-inter.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Author: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include "ifj-inter.h"
//all modules included here

static ifjInter* _ifj_inter_construct()
{
    ifjInter *interpreter = (ifjInter*)malloc(sizeof(struct _ifjInter));
    if(!interpreter)
    {
        fprintf(stderr,"ERROR: allocating interpreter!\n");
        exit(-1);
    }
    return interpreter;
}

static void ifj_inter_init( ifjInter *self )
{
    self->debugMode = 0;
    self->load = &ifj_load;
}

ifjInter* ifj_inter_new   ()
{
    ifjInter *interpreter = _ifj_inter_construct();
    ifj_inter_init(interpreter);
    return interpreter;
}
