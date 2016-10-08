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
	unsigned int 	( *hash_func	)();
	const token 	*( *add_item	)( symbol_table*, token* );
	const token		*( *get_item	)( symbol_table*, const char*);
	int 			( *count_items	)( symbol_table*);
	int				( *drop			)( symbol_table*);
};

symbol_table *ial_symbol_table_new();

int ial_symbol_table_count_items( symbol_table * self);

int ial_symbol_table_drop ( symbol_table *self);

const token *ial_symbol_table_add_item	( 	symbol_table *self,
	 										token *item );

const token *ial_symbol_table_get_item	( 	symbol_table *self,
	 										const char *name );

unsigned int ial_symbol_table_hash_func ( const char *name );


#endif
