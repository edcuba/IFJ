/* ifj-exp.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Jan Demcak <xdemca01@stud.fit.vutbr.cz>
 */

#ifndef IFJ_EXP_H
#define IFJ_EXP_H

#include "ifj_inter.h"
#include "ifj_util.h"
#include "ifj_syna.h"
#include "ifj_lexa.h"
#include "ifj_token.h"
#include "ifj_exec.h"
#include <stdbool.h>

int expresion(ifjInter *self, symbolTable *table, token *expected);
int condition(ifjInter *self, symbolTable *table);

int condition_check_active(ifjInter *self, token *active, int *b);
int condition_check_top_stack(ifjInter *self, token *top_stack, int *a);
int expresion_check_active(ifjInter *self, token *active, int *b);
int expresion_check_top_stack(ifjInter *self, token *top_stack, int *a);

int type_control(ifjInter *self);
int type_control_plus(ifjInter *self);



#endif
