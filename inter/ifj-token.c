/* ifj-token.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Author: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */
#include "ifj-token.h"
#include <string.h>
#include "ifj-lexa.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Generate hashname for string constant
 * @param pointer to string
 * @return string hashname
 */
char * ifj_generate_hashname_str(char * value)
{
    int len = strlen((char *) value) + 10;
    char * hashname = (char *) malloc (sizeof(char) * len );
    snprintf(hashname, len, "//string_%s", value);
    return hashname;
}

/**
 * Generate hashname for integer constant
 * @param pointer to integer
 * @return integer hashname
 */
char * ifj_generate_hashname_int(int *value)
{
    char * hashname = (char *) malloc (sizeof(char)*16);
    snprintf(hashname, 16, "//int%d", *value);
    return hashname;
}

/**
 * Generate hashname for double constant
 * @param pointer to double
 * @return double hashname
 */
char * ifj_generate_hashname_double(double *value)
{
    char * hashname = (char *) malloc (sizeof(char)*64);
    snprintf(hashname, 64, "//double%.16f", *value);
    return hashname;
}

/**
 * Generate symbol token
 * - if symbol is allready in symbol table, returns reference
 * - when symbol is new, save it into symbol table
 * @param table symbol table for current context
 * @param value symbol to store
 * @return reference to token
 */
token * ifj_generate_token(symbolTable *table, int type)
{
    char * hashname = (char *) malloc (sizeof(char)*3);
    if (type >= 65536)
    {
        fprintf(stderr, "Warning: symbol out of range %d\n", type);
    }
    hashname[0] = type % 256;
    hashname[1] = type / 256;
    hashname[2] = 0;
    token *item = table->get_item(table, hashname, type, NULL);
    if (item)
    {
        free(hashname);
        return item;
    }
    else
    {
        item = ifj_token_new();
        item->value = NULL;
        item->type = type;
        item = table->add_item(table, item, hashname);
        free(hashname);
        return item;
    }
}


/**
 * Generate token for constant int value
 * - if constant is allready in symbol table, returns reference
 * - when constant is new, save it into symbol table
 * @param table symbol table for current context
 * @param value value to store
 * @return reference to stored token
 */
token * ifj_generate_token_int ( symbolTable *table, int value )
{
    char *hashname = ifj_generate_hashname_int(&value);

    //get item - push table, hashname, type and hashing function for int
    token *item = table->get_item(table, hashname, T_INTEGER_C, (char *(*)(void*)) &ifj_generate_hashname_int);
    if (item)
    {
        free(hashname);
        return item;
    }
    else
    {
        item = ifj_token_new();
        item->value = (void*) malloc (sizeof(int));

        *((int*) item->value) = value;
        item->type = T_INTEGER_C;

        item = table->add_item(table, item, hashname);
        free(hashname);
        return item;
    }
}

/**
 * Generate token for constant double value
 * - if constant is allready in symbol table, returns reference
 * - when constant is new, save it into symbol table
 * @param table symbol table for current context
 * @param value value to store
 * @return reference to stored token
 */
token * ifj_generate_token_double ( symbolTable *table, double value )
{
    char *hashname = ifj_generate_hashname_double(&value);

    //get item - push table, hashname, type and hashing function for double
    token *item = table->get_item(table, hashname, T_DOUBLE_C, (char *(*)(void*)) &ifj_generate_hashname_double);
    if (item)
    {
        free(hashname);
        return item;
    }
    else
    {
        item = ifj_token_new();
        item->value = (void*) malloc (sizeof(double));

        *((double *) item->value) = value;
        item->type = T_DOUBLE_C;

        item = table->add_item(table, item, hashname);
        free(hashname);
        return item;
    }
}

/**
 * Generate token for constant string
 * - if constant is allready in symbol table, returns reference
 * - when string is new, save it into symbol table
 * @param table symbol table for current context
 * @param value value to store
 * @return reference to stored token
 */
token * ifj_generate_token_str (symbolTable *table, char *value)
{
    char * hashname = ifj_generate_hashname_str(value);

    //get item - push table, hashname, type and hashing function for string
    token *item = table->get_item(table, hashname, T_STRING_C, (char *(*)(void*)) &ifj_generate_hashname_str);
    if (item)
    {
        free(hashname);
        return item;
    }
    else
    {
        item = ifj_token_new();
        item->value = (void *)strdup(value);

        item->type = T_STRING_C;
        item = table->add_item(table, item, hashname);
        free(hashname);
        return item;
    }
}

/**
 * Generate token for identifier
 * - does NOT save token into symbol table
 * @param table symbol table for current context
 * @param value identifier
 * @return reference to stored token
 */
token *ifj_generate_token_id (char *value)
{
    token *item = ifj_token_new();
    item->value = (void *)strdup(value);
    item->type = T_IDENTIFIER;
    return item;
}

/**
* Generate reserved keyword for lexical analysis
* - save new keyword into symbol table
* @param table symbol table for reserved keywords
* @param value identifier
* @param type token type
* @param method token method for prebuild functions
*/
void ifj_generate_reserved (symbolTable *table, char *value, int type, int method)
{
    token *item = table->get_item(table, value, type, NULL);
    if (!item)
    {
        item = ifj_token_new();
        item->value = (void *)strdup(value);

        item->type = type;
        item->method = method;
        item = table->add_item(table, item, NULL);
    }
}

/**
* Generate temporatu token for executor
* - token must be freed after "set" instruction
* @param datatype {T_INTEGER|T_DOUBLE|T_STRING}
* @param data for chosen datatype
* @return reference to created temporaty token
*/
token *ifj_generate_temp(int datatype, void *data)
{
    token *item = ifj_token_new();
    item->type = T_TMP;
    item->datatype = datatype;
    switch (datatype)
    {
        case T_INTEGER:
            item->value = (void*) malloc (sizeof(int));
            *((int *) item->value) = *data;
            break;
        case T_DOUBLE:
            item->value = (void*) malloc (sizeof(double));
            *((double *) item->value) = *data;
            break;
        case T_STRING:
            item->value = data;
            break;
    }
    return item
}
