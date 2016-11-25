/* ifj-base.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#include "ifj-inter.h"
#include <stdio.h>

//exit when return code not 0
#define check(arg) rc = arg; if(rc) do { ifj_inter_free(self); return rc;} while(0)

int main (  int argc,
            char **argv)
{
    int rc;
    ifjInter *self = ifj_inter_new(); //create new main struct
    //XXX
    self->debugMode = 1;
    check(self->load(argc, argv, self)); //load input file
    check(syna_run(self)); //run syntactic analysis
    check(exec_run(self)); //run executor


    ifj_inter_free(self);
    return 0;
}
