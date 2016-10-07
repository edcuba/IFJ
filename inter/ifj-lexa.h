/* ifj-lexa.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Jakub Kulich <xkulic03@stud.fit.vutbr.cz>
 */

#ifndef IFJ_LEXA_H
#define IFJ_LEXA_H

#include "ifj-inter.h"

#define T_SEMICOLON ';'
#define T_LPAREN '('
#define T_RPAREN ')'
#define T_LBLOCK '{'
#define T_RBLOCK '}'
#define T_ASSIGN '='
#define T_DIVIDE '/'
#define T_MULTIPLY '*'
#define T_SUBTRACT '-'
#define T_ADD '+'

#define T_WHILE 300
#define T_FOR 301
#define T_DO 302
#define T_BREAK 303
#define T_CONTINUE 304
#define T_IF 305
#define T_ELSE 306
#define T_RETURN 307
#define T_VOID 308
#define T_STATIC 309

#define T_IDENTIFIER 401
#define T_BOOLEAN 402
#define T_INTEGER 403
#define T_DOUBLE 404
#define T_STRING 405
#define T_CLASS 406

#define T_TRUE 407
#define T_FALSE 408

#define T_END 501
#define T_UNKNOWN 502

token * lexa_next_token ( ifjInter *self );

#endif
