/* ifj-sema.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Eduard Cuba
 */

#ifndef IFJ_SEMA_H
#define IFJ_SEMA_H

#include "ifj-inter.h"

int resolve_identifier(ifjInter *self,
                       symbolTable *table,
                       token **item,
                       int isDefiniton);
int check_typing(token *first, token *second);

void print_table(symbolTable *table, int level);
token *duplicate_context(token *item);
token *resolve_context(ifjInter *self, token *item, token *target);

#endif
