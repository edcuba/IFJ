/* ifj-syna.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Jan Demcak <xdemca01@stud.fit.vutbr.cz>
 */

#ifndef IFJ_SYNA_H
#define IFJ_SYNA_H

#include "ifj-inter.h"
#include "ifj-exp.h"

int is_ID(ifjInter *self, symbolTable *table, token **item);
int is_while(ifjInter *self);
int is_LPAREN(ifjInter *self);
int is_RPAREN(ifjInter *self);
int is_ASSIGN(ifjInter *self);
int is_semicolon(ifjInter *self);
int is_LBLOCK(ifjInter *self);
int is_ID_number(ifjInter *self);

int start(ifjInter *self);
int next_class(ifjInter *self);

int class_inside(ifjInter *self, symbolTable *table);
int class_inside1(ifjInter *self, symbolTable *table);
int class_inside2(ifjInter *self, symbolTable *table, token *item);

int function_declar(ifjInter *self, token *item);
int next_function_param(ifjInter *self, token *item);

int function_inside(ifjInter *self, token *item);
int function_inside1(ifjInter *self, token *item);

int if_else1(ifjInter *self, symbolTable *table);

int statement_inside(ifjInter *self); //GARBAGE
int statement_inside1(ifjInter *self, symbolTable *table);

int sth_next(ifjInter *self, symbolTable *table, token *item);
int fce(ifjInter *self);
int method_call(ifjInter *self);
int function_parameters(ifjInter *self);
int next_function_parameters(ifjInter *self);
int function_parameters_for_exp(ifjInter *self);
int next_function_parameters_for_exp(ifjInter *self);

int get_type_with_void(ifjInter *self, token **item);
int get_type_without_void(ifjInter *self, token **item)
int rel_operator(ifjInter *self);

int expresion(ifjInter *self);

int syna_run( ifjInter *self);
#endif
