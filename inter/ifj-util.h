/* ifj-util.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Robert Kolcun <xcubae00@stud.fit.vutbr.cz>
 *          Jan Demcak < DOPISAT >
 *          Ondrej Kurak < DOPISAT >
 */

 #ifndef IFJ_UTIL_H
 #define IFJ_UTIL_H

#include "ial.h"

typedef struct item
{
	struct item *next;
	token * data;
} linear_item;

typedef struct
{
	linear_item *active;
	linear_item *first;
} linear_list;

linear_list *ifj_new_list ();
int ifj_insert_first (	linear_list *list,
						token *item );
int ifj_insert_last (	linear_list *list,
						token *item );
int ifj_drop_list ( linear_list *list );
void ifj_set_active_first ( linear_list *list );
void ifj_set_active_last ( linear_list *list );
void ifj_set_active_next ( linear_list *list );

int ifj_read_int ();
double ifj_read_double ();
const char * ifj_read_string ();

void ifj_print ( const char *input );

int ifj_length ( const char *inputString );

char * ifj_substr (	const char *inputString,
					int i,
					int n );

int ifj_compare (	const char *s1,
					const char *s2 );

#endif