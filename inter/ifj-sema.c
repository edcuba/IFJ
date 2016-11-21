/* ifj-sema.c
*
* Copyright (C) 2016 SsYoloSwag41 Inc.
* Authors: Eduard Cuba
*/

#include "ifj-sema.h"
#include "ifj-inter.h"
#include "string.h"
#include "stdlib.h"

/**
* @param self global struct - we need reference to global symbol table
* @param table symbol table from current context
* @param item reference pointer for token to be resolved
* @param isDefiniton bool if current rule is type definition
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
        if(isDefiniton)
        {
           fprintf(stderr, "ERROR: Identifier \"%s\" defined in foreign class!\n",
                   (char *)seek->value);
            ifj_token_free(seek);
            *item = NULL;
            self->returnCode = 3;
            return 3;
        }

        //extract class and id
        char *full_name = strdup((char *)seek->value); //copy id for processing
        full_name[(void *)breakPoint - seek->value] = 0; //split id in half

        char *id_class = strdup(full_name); //copy class
        char *id_id = strdup(breakPoint + 1); //copy id
        free(full_name);

        //find requested symbol table

        //search for class in global context
        token *class = self->table->get_item(self->table,
                                             id_class,
                                             T_IDENTIFIER,
                                             NULL);
        if(!class || !class->childTable)
        {
            fprintf(stderr, "ERROR: Class \"%s\" undefined!\n", id_class);
            free(id_class);
            free(id_id);
            ifj_token_free(seek);
            *item = NULL;
            self->returnCode = 3;
            return 3;
        }

        token *child = self->table->get_item(class->childTable,
                                             id_id,
                                             T_IDENTIFIER,
                                             NULL);

        if(!child)
        {
            fprintf(stderr, "ERROR: Identifier \"%s\" from class \"%s\" undefined!\n",
                    id_id,
                    id_class);
            free(id_class);
            free(id_id);
            ifj_token_free(seek);
            *item = NULL;
            self->returnCode = 3;
            return 3;
        }

        free(id_id);
        free(id_class);
        ifj_token_free(seek);

        *item = child;
        return 0;

    }
    if(isDefiniton)
    { //type cant be redefined in current context
        token *prev = table->get_item(table, seek->value, T_IDENTIFIER, NULL);
        if (prev) //redefined
        {
            ifj_token_free(seek);
            *item = NULL;
            fprintf(stderr, "ERROR: Identifier \"%s\" redefined!\n", (char*)seek->value);
            self->returnCode = 3;
            return 3;
        }
        else
        { // proper definition
            table->add_item(table, seek, NULL);
            return 0;
        }
    }
    else
    { //find identifier in avalible contexts
        symbolTable *context = table;
        while(context)
        {
            token *prev = context->get_item(context, seek->value, T_IDENTIFIER, NULL);
            if (prev) //found
            {
                ifj_token_free(seek);
                *item = NULL;
                // return found token
                *item = prev;
                return 0;
            }
            context = context->parent;
        }
        fprintf(stderr, "ERROR: Identifier \"%s\" undefined!\n", (char*)seek->value);
        ifj_token_free(seek);
        *item = NULL;
        self->returnCode = 3;
        return 3;
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
        return 0;
    }
    else if(first->dataType == T_DOUBLE && second->dataType == T_INTEGER)
    {
        return 0;
    }
    else if(first->dataType == T_INTEGER && second->dataType == T_DOUBLE)
    {
        return 0;
    }
    return 1;
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
