/* ifj-inter.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Author: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#ifndef IFJ_INTER_H
#define IFJ_INTER_H

typedef struct _IfjInter 
{
    gboolean debugMode;
    void (*lexical_analyse)(void);
} IfjInter;

IfjInter* ifj_inter_new(); 

#endif
