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
#include "tests/lexa.c"
#include "ifj-util.h"

#define TEST_TOKENS 1000

#define check(name,arg) rc = arg;   if(rc) {printf("[%d] ERROR: %s\n", rc, name); return rc;}\
                                    else printf("[%d] SUCCESS: %s\n", rc, name)

#define printok(what) printf("OK: %s\n",what)
#define printbad(what) printf("WRONG: %s\n",what)

#define check_var(what,var) if(var) printok(what); else printbad(what)
#define check_var_strict(what,var) if(!var) { printbad(what); return 1;} else printok(what)
#define check_token_str(var) if(var) printf("Token.value:%s | Token.type:%d\n", (char*)var->value, var->type); else return 1
#define check_token_int(var) if(var) printf("Token.value:%d | Token.type:%d\n", *((int*)var->value), var->type); else return 1
#define check_token_double(var) if(var) printf("Token.value:%f | Token.type:%d\n", *((double*)var->value), var->type); else return 1
#define check_token_symbol(var) if(var) printf("Token.value:%c | Token.type:%d\n", var->type, var->type); else return 1


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
    check_var("lexa module",self->lexa_module);
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
    printf("\nCreating string token: %s\n", str);
    token * item = ifj_generate_token_str(self->table, str);
    check_token_str(item);

    printf("\nCreating identifier token: %s\n", str);
    token * item2 = ifj_generate_token_id(self->table, str);
    check_token_str(item2);

    check_var_strict("tokens differ", (item != item2));

    printf("\nCreating another string token: %s\n", str);
    token * item3 = ifj_generate_token_str(self->table, str);
    check_token_str(item3);
    check_var_strict("tokens are equal", (item == item3));

    printf("\nCreating another identifier token: %s\n", str);
    token * item4 = ifj_generate_token_id(self->table, str);
    check_token_str(item4);
    check_var_strict("tokens are equal", (item2 == item4));

    int num = 9;
    printf("\nCreating int token: %d\n", num);
    token * item5 = ifj_generate_token_int(self->table, num);
    check_token_int(item5);

    double numd = 9.42;
    printf("\nCreating double token: %f\n", numd);
    token * item6 = ifj_generate_token_double(self->table, numd);
    check_token_double(item6);

    printf("\nCreating another int token: %d\n", num);
    token * item7= ifj_generate_token_int(self->table, num);
    check_token_int(item7);
    check_var_strict("tokens are equal", (item5 == item7));

    printf("\nCreating another double token: %f\n", numd);
    token * item8= ifj_generate_token_double(self->table, numd);
    check_token_double(item8);
    check_var_strict("tokens are equal", (item8 == item6));

    char symbol = '(';
    printf("\nCreating symbol token: %c\n", symbol);
    token * item9= ifj_generate_token(self->table, symbol);
    check_token_symbol(item9);

    char symbol2 = ')';
    printf("\nCreating symbol token: %c\n", symbol2);
    token * item10= ifj_generate_token(self->table, symbol2);
    check_token_symbol(item10);
    check_var_strict("tokens differ", (item != item2));

    printf("\nCreating another symbol token: %c\n", symbol);
    token * item11= ifj_generate_token(self->table, symbol);
    check_token_symbol(item11);
    check_var_strict("tokens are equal", (item9 == item11));


    return 0;
}

static int check_reserved (ifjInter *self)
{
    printf("-------- Reserved symbols persistor --------\n");
    char * str = "while";
    printf("\nCreating reserved 42: %s\n", str);
    token * item = ifj_generate_token_id(self->table, str);
    item->type = 42;
    check_token_str(item);
    printf("\nGetting token\n");
    token *got = self->table->get_item(self->table, str, 0, NULL);
    check_token_str(got);

    char * str2 = "do";
    printf("\nCreating reserved 69: %s\n", str2);
    token * item2 = ifj_generate_token_id(self->table, str2);
    item2->type = 69;
    check_token_str(item2);
    printf("\nGetting token\n");
    token *got2 = self->table->get_item(self->table, str2, 0, NULL);
    check_token_str(got2);


    return 0;
}

static int check_linear_list(ifjInter *self)
{
    printf("-------- Linear list --------\n");
    linear_list *inputList = ifj_new_list();
    check_var("List initialized", inputList != NULL);
    ifj_insert_first(inputList, NULL);
    check_var("First item inserted", inputList->first != NULL);
    ifj_insert_last(inputList, NULL);
    check_var("Insert item to last position", inputList->first->next != NULL);
    ifj_set_active_first(inputList);
    check_var("Set first item to active", inputList->active == inputList->first);
    ifj_set_active_next(inputList);
    check_var("Set next item to active", inputList->active == inputList->first->next);

    linear_item *tempItem = inputList->first;
    ifj_insert_first(inputList, NULL);
    check_var("Input next item to first position", tempItem != inputList->first);
    check_var_strict("Drop list", ifj_drop_list(inputList) == 0);
    free(inputList);

    token *tok = ifj_generate_token_id(self->table, "number");
    inputList = ifj_new_list();
    ifj_insert_last(inputList, tok);
    check_var_strict("Insert item to last position with token", inputList->first->data == tok);
    check_token_str(tok);


    return 0;
}


int main(int argc, char **argv)
{
    int rc;
    srand(time(NULL));

    ifjInter *inter = ifj_inter_new(); //create new main struct
    inter->debugMode = 1; //enable debug mode
    check ( "File loading", inter->load(argc, argv, inter));
    check ( "inter struct", check_inter(inter)); //check structore initialization
    check ( "symbol table", check_symbol_table(inter)); //check symbol table functionality
    check ( "token persistor", check_token_persistor(inter)); //check functions for creating new tokens
    check ( "reserved symbols", check_reserved(inter)); //check reserved symbol table for lexa
    check ( "Lexical analysis", check_lexical_analysis(inter));
    check ( "Linear list", check_linear_list(inter));
    //just add tests for your modules here...

    ifj_inter_free(inter);
}
