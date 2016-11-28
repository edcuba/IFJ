/* ial.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#ifndef IAL_H
#define IAL_H

#include "ifj_inter.h"

/**
 * struct for symbol table
 * @param parent parent table or NULL when global
 * @param size rows in table
 * @param row hash table structure - pointer to array of pointers to token
 */
struct _symbolTable
{
	symbolTable *parent;
	unsigned int size;
	token **row;
	unsigned int identifiers;
};

symbolTable *ial_symbol_table_new(int size);

int ial_symbol_table_drop ( symbolTable *self);

void ial_symbol_table_add_item( symbolTable *self,
	 							token *item,
								char * hashname);

token *ial_symbol_table_get_item( 	symbolTable *self,
	 								const char *hashname,
									int type,
									char *(*generate_hashname)(void*));

unsigned int ial_symbol_table_hash_func (const char *hashname, unsigned int size);

token *ifj_find (	const char *s1,
					const char *search );

token *ifj_sort ( const char *s1 );

#endif
