/* ifj-load.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

 #include "ifj-inter.h"
 #include <stdio.h>

gint ifj_load ( gint argc,
                gchar **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Error: No input file!\n");
        return 99; //no file set
    }
    return 0;
}
