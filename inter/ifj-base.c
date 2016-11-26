/* ifj-base.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#include "ifj-inter.h"
#include <stdio.h>

//exit when return code not 0
#define check(arg) rc = arg; if(rc) do { ifj_inter_free(self); return rc;} while(0)

int main (  int argc,
            char **argv)
{
    int rc;
    ifjInter *self = ifj_inter_new(); //create new main struct
    //XXX
    self->debugMode = 0;
    check(self->load(argc, argv, self)); //load input file
    check(syna_run(self)); //run syntactic analysis
    check(exec_run(self)); //run executor

    /*
    //Duplicate context debug...
    token *test = ifj_token_new();
    test->type = T_IDENTIFIER;
    test->value = (void *)strdup("Game.play");
    resolve_identifier(self, self->table, &test, 0);
    token *dupl = duplicate_context(test);

    test = self->table->get_item(test->childTable, "b", T_IDENTIFIER, NULL);
    *((int *)test->data) = 3;

    printf("%d\n", *((int *)test->data) );
    test = resolve_context(self, test, dupl);
    printf("%d\n", *((int *)test->data) );

    dupl->type = T_IDENTIFIER;
    test = self->table->get_item(self->table, "Main", T_IDENTIFIER, NULL);
    test->childTable->add_item(test->childTable, dupl, NULL);

    test = self->table->get_item(dupl->childTable, "b", T_IDENTIFIER, NULL);
    *((int *)test->data) = 2;

    print_table(self->table, 0);
*/


    ifj_inter_free(self);
    return 0;
}
