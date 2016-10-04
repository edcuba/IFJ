/* ifj-base.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#include "ifj-inter.h"
#include <stdio.h>

#define check(arg) rc = arg; if(rc) return rc

int main (  int argc,
            char **argv)
{
    int rc;
    ifjInter *inter = ifj_inter_new();
    check( inter->load(argc, argv) );
    check( inter->syna(inter) );

    return 0;
}
