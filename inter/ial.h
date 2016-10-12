/* ial.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#ifndef IAL_H
#define IAL_H

#include "ifj-inter.h"

//struct for symbol table
struct _symbol_table
{
	unsigned int size;
	token **row;
	token 	*( *add_item	)( symbol_table*, token*, char*);
	token	*( *get_item	)( symbol_table*, const char*, int, char *(*)(void*));
	int 	( *count_items	)( symbol_table*);
	int		( *drop			)( symbol_table*);
};

symbol_table *ial_symbol_table_new();

int ial_symbol_table_count_items( symbol_table * self);

int ial_symbol_table_drop ( symbol_table *self);

token *ial_symbol_table_add_item( 	symbol_table *self,
	 								token *item,
									char * hashname);

token *ial_symbol_table_get_item( 	symbol_table *self,
	 								const char *hashname,
									int type,
									char *(*generate_hashname)(void*));

unsigned int ial_symbol_table_hash_func ( const char *hashname );


#endif
