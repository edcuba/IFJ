/* ifj-inter.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Author: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#ifndef IFJ_INTER_H
#define IFJ_INTER_H

#include <glib.h>

typedef struct _ifjInter
{
    gboolean debugMode;
    gint (*load)(gint, gchar**);
    void (*lexa)(void);
    void (*sema)(void);
    void (*syma)(void);

} ifjInter;

ifjInter* ifj_inter_new();

gint ifj_load   (   gint argc,
                    gchar **argv);

#endif
