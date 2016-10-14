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
 * @returns string hashname
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
 * @returns integer hashname
 */
char * ifj_generate_hashname_int(int *value)
{
    char * hashname = (char *) malloc (sizeof(char) * 16);
    snprintf(hashname, 16, "//int%d", *value);
    return hashname;
}

/**
 * Generate hashname for double constant
 * @param pointer to double
 * @returns double hashname
 */
char * ifj_generate_hashname_double(double *value)
{
    char * hashname = (char *) malloc (sizeof(char) * 32);
    snprintf(hashname, 32, "//double%f", *value);
    return hashname;
}

token * ifj_generate_token(symbol_table *table, int type) {

}


/**
 * Generate token for constant int value
 * - if constant is allready in symbol table, returns reference
 * - when constant is new, save it into symbol table
 * @param table symbol table for current context
 * @param value value to store
 * @returns reference to stored token
 */
token * ifj_generate_token_int ( symbol_table *table, int value )
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
 * @returns reference to stored token
 */
token * ifj_generate_token_double ( symbol_table *table, double value )
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
 * @returns reference to stored token
 */
token * ifj_generate_token_str (symbol_table *table, char *value)
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
        item->value = (void *)strndup(value, strlen(value));

        item->type = T_STRING_C;
        item = table->add_item(table, item, hashname);
        free(hashname);
        return item;
    }
}

/**
 * Generate token for identifier
 * - if identifier is allready in symbol table, returns reference
 * - when identifier is new, save it into symbol table
 * @param table symbol table for current context
 * @param value identifier
 * @returns reference to stored token
 */
token * ifj_generate_token_id (symbol_table *table, char *value)
{
    //get item - push table, hashname, type and no hashing function
    token *item = table->get_item(table, value, T_IDENTIFIER, NULL);
    if (item)
    {
        return item;
    }
    else
    {
        item = ifj_token_new();
        item->value = (void *)strndup(value, strlen(value));

        item->type = T_IDENTIFIER;
        item = table->add_item(table, item, NULL);
        return item;
    }
}
