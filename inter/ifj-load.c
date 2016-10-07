/* ifj-load.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors:
 */

 #include "ifj-inter.h"
 #include <stdio.h>

int ifj_load (  int argc,
                char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Error: No input file!\n");
        return 99; //no file set
    }
    return 0;
}
