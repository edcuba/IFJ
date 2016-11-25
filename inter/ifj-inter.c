/* ifj-inter.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Author: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include "ifj-inter.h"
#include "ifj-tables.h"
#include <string.h>

/**
 * Drop interpreter including all tables
 * @param self interpreter
 */
void ifj_inter_free(ifjInter *self)
{
    if(self->syna)
    {
        ifj_syna_free(self->syna);
    }
    if(self->table)
    {
        self->table->drop(self->table);
    }
    ifj_lexa_free(self->lexa_module);
    ifj_drop_list(self->code);
    ifj_stack_drop(self->stack);
    if(self->pushBack)
    {
        ifj_token_free(self->pushBack);
    }
    self->table = NULL;
    free(self);
}

/**
* Initialize global symbol table. Add prebuild functions into class ifj16.
* @param self default interpreter structure with empty symbol table.
*/
void ifj_global_symbol_table_init(ifjInter *self)
{
    if(!self || !self->table)
    {
        if(self->debugMode)
            fprintf(stderr, "Internal error: global symbol table not initialized\n");
        return;
    }
    symbolTable *reserved = self->lexa_module->reserved_words;

    //create token for ifj16 class
    token *class = ifj_generate_token_id("ifj16");
    token *item;
    token *t_str = reserved->get_item(reserved, "String", 0, NULL);
    token *t_int = reserved->get_item(reserved, "int", 0, NULL);

    //save class into global table
    self->table->add_item(self->table, class, NULL);

    class->childTable = ial_symbol_table_new();

    //bind
    class->childTable->parent = self->table;

    //here we have symbol table for ifj16 class
    symbolTable *ifj16 = class->childTable;

    //add methods
    item = ifj_generate_reserved(ifj16, "print", T_IDENTIFIER, IFJ16_PRINT);
    item->dataType = T_VOID;

    item = ifj_generate_reserved(ifj16, "readInt", T_IDENTIFIER, IFJ16_READINT);
    item->dataType = T_INTEGER;

    item = ifj_generate_reserved(ifj16, "readDouble", T_IDENTIFIER, IFJ16_READDOUBLE);
    item->dataType = T_DOUBLE;

    item = ifj_generate_reserved(ifj16, "readString", T_IDENTIFIER, IFJ16_READSTRING);
    item->dataType = T_STRING;

    item = ifj_generate_reserved(ifj16, "find", T_IDENTIFIER, IFJ16_FIND);
    item->dataType = T_INTEGER;
    item->args = ifj_stack_new();
    ifj_stack_push(item->args, t_str);
    ifj_stack_push(item->args, t_str);

    item = ifj_generate_reserved(ifj16, "sort", T_IDENTIFIER, IFJ16_SORT);
    item->dataType = T_STRING;
    item->args = ifj_stack_new();
    ifj_stack_push(item->args, t_str);

    item = ifj_generate_reserved(ifj16, "length", T_IDENTIFIER, IFJ16_LENGTH);
    item->dataType = T_INTEGER;
    item->args = ifj_stack_new();
    ifj_stack_push(item->args, t_str);

    item = ifj_generate_reserved(ifj16, "substr", T_IDENTIFIER, IFJ16_SUBSTR);
    item->dataType = T_STRING;
    item->args = ifj_stack_new();
    ifj_stack_push(item->args, t_str);
    ifj_stack_push(item->args, t_int);
    ifj_stack_push(item->args, t_int);

    item = ifj_generate_reserved(ifj16, "compare", T_IDENTIFIER, IFJ16_COMPARE);
    item->dataType = T_INTEGER;
    item->args = ifj_stack_new();
    ifj_stack_push(item->args, t_str);
    ifj_stack_push(item->args, t_str);
}

/**
 * Holder for some syna - exp constants
 * @return initialized structure for syna
 **/
ifjSyna *ifj_syna_new(ifjInter* inter)
{
    ifjSyna *self = calloc(1, sizeof(ifjSyna));
    if(!self)
    {
        fprintf(stderr,"ERROR: allocating parser!\n");
        exit(99);
    }
    self->semicolon = ifj_generate_token(inter->table, T_SEMICOLON);
    self->lblock = ifj_generate_token(inter->table, T_LBLOCK);
    self->t_less =  ifj_generate_token(inter->table, T_LARRAY);
    self->stack  = ifj_stack_new();
    self->help_stack = ifj_stack_new();
    self->type_stack = ifj_stack_new();
    self->E = ifj_generate_token(inter->table, E_TYPE);
    self->predictCondition = &predictCondition;
    self->predictExpresion = &predictExpresion;
    return self;
}

/**
 * free syna structure
 **/
void ifj_syna_free(ifjSyna *self)
{
    ifj_stack_drop(self->stack);
    ifj_stack_drop(self->help_stack);
    ifj_stack_drop(self->type_stack);
    free(self);
}

/**
 * Create new interpreter structure
 *  - contains initialized default methods and attributes
 * @return interpreter
 */
ifjInter* ifj_inter_new()
{
    ifjInter *self = calloc(1, sizeof(ifjInter));
    if(!self)
    {
        fprintf(stderr,"ERROR: allocating interpreter!\n");
        exit(99);
    }
    self->debugMode = 0;
    self->lexa_module = ifj_lexa_init();
    self->load = &ifj_load;
    self->table = ial_symbol_table_new();
    self->code = ifj_list_new();
    ifj_global_symbol_table_init(self);
    self->stack = ifj_stack_new();
    self->returnCode = 0;
    self->syna = ifj_syna_new(self);
    self->pushBack = NULL;
    self->preLoad = 1;
    self->bootstrap = NULL;
    return self;
}

/**
 * Drop token not connected to hash structure
 * @param item token
 */
void ifj_token_free( token *item )
{
    if(!item)
        return;

    if (item->value == item->data)
    {
        free(item->data);
    }
    else
    {
        if(item->value)
        {
            free((void *)item->value);
        }

        if(item->data)
        {
            free(item->data);
        }
    }
    if(item->args)
    {
        //token themselfs are freed with symbol table
        ifj_stack_drop(item->args);
    }
    if(item->childTable)
    {
        ial_symbol_table_drop(item->childTable);
    }

    free(item);
}

/**
 * Create new initialized token structure
 * @return token
 */
token * ifj_token_new   ()
{
    token *item = calloc (1, sizeof( token ));
    if(!item)
    {
        fprintf(stderr,"ERROR: allocating token!\n");
        exit(99);
    }
    item->type = 0;
    item->value = NULL;
    item->next = NULL;

    item->childTable = NULL;
    item->method = 0;
    item->jump = NULL;
    item->args = NULL;

    item->dataType = 0;
    item->data = NULL;

    return item;
}

/**
 * Create new initialized instruction structure
 * @return instruction
 */
instruction * ifj_instruction_new   ()
{
    instruction *item = calloc (1, sizeof( instruction ));
    if(!item)
    {
        fprintf(stderr,"ERROR: allocating instruction!\n");
        exit(99);
    }
    item->type = 0;
    item->op1 = NULL;
    item->op2 = NULL;
    item->op3 = NULL;
    item->next = NULL;
    return item;
}

char *strdup (const char *s1)
{
    if(!s1)
        return NULL;
    size_t len = strlen(s1);
    char *dupl = malloc((len + 1) * sizeof(char));
    memcpy(dupl, s1, len);
    dupl[len] = 0; //null terminator
    return dupl;
}

void print_unexpected(ifjInter *self, token *item)
{
    fprintf(stderr, "Error: line %d unexpected identifier \"",
            self->lexa_module->line_number);
    switch(item->type)
    {
        case T_IDENTIFIER:
            fprintf(stderr,"%s\"\n", (char *)item->value);
            break;
        case T_INTEGER_C:
            fprintf(stderr,"%d\"\n", *((int *)item->value));
            break;
        case T_DOUBLE_C:
            fprintf(stderr,"%g\"\n", *((double *)item->value));
            break;
        case T_STRING_C:
            fprintf(stderr,"%s\"\n", (char *)item->value);
            break;
        case T_END:
            fprintf(stderr,"EOF\"\n");
            break;
        case T_EOL:
            fprintf(stderr, "EOL\"\n");
            break;
        case T_UNKNOWN:
            fprintf(stderr, "unknown\"\n");
            self->returnCode = 1;
            break;
        default:
            if (item->type < 256)
                fprintf(stderr,"%c\"\n", item->type);
            else
                fprintf(stderr,"code: %d\"\n", item->type);
            break;
    }
}

void print_mistyped(ifjInter *self, token *item, token *expected)
{
    fprintf(stderr, "Error: line %d identifier \"",
            self->lexa_module->line_number);
    switch(item->type)
    {
        case T_INTEGER_C:
            fprintf(stderr,"%d\"", *((int *)item->value));
            break;
        case T_DOUBLE_C:
            fprintf(stderr,"%g\"", *((double *)item->value));
            break;
        case T_STRING_C:
        case T_IDENTIFIER:
            fprintf(stderr,"%s\"", (char *)item->value);
            break;
        default:
            if (item->type < 256)
                fprintf(stderr,"%c\"", item->type);
    }
    fprintf(stderr, " mistyped, expected ");
    if (!expected)
    {
        fprintf(stderr, "nothing\n");
    }
    else switch(expected->dataType)
    {
        case T_INTEGER:
            fprintf(stderr,"int\n");
            break;
        case T_DOUBLE:
            fprintf(stderr,"double\n");
            break;
        case T_STRING:
            fprintf(stderr,"String\n");
            break;
        default:
            fprintf(stderr,"%d\n", expected->dataType);
            break;
    }
}
