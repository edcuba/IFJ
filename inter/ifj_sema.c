/* ifj-sema.c
*
* Copyright (C) 2016 SsYoloSwag41 Inc.
* Authors: Eduard Cuba
*/

#include "ifj_sema.h"
#include "ifj_inter.h"
#include "string.h"
#include "stdlib.h"

/**
* @param self global struct - we need reference to global symbol table
* @param table symbol table from current context
* @param item reference pointer for token to be resolved
* @param isDefiniton 1 if current rule is type definition, 0 if not, -1 if clone
* @return 0 if success, 3 if redefined/undefined
*/
int resolve_identifier(ifjInter *self,
                       symbolTable *table,
                       token **item,
                       int isDefiniton)
{
    token *seek = *item;
    char *breakPoint = strchr((char *)seek->value, '.');
    if(breakPoint) //full classifier
    {
        if(isDefiniton == 1) //be aware of -1!
        {
           fprintf(stderr, "ERROR: line %d Identifier \"%s\" defined in foreign class!\n",
                   self->lexa_module->line_number,
                   (char *)seek->value);
            ifj_token_free(seek);
            *item = NULL;
            self->returnCode = 3;
            return 0;
        }

        //extract class and id
        char *full_name = strdup((char *)seek->value); //copy id for processing
        full_name[(void *)breakPoint - seek->value] = 0; //split id in half

        char *id_class = strdup(full_name); //copy class
        char *id_id = strdup(breakPoint + 1); //copy id
        free(full_name);

        //find requested symbol table

        //search for class in global context
        token *class = ial_symbol_table_get_item(self->table,
                                                 id_class,
                                                 T_IDENTIFIER,
                                                 NULL);
        if(!class || !class->childTable)
        {
            fprintf(stderr, "ERROR: line %d Class \"%s\" undefined!\n",
                    self->lexa_module->line_number,
                    id_class);
            free(id_class);
            free(id_id);
            ifj_token_free(seek);
            *item = NULL;
            self->returnCode = 3;
            return 0;
        }

        token *child = ial_symbol_table_get_item(class->childTable,
                                                 id_id,
                                                 T_IDENTIFIER,
                                                 NULL);

        if(!child)
        {
            fprintf(stderr, "ERROR: line %d Identifier \"%s\" from class \"%s\" undefined!\n",
                    self->lexa_module->line_number,
                    id_id,
                    id_class);
            free(id_class);
            free(id_id);
            ifj_token_free(seek);
            *item = NULL;
            self->returnCode = 3;
            return 0;
        }

        free(id_id);
        free(id_class);
        if(!isDefiniton) //dont free when resolving clone
        {
            ifj_token_free(seek);
        }

        *item = child;
        return 1;

    }
    if(isDefiniton == 1) //be aware of -1!
    { //type cant be redefined in current context
        token *prev = ial_symbol_table_get_item(table, seek->value, T_IDENTIFIER, NULL);
        token *super = NULL;
        if(table->parent && table->parent != self->table) //if it is function
        {
            //check parent context
            super = ial_symbol_table_get_item(table->parent, seek->value, T_IDENTIFIER, NULL);

            //function or static variable?
            if(super && !super->childTable)
            {
                super = NULL; //just variable
            }
        }
        if (prev || super) //redefined
        {
            fprintf(stderr, "ERROR: line %d Identifier \"%s\" redefined!\n",
                    self->lexa_module->line_number,
                    (char*)seek->value);
            ifj_token_free(seek);
            *item = NULL;
            self->returnCode = 3;
            return 0;
        }
        else
        { // proper definition
            ial_symbol_table_add_item(table, seek, NULL);
            return 1;
        }
    }
    else
    { //find identifier in avalible contexts
        symbolTable *context = table;
        while(context)
        {
            token *prev = ial_symbol_table_get_item(context, seek->value, T_IDENTIFIER, NULL);
            if (prev == seek) //allready resolved
            {
                return 1;
            }
            if (prev) //found
            {
                if(!isDefiniton) //dont free when resolving clone
                {
                    ifj_token_free(seek);
                }
                // return found token
                *item = prev;
                return 1;
            }
            context = context->parent;
        }
        fprintf(stderr, "ERROR: line %d Identifier \"%s\" undefined!\n",
                self->lexa_module->line_number,
                (char*)seek->value);
        ifj_token_free(seek);
        *item = NULL;
        self->returnCode = 3;
        return 0;
    }
}

/**
 * Check if types are convertable
 * @param first first idenfifier
 * @param second second idenfifier
 * @return 0 if convertable
 **/
int check_typing(token *first, token *second)
{
    if(first->dataType == second->dataType)
    {
        return 1;
    }
    else if(first->dataType == T_DOUBLE && second->dataType == T_INTEGER)
    {
        return 1;
    }
    else if(first->dataType == T_INTEGER && second->dataType == T_DOUBLE)
    {
        return 1;
    }
    return 0;
}

void print_table(symbolTable *table, int level)
{
    for (unsigned int i = 0; i < table->size; ++i)
	{
		if (table->row[i] == NULL)
		{
			continue;
		}

		token *item = table->row[i];
		while (item != NULL)
		{
            if(item->type == T_IDENTIFIER)
            {
                for(int j = 0; j <= level; ++j)
                    fprintf(stderr,"\t");
                switch (item->dataType) {
                    case T_INTEGER:
                        fprintf(stderr, "int ");
                        break;
                    case T_STRING:
                        fprintf(stderr, "String ");
                        break;
                    case T_DOUBLE:
                        fprintf(stderr, "double ");
                        break;
                }
                fprintf(stderr,"%s", (char *)item->value);
                if(item->args)
                {
                    fprintf(stderr, "(");
                    for(int i = 0; i <= item->args->top; ++i)
                    {
                        token *sub = item->args->elements[i];
                        switch (sub->dataType) {
                            case T_INTEGER:
                                fprintf(stderr, " int ");
                                break;
                            case T_STRING:
                                fprintf(stderr, " String ");
                                break;
                            case T_DOUBLE:
                                fprintf(stderr, " double ");
                                break;
                        }
                        fprintf(stderr,"%s", (char *)sub->value);

                    }
                    fprintf(stderr, ")");
                }
                else if(item->childTable)
                {
                    fprintf(stderr, "()");
                }
                if(item->data)
                {
                    switch (item->dataType) {
                        case T_INTEGER:
                            fprintf(stderr, " = %d", *((int *) item->data));
                            break;
                        case T_STRING:
                            fprintf(stderr, " = %s", (char *) item->data);
                            break;
                        case T_DOUBLE:
                            fprintf(stderr, " = %g", *((double *) item->data));
                            break;
                        }
                }
                fprintf(stderr, "\n");
                if(item->childTable)
                {
                    print_table(item->childTable, level + 1);
                }
            }
			item = item->next;
		}
	}
}

/**
 * Duplicate context of function - for recursion - identifiers only!
 * @param item original function identifier
 * @return temporary token with duplicated symbol table, return value and args
 **/
token *duplicate_context(token *item)
{
    token *dupl = ifj_generate_temp(T_VOID, NULL);
    dupl->dataType = item->dataType;
    dupl->value = strdup((char *) item->value);
    dupl->jump = item->jump;
    dupl->method = item->method;
    symbolTable *table = item->childTable;
    int size = (table->identifiers - 1) * 2;
    symbolTable *newTable = ial_symbol_table_new((size <= 0)? 1 : size);
    newTable->identifiers = table->identifiers;
    newTable->parent = table->parent;
    dupl->childTable = newTable;

    //Duplicate symbol table
    for (unsigned int i = 0; i < table->size; ++i)
	{
		if (table->row[i] == NULL)
		{
			continue;
		}
		token *source = table->row[i];

		while (source != NULL)
		{
            if(source->type == T_IDENTIFIER)
            {
                token *clone;
                if(source->dataType == T_STRING)
                {
                    clone = ifj_generate_temp(source->dataType, strdup((char *)source->data));
                }
                else
                {
                    clone = ifj_generate_temp(source->dataType, source->data);
                }
                clone->type = T_IDENTIFIER;
                clone->value = strdup((char *) source->value);
                ial_symbol_table_add_item(newTable, clone, NULL);
            }
			source = source->next;
		}
	}

    //duplicate function args
    if(item->args && !ifj_stack_empty(item->args))
    {
        dupl->args = ifj_stack_new();
        for(int i = 0; i <= item->args->top; ++i)
        {
            token *arg = item->args->elements[i];
            arg = ial_symbol_table_get_item(newTable, arg->value, T_IDENTIFIER, NULL);
            ifj_stack_push(dupl->args, arg);
        }
    }
    return dupl;
}

/**
 * Replace identifier A from source context with identifier A from target context
 * @param self global structure
 * @param item identifier A
 * @param target target context
 * @return identifier A from target context
 **/
inline token *resolve_context(ifjInter *self, token *item, token *target)
{
    if(!item || item->type != T_IDENTIFIER || item->method || !target)
    {
        return item; //we dont resolve contexts for constants or statics
    }
    //resolve but dont free item
    resolve_identifier(self, target->childTable, &item, -1);
    return item;
}
