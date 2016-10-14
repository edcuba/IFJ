/* ifj-test.c
 * Module for testing functionality of ifj16 interpreter
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#include "ifj-inter.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "ifj-lexa.h"

#define TEST_TOKENS 100000

#define check(name,arg) rc = arg;   if(rc) {printf("[%d] ERROR: %s\n", rc, name); return rc;}\
                                    else printf("[%d] SUCCESS: %s\n", rc, name)

#define printok(what) printf("OK: %s\n",what)
#define printbad(what) printf("WRONG: %s\n",what)

#define check_var(what,var) if(var) printok(what); else printbad(what)
#define check_var_strict(what,var) if(!var) { printbad(what); return 1;} else printok(what)
#define check_token_str(var) if(var) printf("Token.value:%s | Token.type:%d\n", (char*)var->value, var->type); else return 1
#define check_token_int(var) if(var) printf("Token.value:%d | Token.type:%d\n", *((int*)var->value), var->type); else return 1
#define check_token_double(var) if(var) printf("Token.value:%f | Token.type:%d\n", *((double*)var->value), var->type); else return 1


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
static char * generate_value ()
{
    int length = 3+( rand() % 15);
    char *value = (char *) malloc(sizeof(char)*(length+1));
    for (int a = 0; a<length; ++a)
    {
        value[a] = (rand() % 25) + 97;
    }
    return value;
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
        item->type = rand() % 10; //generate type
        item->value = generate_value(); //generate value
        //test if token with same hash is allready in table
        const token *other = self->table->get_item(self->table, item->value, item->type, NULL);
        if(other)
        {
            if(strncmp(item->value, other->value, strlen(item->value))) // not equal = hash problem
            {
                hash_dupl++;
            }
            else //just value generator problem
            {
                generator_problems++;
                ifj_token_free(item);
                continue;
            }
        }
        //push new item
        pushed++;
        self->table->add_item(self->table, item, NULL);
    }
    //print some stats
    printf("Pushed %d tokens | Hash problems: %d | Generator problems: %d\n", pushed, hash_dupl, generator_problems);
    int lost = (TEST_TOKENS - generator_problems - pushed);
    printf("Lost %d\n",lost);

    //check for lost items
    check_var_strict("items check", !(lost));
    printf("Checking items in table...\n");

    int tokens_in_table = self->table->count_items(self->table);
    int missing = pushed - tokens_in_table;

    printf("Tokens in table: %d | Missing: %d\n", tokens_in_table, missing);
    check_var_strict("items check", !missing);

    //drop table and create new one
    check_var_strict("drop table", !self->table->drop(self->table));
    free(self->table->row);
    free(self->table);
    check_var_strict("reinit table", (self->table = ial_symbol_table_new()));
    return 0;
}


/**
 * Check token persistor functionality
 * @param self interpreter structure
 * @returns 0 if successful
 */
static int check_token_persistor (ifjInter *self)
{
    printf("-------- Token persistor --------\n");
    char * str = "SsYoloSwag41";
    printf("Creating string token: %s\n", str);
    token * item = ifj_generate_token_str(self->table, str);
    check_token_str(item);

    printf("Creating identifier token: %s\n", str);
    token * item2 = ifj_generate_token_id(self->table, str);
    check_token_str(item2);

    check_var_strict("tokens differ", (item != item2));

    printf("Creating another string token: %s\n", str);
    token * item3 = ifj_generate_token_str(self->table, str);
    check_token_str(item3);
    check_var_strict("tokens are equal", (item == item3));

    printf("Creating another identifier token: %s\n", str);
    token * item4 = ifj_generate_token_id(self->table, str);
    check_token_str(item4);
    check_var_strict("tokens are equal", (item2 == item4));

    int num = 9;
    printf("Creating int token: %d\n", num);
    token * item5 = ifj_generate_token_int(self->table, num);
    check_token_int(item5);

    double numd = 9.42;
    printf("Creating double token: %f\n", numd);
    token * item6 = ifj_generate_token_double(self->table, numd);
    check_token_double(item6);

    printf("Creating another int token: %d\n", num);
    token * item7= ifj_generate_token_int(self->table, num);
    check_token_int(item7);
    check_var_strict("tokens are equal", (item5 == item7));

    printf("Creating another double token: %f\n", numd);
    token * item8= ifj_generate_token_double(self->table, numd);
    check_token_double(item8);
    check_var_strict("tokens are equal", (item8 == item6));

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
    check ( "token persistor", check_token_persistor(inter)); //check functions for creating new tokens
    //check ("Lexical analysis", check_lexical_analysis(inter));
    //just add tests for your modules here...

    ifj_inter_free(inter);
}