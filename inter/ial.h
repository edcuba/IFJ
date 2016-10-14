/* ial.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#ifndef IAL_H
#define IAL_H

#include "ifj-inter.h"

/**
 * struct for symbol table
 * @param parent parent table or NULL when global
 * @param size rows in table
 * @param row hash table structure - pointer to array of pointers to token
 * @param add_item pointer to func add_item(table, item, hashname)
 * @param get_item pointer to func get_item(table, hashname, type, &hash_func)
 * hash_func pointer to func with return type char* with void pointer argument
 * @param count_items - pointer to func returning count of active items in table
 * @param drop pointer to func used to free all items in table
 */
struct _symbol_table
{
	symbol_table *parent;
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

int ifj_find (	const char *s1,
				const char *search );

const char * ifj_sort ( const char *inputString );

#endif
