/* ifj-util.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Robert Kolcun <xkolcu00@stud.fit.vutbr.cz>
 *          Jan Demcak < DOPISAT >
 *          Ondrej Kurak < DOPISAT >
 */

#ifndef IFJ_UTIL_H
#define IFJ_UTIL_H

#include "ial.h"
#include "ifj-inter.h"
#include <stdbool.h>


struct _token_stack
{
	token **elements;
	int top;	// -1 for empty stack
	int size;	// default size is 32
};

struct _linear_list
{
	instruction *active;
	instruction *first;
};

// Stack functions
token_stack *ifj_stack_new ();
void ifj_stack_push (	token_stack *inStack,
						token *inToken );
token *ifj_stack_pop ( token_stack *inStack );
token *ifj_stack_top ( token_stack *inStack );
bool ifj_stack_full ( token_stack *inStack );
bool ifj_stack_empty ( token_stack *inStack );
void ifj_stack_drop ( token_stack *inStack );

// Linear list functions
linear_list *ifj_list_new ();
int ifj_insert_first (	linear_list *list,
						int inputType,
						token *oper1,
						token *oper2,
						token *oper3 );
int ifj_insert_last (	linear_list *list,
						int inputType,
						token *oper1,
						token *oper2,
						token *oper3 );
void ifj_drop_list ( linear_list *list );
void ifj_set_active_first ( linear_list *list );
void ifj_set_active_last ( linear_list *list );
void ifj_set_active_next ( linear_list *list );

// IFJ16 functions
token *ifj_read_int ();
token *ifj_read_double ();
token *ifj_read_string ();

void ifj_print ( const char *input );

int ifj_length ( const char *inputString );

char * ifj_substr (	const char *inputString,
					int i,
					int n );

int ifj_compare (	const char *s1,
					const char *s2 );

#endif
