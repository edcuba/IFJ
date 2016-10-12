/* ifj-test.c
 * Module for testing functionality of ifj16 interpreter
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#include "ifj-inter.h"
#include "tests/lexa.c"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define TEST_TOKENS 100000

#define check(name,arg) rc = arg;   if(rc) {printf("[%d] ERROR: %s\n", rc, name); return rc;}\
                                    else printf("[%d] SUCCESS: %s\n", rc, name)

#define printok(what) printf("OK: %s\n",what)
#define printbad(what) printf("WRONG: %s\n",what)

#define check_var(what,var) if(var) printok(what); else printbad(what)
#define check_var_strict(what,var) if(!var) { printbad(what); return 1;} else printok(what)


/**
 * Check default interpreter attributes
 * @param self interpreter
 * @returns 0 if successful
 */
static int check_inter (ifjInter *self)
{
    printf("-------- Checking inter structure ---------\n");
    if (!self)
    {
        printbad("inter struct undefined");
        return 1;
    }
    check_var("symbol table init", self->table);
    check_var("load func",self->load);
    check_var("lexa func",self->lexa);
    check_var("syna func",self->syna);
    check_var("sema func",self->sema);
    return 0;
}

/**
 * Generate some 3-13 char string for testing purposes
 * @returns generated string
 */
static char * generate_name ()
{
    int length = 3+( rand() % 10);
    char *name = (char *) malloc(sizeof(char)*(length+1));
    for (int a = 0; a<length; ++a)
    {
        name[a] = (rand() % 25) + 97;
    }
    return name;
}

/**
 * Check symbol table functionality
 * @param self ifjInter structure
 * @returns 0 if successful
 */
static int check_symbol_table(ifjInter *self)
{
    printf("-------- Testing symbol table ---------\n");
    if (!self->table)
    {
        printbad("symbol table undefined");
        return 1;
    }
    //test if default functions are initialized
    check_var_strict("add token function",self->table->add_item);
    check_var_strict("get token function",self->table->get_item);
    check_var_strict("drop table function",self->table->drop);

    //save TEST_TOKENS tokens in symbol table
    printf("Pushing %d tokens\n", TEST_TOKENS);
    int hash_dupl = 0;
    int generator_problems = 0;
    int pushed = 0;
    for(int i = 0; i < TEST_TOKENS; ++i)
    {
        token *item = ifj_token_new(); //create new token
        item->name = generate_name(); //generate name
        item->type = rand() % 10; //generate type
        item->value = generate_name(); //generate value
        //test if token with same hash is allready in table
        const token *other = self->table->get_item(self->table, item->name);
        if(other)
        {
            if(strcmp(item->name, other->name)) // not equal = hash problem
            {
                hash_dupl++;
            }
            else //just name generator problem
            {
                generator_problems++;
                continue;
            }
        }
        //push new item
        pushed++;
        self->table->add_item(self->table, item);
    }
    //print some stats
    printf("Pushed %d tokens | Hash problems: %d | Generator problems: %d\n", pushed, hash_dupl, generator_problems);
    int lost = (TEST_TOKENS - generator_problems - pushed);
    printf("Lost %d\n",lost);

    //check for lost items
    check_var_strict("items check", lost);
    printf("Checking items in table...\n");

    int tokens_in_table = self->table->count_items(self->table);
    int missing = pushed - tokens_in_table;

    printf("Tokens in table: %d | Missing: %d\n", tokens_in_table, missing);
    check_var_strict("items check", missing);

    //drop table and create new one
    check_var_strict("drop table", self->table->drop(self->table));
    check_var_strict("reinit table", (self->table = ial_symbol_table_new()));
    return 0;
}

/**
 * Check reserved table functionality
 * @param self ifjInter structure
 * @returns 0 if successful
 */
static int check_reserved_table(ifjInter *self) {

    printf("-------- Testing reserved table ---------\n");
    ifj_lexa *lexa = ifj_lexa_init();
    if (lexa == NULL) {
        printf("Reserved table not initialized");
        return 1;
    }

    ifj_lexa_add_reserved(lexa, "while", T_WHILE);
    if (ifj_lexa_is_reserved(lexa, "while") != T_WHILE) {
        printf("Reserved word \"while\" should be in table");
        return 1;
    }

    if (ifj_lexa_is_reserved(lexa, "pes") != -1) {
        printf("Reserved word \"pes\" should be in table");
        return 1;
    }

    ifj_lexa_add_reserved(lexa, "if", T_IF);

    if (ifj_lexa_is_reserved(lexa, "while") != T_WHILE) {
        printf("Reserved word \"while\" should be in table");
        return 1;
    }

    if (ifj_lexa_is_reserved(lexa, "if") != T_IF) {
        printf("Reserved word \"if\" should be in table");
        return 1;
    }

    if (ifj_lexa_is_reserved(lexa, "pes") != -1) {
        printf("Reserved word \"pes\" should be in table");
        return 1;
    }

    ifj_lexa_free(lexa);

    return 0;
}

int main()
{
    int rc;
    srand(time(NULL));

    ifjInter *inter = ifj_inter_new(); //create new main struct
    inter->debugMode = 1; //enable debug mode
    check ( "inter struct", check_inter(inter)); //check structore initialization
    check ( "symbol table", check_symbol_table(inter)); //check symbol table functionality
    check ( "reserved table", check_reserved_table(inter));
    check ("Lexical analysis", check_lexical_analysis(inter));
    //just add tests for your modules here...
}
