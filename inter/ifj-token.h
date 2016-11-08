/* ifj-token.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Author: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#ifndef IFJ_UTILS_H
#define IFJ_UTILS_H

#include "ifj-inter.h"

char * ifj_generate_hashname ( token *item);

token * ifj_generate_token(symbolTable *table, int type);

token * ifj_generate_token_int ( symbolTable *table, int value );

token * ifj_generate_token_double ( symbolTable *table, double value );

token * ifj_generate_token_str ( symbolTable *table, char *value);

token * ifj_generate_token_id ( symbolTable *table, char *value);

char * ifj_generate_hashname_str(char * value);
char * ifj_generate_hashname_int(int *value);
char * ifj_generate_hashname_double(double *value);

#endif
