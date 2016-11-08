/* ifj-sema.c
*
* Copyright (C) 2016 SsYoloSwag41 Inc.
* Authors: Eduard Cuba
*/

#include "ifj-sema.h"
#include "ifj-inter.h"

/**
* @param table symbol table from current context
* @param item reference pointer for token to be resolved
* @param isDefiniton bool if current rule is type definition
* @return 0 if success, 3 if redefined/undefined
*/
int resolve_identifier(symbolTable *table,
                       token **item,
                       int isDefiniton)
{
    //TODO resolve full qualifiers
    token *seek = *item;
    if(isDefiniton)
    { //type cant be redefined in current context
        token *prev = table->get_item(table, seek->value, T_IDENTIFIER, NULL);
        if (prev) //redefined
        {
            ifj_token_free(seek);
            *item = NULL;
            fprintf(stderr, "ERROR: Identifier %s redefined!\n", (char*)seek->value);
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
        ifj_token_free(seek);
        *item = NULL;
        fprintf(stderr, "ERROR: Identifier %s undefined!\n", (char*)seek->value);
        return 3;
    }
}

int sema_run(ifjInter *self)
{
    return 0;
}
