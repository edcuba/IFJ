/* ifj-base.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#include "ifj-inter.h"
#include <stdio.h>


#define check(arg) rc = arg; if(rc) do { ifj_inter_free(inter); return rc;} while(0) //exit when return code not 0

int main (  int argc,
            char **argv)
{
    int rc;
    ifjInter *inter = ifj_inter_new(); //create new main struct
    check( inter->load(argc, argv, inter) ); //load input file
    check( inter->syna(inter) ); //run syntactic analysis


    ifj_inter_free(inter);
    return 0;
}
