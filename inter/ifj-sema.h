/* ifj-sema.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Eduard Cuba
 */

#ifndef IFJ_SEMA_H
#define IFJ_SEMA_H

#include "ifj-inter.h"

int sema_run( ifjInter *self);

int resolve_identifier(ifjInter *self,
                       symbolTable *table,
                       token **item,
                       int isDefiniton);
int check_typing(token *op,
                token *first,
                token *second);

#endif
