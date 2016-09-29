/* ifj-base.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#include "ifj-inter.h"
#include <stdio.h>

#define check(arg) rc = arg; if(rc) return rc

gint main ( gint argc,
            gchar **argv)
{
    gint rc;
    ifjInter *inter = ifj_inter_new();
    check( inter->load(argc, argv) );


    return 0;
}
