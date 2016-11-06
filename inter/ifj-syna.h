/* ifj-syna.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors:
 */

#ifndef IFJ_SYNA_H
#define IFJ_SYNA_H

#include "ifj-inter.h"

int is_ID(ifjInter *self);
int is_while(ifjInter *self);
int is_LPAREN(ifjInter *self);
int is_RPAREN(ifjInter *self);
int is_ASSIGN(ifjInter *self);
int is_semicolon(ifjInter *self);

int start(ifjInter *self);
int next_class(ifjInter *self);

int class_inside(ifjInter *self);
int class_inside1(ifjInter *self);
int class_inside2(ifjInter *self);


int id_declar(ifjInter *self);
int id_declar1(ifjInter *self);
int function_declar(ifjInter *self);
int next_function_param(ifjInter * self);

int function_inside(ifjInter *self);
int function_inside1(ifjInter *self);

int if_else1(ifjInter *self);

int statement_inside(ifjInter *self);
int statement_inside1(ifjInter *self);

int condition(ifjInter *self);
int sth_next(ifjInter *self);
int fce(ifjInter *self);
int method_call(ifjInter *self);
int function_parameters(ifjInter *self);
int next_function_parameters(ifjInter *self);

int tell_me_type_with_void(ifjInter *self);
int tell_me_type_without_void(ifjInter *self);
int rel_operator(ifjInter *self);

int expresion(ifjInter *self);

int syna_run( ifjInter *self);
#endif
