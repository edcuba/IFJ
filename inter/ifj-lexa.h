/* ifj-lexa.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Jakub Kulich <xkulic03@stud.fit.vutbr.cz>
 */

#ifndef IFJ_LEXA_H
#define IFJ_LEXA_H

#include "utils/buffer.h"
#include "ifj-token.h"
#include "ifj-inter.h"
#include <stdio.h>

#define T_SEMICOLON ';'
#define T_COLON ':'
#define T_DOT '.'
#define T_LPAREN '('
#define T_RPAREN ')'
#define T_LBLOCK '{'
#define T_RBLOCK '}'
#define T_LARRAY '['
#define T_RARRAY ']'
#define T_ASSIGN '='
#define T_DIVIDE '/'
#define T_MULTIPLY '*'
#define T_SUBTRACT '-'
#define T_ADD '+'
#define T_LESS '<'
#define T_GREATER '>'
#define T_COMMA ','
#define T_NOT '!'
#define T_TERNARY '?'

#define T_LESS_EQUAL 601
#define T_GREATER_EQUAL 602
#define T_EQUAL 603
#define T_NOT_EQUAL 604
#define T_AND 605
#define T_OR 606
#define T_INC 607
#define T_DEC 608

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
#define T_CLASS 310
#define T_BOOLEAN 311
#define T_INTEGER 312
#define T_DOUBLE 313
#define T_STRING 314
#define T_FALSE 315
#define T_TRUE 316

#define T_IDENTIFIER 401
#define T_BOOLEAN_C 402
#define T_INTEGER_C 403
#define T_DOUBLE_C 404
#define T_STRING_C 405

#define T_END 501
#define T_UNKNOWN 502

enum lexa_state {

    LS_START,
    LS_NUMBER,
    LS_NUMBER_BIN,
    LS_NUMBER_HEX,
    LS_NUMBER_ZERO,
    LS_DOUBLE_NUMBER,
    LS_DOUBLE_NUMBER_HEX,
    LS_EXPO_FIRST_NUMBER,
    LS_EXPO_FIRST_NUMBER_HEX,
    LS_EXPO,
    LS_EXPO_HEX,
    LS_WORD,
    LS_WORD_D,
    LS_DIV,
    LS_COMPARE_LESS,
    LS_COMPARE_GREATER,
    LS_EQUAL,
    LS_STRING,
    LS_NEQ,
    LS_AND,
    LS_OR,
    LS_PLUS,
    LS_MINUS,
    LS_MULTI_COMMENT,
    LS_MULTI_COMMENT_END,
    LS_COMMENT,
    LS_ESCAPE,
    LS_ESCAPE_OCTAL
};

struct _ifj_lexa {
    FILE *inputFile;
    symbolTable *reserved_words;
    dyn_buffer *b_str;
    dyn_buffer *b_num;
};

/**
 * Function initialize lexical analysis module structure
 *
 * @return Pointer to structure, NULL if error occurred
 */
ifj_lexa *ifj_lexa_init();

/**
 * Free lexical analysis unit structure
 *
 * @param l
 */
void ifj_lexa_free(ifj_lexa *l);

/**
 * Function adds word to table of reserved words
 *
 * @param l Lexical analysis structure
 * @param word Pointer to word
 * @param token_type Token type for word
 */
void ifj_lexa_add_reserved(ifj_lexa *l, char *word, int token_type);

/**
 * Function finds out if word is reserved word
 *
 * @param l Lexical analysis structure
 * @param word Word to find out
 * @return Token type if function is reserved, otherwise -1
 */
int ifj_lexa_is_reserved(ifj_lexa *l, char *word);

/**
 * Read next token from input file and process it
 *
 * @param l Lexical analysis structure
 * @param self interpreter structure
 * @return next token
 */
token *lexa_next_token(ifj_lexa *l, symbolTable *table);

#endif
