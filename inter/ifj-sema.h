/* ifj-sema.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Eduard Cuba
 */

#ifndef IFJ_SEMA_H
#define IFJ_SEMA_H

#include "ifj-inter.h"

int sema_run( ifjInter *self);

int resolve_identifier(symbolTable *table,
                       token **item,
                       int isDefiniton);

#endif
