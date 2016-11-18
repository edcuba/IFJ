/* ifj-inter.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Author: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include "ifj-inter.h"
#include <string.h>

/**
 * Drop interpreter including all tables
 * @param self interpreter
 */
void ifj_inter_free(ifjInter *self)
{
    if(self->table)
    {
        self->table->drop(self->table);
    }
    ifj_lexa_free(self->lexa_module);
    ifj_drop_list(self->code);
    ifj_stack_drop(self->stack);
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

    //create token for ifj16 class
    token *class = ifj_generate_token_id("ifj16");

    //save class into global table
    self->table->add_item(self->table, class, NULL);

    class->childTable = ial_symbol_table_new();

    //bind
    class->childTable->parent = self->table;

    //here we have symbol table for ifj16 class
    symbolTable *ifj16 = class->childTable;

    //add methods
    ifj_generate_reserved(ifj16, "print", T_IDENTIFIER, IFJ16_PRINT);

    ifj_generate_reserved(ifj16, "readInt", T_IDENTIFIER, IFJ16_READINT);

    ifj_generate_reserved(ifj16, "readDouble", T_IDENTIFIER, IFJ16_READDOUBLE);

    ifj_generate_reserved(ifj16, "readString", T_IDENTIFIER, IFJ16_READSTRING);

    ifj_generate_reserved(ifj16, "find", T_IDENTIFIER, IFJ16_FIND);

    ifj_generate_reserved(ifj16, "sort", T_IDENTIFIER, IFJ16_SORT);

    ifj_generate_reserved(ifj16, "length", T_IDENTIFIER, IFJ16_LENGTH);

    ifj_generate_reserved(ifj16, "substr", T_IDENTIFIER, IFJ16_SUBSTR);

    ifj_generate_reserved(ifj16, "compare", T_IDENTIFIER, IFJ16_COMPARE);
}

/**
 * Create new interpreter structure
 *  - contains initialized default methods and attributes
 * @return interpreter
 */
ifjInter* ifj_inter_new   ()
{
    ifjInter *self = calloc ( 1, sizeof( ifjInter ));
    if(!self)
    {
        fprintf(stderr,"ERROR: allocating interpreter!\n");
        exit(99);
    }
    self->debugMode = 0;
    self->lexa_module = ifj_lexa_init();
    self->load = &ifj_load;
    self->syna = &syna_run;
    self->table = ial_symbol_table_new();
    self->code = ifj_list_new();
    ifj_global_symbol_table_init(self);
    self->stack = ifj_stack_new();
    return self;
}

/**
 * Drop token not connected to hash structure
 * @param item token
 */
void ifj_token_free( token *item)
{
    if(!item)
        return;

    if(item->value)
        free( (void*) item->value);

    if(item->childTable)
    {
        ial_symbol_table_drop(item->childTable);
    }

    if(item->data)
    {
        free(item->data);
    }

    if(item->args)
    {
        //token themselfs are freed with symbol table
        free(item->args);
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
