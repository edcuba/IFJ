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
#include "ifj-inter.h"

typedef struct item
{
	struct item *next;
	instruction * data;
} linear_item;

struct _linear_list
{
	linear_item *active;
	linear_item *first;
};

linear_list *ifj_list_new ();
int ifj_insert_first (	linear_list *list,
						instruction *item );
int ifj_insert_last (	linear_list *list,
						instruction *item );
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
