/* ifj-sema.c
*
* Copyright (C) 2016 SsYoloSwag41 Inc.
* Authors: Eduard Cuba
*/

#include "ifj-sema.h"
#include "ifj-inter.h"

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
            //TODO free token
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
                //TODO free token
                // return found token

                return 0;
            }
            context = context->parent;
        }
        //not found
        //TODO free token
        fprintf(stderr, "ERROR: Identifier %s undefined!\n", (char*)seek->value);
        return 3;
    }
}

int sema_run(ifjInter *self)
{
    return 0;
}
