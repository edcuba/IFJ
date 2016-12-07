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

int expression(ifjInter *self, symbolTable *table, token *expected, int endChar);
int condition(ifjInter *self, symbolTable *table, int endChar);

int condition_check_active(ifjInter *self, token *active, int *b);
int condition_check_top_stack(ifjInter *self, token *top_stack, int *a);
int expression_check_active(ifjInter *self, token *active, int *b);
int expression_check_top_stack(ifjInter *self, token *top_stack, int *a);

int type_control(ifjInter *self);
int type_control_plus(ifjInter *self);



int E_reduct(ifjInter *self,
             int insert,
             int debug_constant,
             int plus_operation,
             ifjSyna *syna,
             token * active,
             token * top_on_help_stack,
             token **top_stack,
             token * instructHelp);

int E_simple_reduct(ifjInter *self,
                    int debug_constant,
                    ifjSyna *syna,
                    token **top_stack,
                    token * top_on_help_stack,
                    token * active);


#endif
