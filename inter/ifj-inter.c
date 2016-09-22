/* ifj-inter.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Author: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "ifj-inter.h"
//all modules included here

static IfjInter* ifj_inter_construct()
{
    IfjInter *interpreter = (IfjInter*)malloc(sizeof(struct _IfjInter));
    if(!interpreter)
    {
	fprintf(stderr,"ERROR: allocating interpreter!\n");
	exit(-1);
    }
    return interpreter;
}

static void ifj_inter_init(IfjInter *self)
{
    self->debugMode = 0;
}

IfjInter* ifj_inter_new   ()
{
    IfjInter *interpreter = ifj_inter_construct();
    ifj_inter_init(interpreter);
    return interpreter;
}
