/* ifj-token.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Author: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#ifndef IFJ_UTILS_H
#define IFJ_UTILS_H

#include "ifj-inter.h"

char * ifj_generate_hashname ( token *item);

token * ifj_generate_token(symbol_table *table, int type);

token * ifj_generate_token_int ( symbol_table *table, int value );

token * ifj_generate_token_double ( symbol_table *table, double value );

token * ifj_generate_token_str ( symbol_table *table, char *value);

token * ifj_generate_token_id ( symbol_table *table, char *value);

char * ifj_generate_hashname_str(char * value);
char * ifj_generate_hashname_int(int *value);
char * ifj_generate_hashname_double(double *value);

#endif
