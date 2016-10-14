/* ifj-lexa.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Jakub Kulich <xkulic03@stud.fit.vutbr.cz>
 */

#include <stdlib.h>
#include <ctype.h>
#include "utils/htable.h"
#include "ifj-lexa.h"
#include "limits.h"
#include "ifj-inter.h"
#include "utils/buffer.h"

ifj_lexa *ifj_lexa_init() {
    ifj_lexa *l = malloc(sizeof(ifj_lexa));
    if (l == NULL) {
        return NULL;
    }

    // 29 for minimalising colision in hash table
    l->reserved_words = htab_init(29);
    l->b_str = dyn_buffer_init(64);
    l->b_num = dyn_buffer_init(16);

    ifj_lexa_add_reserved(l, 'while', T_WHILE);
    ifj_lexa_add_reserved(l, 'for', T_FOR);
    ifj_lexa_add_reserved(l, 'do', T_DO);
    ifj_lexa_add_reserved(l, 'break', T_BREAK);
    ifj_lexa_add_reserved(l, 'continue', T_CONTINUE);
    ifj_lexa_add_reserved(l, 'if', T_IF);
    ifj_lexa_add_reserved(l, 'else', T_ELSE);
    ifj_lexa_add_reserved(l, 'return', T_RETURN);
    ifj_lexa_add_reserved(l, 'void', T_VOID);
    ifj_lexa_add_reserved(l, 'static', T_STATIC);
    ifj_lexa_add_reserved(l, 'class', T_CLASS);
    ifj_lexa_add_reserved(l, 'boolean', T_BOOLEAN);
    ifj_lexa_add_reserved(l, 'integer', T_INTEGER);
    ifj_lexa_add_reserved(l, 'double', T_DOUBLE);
    ifj_lexa_add_reserved(l, 'string', T_STRING);
    ifj_lexa_add_reserved(l, 'false', T_FALSE);
    ifj_lexa_add_reserved(l, 'true', T_TRUE);

    return l;
}

void ifj_lexa_free(ifj_lexa *l) {
    htab_free(l->reserved_words);
    dyn_buffer_free(l->b_str);
    dyn_buffer_free(l->b_num);
    free(l);
}

void ifj_lexa_add_reserved(ifj_lexa *l, char *word, int token_type) {
    htab_lookup_add(l->reserved_words, word, (unsigned) token_type);
}

int ifj_lexa_is_reserved(ifj_lexa *l, char *word) {
    struct htab_listitem *item = htab_lookup(l->reserved_words, word);
    if (item == NULL) {
        return -1;
    } else {
        return item->id;
    }
}

token *lexa_next_token(ifj_lexa *l, ifjInter *self) {

    int newChar = 0;

    enum lexa_state state;
    state = LS_START;

    dyn_buffer_clear(l->b_str);
    dyn_buffer_clear(l->b_num);

    token *t = ifj_token_new();
    if (t == NULL) {
        return NULL;
    }

    while (1) {
        newChar = getc(self->inputFile);

        switch (state) {
            case LS_START:
                if (isdigit(newChar)) {
                    dyn_buffer_append(l->b_str, newChar);
                    state = LS_NUMBER;
                    break;
                } else if (isalpha(newChar)) {
                    dyn_buffer_append(l->b_str, newChar);
                    state = LS_WORD;
                    break;
                } else if (isspace(newChar)) {
                    break;
                } else if (newChar == EOF) {
                    t->type = T_END;
                    return t;
                } else if (newChar == '/') {
                    state = LS_DIV;
                    break;
                } else if (newChar == '\"') {
                    state = LS_STRING;
                    break;
                } else {
                    switch (newChar) {
                        case '_':
                        case '$':
                            dyn_buffer_append(l->b_str, newChar);
                            state = LS_WORD;
                            break;
                        case '<':
                            state = LS_COMPARE_LESS;
                            break;
                        case '>':
                            state = LS_COMPARE_GREATER;
                            break;
                        case '=':
                            state = LS_EQUAL;
                            break;
                        case '!':
                            state = LS_NEQ;
                            break;
                        case '&':
                            state = LS_AND;
                            break;
                        case '|':
                            state = LS_OR;
                            break;
                        case '+':
                            state = LS_PLUS;
                            break;
                        case '-':
                            state = LS_MINUS;
                            break;
                        case ';':
                        case ':':
                        case '(':
                        case ')':
                        case '{':
                        case '}':
                        case '[':
                        case ']':
                        case '*':
                        case ',':
                        case '?':
                            t->type = newChar;
                            return t;
                        default:
                            t->type = T_UNKNOWN;
                            return t;

                    }
                }

                break;
            case LS_DIV:
                if (newChar == '/') {
                    state = LS_COMMENT;
                    break;
                } else if (newChar == '*') {
                    state = LS_MULTI_COMMENT;
                    break;
                } else {
                    ungetc(newChar, self->inputFile);
                    t->type = T_DIVIDE;
                    return t;
                }
                break;
            case LS_COMMENT:
                if (newChar == '\n') {
                    state = LS_START;
                    break;
                } else if (newChar == EOF) {
                    t->type = T_END;
                    return t;
                }
                break;
            case LS_MULTI_COMMENT:
                if (newChar == '*') {
                    state = LS_MULTI_COMMENT_END;
                    break;
                } else if (newChar == EOF) {
                    free(t);
                    return NULL;
                }
                break;
            case LS_MULTI_COMMENT_END:
                if (newChar == '/') {
                    state = LS_START;
                    break;
                } else if (newChar == '*') {
                    break;
                } else if (newChar == EOF) {
                    free(t);
                    return NULL;
                } else {
                    state = LS_MULTI_COMMENT;
                    break;
                }
                break;
            case LS_STRING:
                if (newChar == '\"') {
                    // TODO: copy value from l->b_str to token and return
                } else if (newChar == '\\') {
                    state = LS_ESCAPE;
                    break;
                } else if (newChar >= 32 && newChar <= 255) {
                    dyn_buffer_append(l->b_str, newChar);
                } else {
                    free(t);
                    return NULL;
                }
                break;
            case LS_ESCAPE:
                if (isdigit(newChar)) {
                    state = LS_ESCAPE_OCTAL;
                    dyn_buffer_append(l->b_num, newChar);
                    break;
                } else if (newChar == EOF) {
                    return NULL;
                } else {
                    state = LS_START;

                    switch (newChar) {
                        case '\'':
                            dyn_buffer_append(l->b_str, '\'');
                            break;
                        case '\"':
                            dyn_buffer_append(l->b_str, '\"');
                            break;
                        case '\\':
                            dyn_buffer_append(l->b_str, '\\');
                            break;
                        case '\n':
                            dyn_buffer_append(l->b_str, '\n');
                            break;
                        case '\t':
                            dyn_buffer_append(l->b_str, '\t');
                            break;
                        default:
                            return NULL;
                    }
                }
                break;
            case LS_ESCAPE_OCTAL:
                if (l->b_num->top == 2) {
                    int escChar = (int) strtol(dyn_buffer_get_content(l->b_num),
                                         NULL, 8);
                    dyn_buffer_clear(l->b_num);

                    dyn_buffer_append(l->b_str, escChar);

                    ungetc(newChar, self->inputFile);
                    state = LS_STRING;
                } else {
                    if (isdigit(newChar)) {
                        dyn_buffer_append(l->b_num, newChar);
                    } else {
                        return NULL;
                    }
                }
                break;
            case LS_COMPARE_GREATER:
                if (newChar == '=') {
                    t->type = T_GREATER_EQUAL;
                    return t;
                } else {
                    ungetc(newChar, self->inputFile);
                    t->type = T_GREATER;
                    return t;
                }
            case LS_COMPARE_LESS:
                if (newChar == '=') {
                    t->type = T_LESS_EQUAL;
                    return t;
                } else {
                    ungetc(newChar, self->inputFile);
                    t->type = T_LESS;
                    return t;
                }
            case LS_AND:
                if (newChar == '&') {
                    t->type = T_AND;
                    return t;
                } else {
                    ungetc(newChar, self->inputFile);
                    t->type = T_UNKNOWN;
                    return t;
                }
            case LS_OR:
                if (newChar == '|') {
                    t->type = T_OR;
                    return t;
                } else {
                    ungetc(newChar, self->inputFile);
                    t->type = T_UNKNOWN;
                    return t;
                }
            case LS_EQUAL:
                if (newChar == '=') {
                    t->type = T_EQUAL;
                    return t;
                } else {
                    ungetc(newChar, self->inputFile);
                    t->type = T_ASSIGN;
                    return t;
                }
            case LS_NEQ:
                if (newChar == '=') {
                    t->type = T_NOT_EQUAL;
                    return t;
                } else {
                    ungetc(newChar, self->inputFile);
                    t->type = T_NOT;
                    return t;
                }
            case LS_PLUS:
                if (newChar == '+') {
                    t->type = T_INC;
                    return t;
                } else {
                    ungetc(newChar, self->inputFile);
                    t->type = T_ADD;
                    return t;
                }
            case LS_MINUS:
                if (newChar == '-') {
                    t->type = T_DEC;
                    return t;
                } else {
                    ungetc(newChar, self->inputFile);
                    t->type = T_SUBTRACT;
                    return t;
                }
            case LS_NUMBER:
                if (isdigit(newChar)) {
                    dyn_buffer_append(l->b_str, newChar);
                    break;
                } else if (newChar == '.') {
                    dyn_buffer_append(l->b_str, newChar);
                    state = LS_DOUBLE_NUMBER;
                    break;
                } else if (newChar == 'e' || newChar == 'E') {
                    dyn_buffer_append(l->b_str, newChar);
                    state = LS_EXPO_FIRST_NUMBER;
                    break;
                } else {
                    ungetc(newChar, self->inputFile);
                    long val = strtol(dyn_buffer_get_content(l->b_str), NULL, 10);
                    dyn_buffer_clear(l->b_str);

                    // TODO: overflow prevention using errno
                    if (val > INT_MAX) {
                        t->type = T_UNKNOWN;
                        return t;
                    }

                    t->type = T_INTEGER_C;
                    // TODO: set value
                    return t;
                }
                break;
            case LS_DOUBLE_NUMBER:
                if (isdigit(newChar)) {
                    dyn_buffer_append(l->b_str, newChar);
                    break;
                } else if (newChar == 'e' || newChar == 'E') {
                    dyn_buffer_append(l->b_str, 'e');
                    state = LS_EXPO_FIRST_NUMBER;
                    break;
                } else {
                    ungetc(newChar, self->inputFile);
                    double val = strtod(dyn_buffer_get_content(l->b_str), NULL);
                    dyn_buffer_clear(l->b_str);
                    // TODO: check overflowing
                    t->type = T_INTEGER_C;
                    // TODO: set value
                    return t;
                }
            case LS_EXPO_FIRST_NUMBER:
                if (newChar == '+' || newChar == '-') {
                    dyn_buffer_append(l->b_str, newChar);
                    state = LS_EXPO;
                    break;
                } else {
                    return NULL;
                }
                break;
            case LS_EXPO:
                if (isdigit(newChar)) {
                    dyn_buffer_append(l->b_str, newChar);
                    break;
                } else {
                    ungetc(newChar, self->inputFile);
                    double val = strtod(dyn_buffer_get_content(l->b_str), NULL);
                    dyn_buffer_clear(l->b_str);
                    // TODO: check overflowing
                    t->type = T_INTEGER_C;
                    // TODO: set value
                    return t;
                }
                break;
            case LS_WORD:
                if (isalnum(newChar) || newChar == '_') {
                    dyn_buffer_append(l->b_str, newChar);
                    break;
                } else {
                    char *value = dyn_buffer_get_content(l->b_str);
                    int tokenType = ifj_lexa_is_reserved(l, value);

                    ungetc(newChar, self->inputFile);

                    if (tokenType != -1) {
                        t->type = tokenType;
                        return t;
                    } else {
                        t->type = T_IDENTIFIER;
                        // TODO: Copy value to token
                        return t;
                    }
                }
                break;
        }
    }

}

