/* ial.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 * 			Robert Kolcun <xkolcu00@stud.fit.vurbr.cz>
 */

#include "ial.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TABLE_ROWS 4241

/**
 * Add new item
 * - store pointer to token in hash table
 * @param self symbol table
 * @param item token
 * @returns stored token pointer or NULL when unsuccessful
 */
token *ial_symbol_table_add_item	( 	symbol_table *self,
	 									token *item,
										char *hashname)
{
	unsigned int hash;
	if(hashname)
		hash = ial_symbol_table_hash_func(hashname);
	else
		hash = ial_symbol_table_hash_func(item->value);

	item->next = self->row[hash];
	self->row[hash] = item;

	return item;
}

/**
 * Get item by hashname
 * - store pointer to token in hash table
 * @param self symbol table
 * @param hashname token value or generated hashname
 * @param type target type
 * @param generate_hashname function generating hashname for target type
 * @returns token pointer or NULL when unsuccessful
 */
token *ial_symbol_table_get_item	( 	symbol_table *self,
	 									const char *hashname,
										int type,
										char *(*generate_hashname)(void*))
{
	unsigned int hash = ial_symbol_table_hash_func(hashname);

	token *item = self->row[hash];

	while (item != NULL)
	{
		if(item->type != type)
		{
			item = item->next;
			continue;
		}

		if (generate_hashname)
		{
			char *h_name = generate_hashname((void*)item->value);
			if (strlen(hashname) == strlen(h_name) && !strcmp(h_name, hashname) )
			{
				free(h_name);
				return item;
			}
			free(h_name);
		}
		else if (strlen((char *) item->value) == strlen (hashname) && !strcmp(item->value, hashname))
		{
			return item;;
		}

		item = item->next;
	}

	unsigned int hash = ial_symbol_table_hash_func(item->name);

	item->next = self->row[hash];
	self->row[hash] = item;

	return item;
}

/**
 * Get item by name
 * - store pointer to token in hash table
 * - allows to access pointer later
 * @param self symbol table
 * @param name token name
 * @returns token pointer or NULL when unsuccessful
 */
const token *ial_symbol_table_get_item	( 	symbol_table *self,
	 										const char *name )
{
	unsigned int hash = ial_symbol_table_hash_func(name);

	token *item = self->row[hash];

	while (item != NULL)
	{
		if (!strcmp(item->name, name))
		{
			return item;
		}

		item = item->next;
	}

	return NULL;
}

/**
 * sdbm hashing algorithm
 * @param item token
 * @returns hash table row
 */
unsigned int ial_symbol_table_hash_func ( const char *hashname )
{
	if (!hashname)
	{
		return -1;
	}

	unsigned int hash = 0;
	unsigned char *begin = (unsigned char*)hashname;
	int current = 0;

	while( (current = *begin) )
	{
		begin++;
		hash = current + (hash << 6) + (hash << 16) - hash;
	}

	return hash % TABLE_ROWS;
}

/**
 * Count items in symbol table
 * @param self symbol_table
 * @returns number of items
*/
int ial_symbol_table_count_items( symbol_table *self)
{

	int counter = 0;

	for (unsigned int i = 0; i < self->size; ++i)
	{

		if (self->row[i] == NULL)
		{
			continue;
		}

		token *item = self->row[i];

		while (item != NULL)
		{
			counter++;
			item = item->next;
		}
	}

	return counter;
}

/**
 * Free all tokens and symbol table
 * @param self symbol_table
 * @returns 0 if successful
 */
int ial_symbol_table_drop ( symbol_table *self)
{
	if (!self)
		return 1;

	for (unsigned int i = 0; i < self->size; ++i)
	{
		if (self->row[i] == NULL)
		{
			continue;
		}

		token *item = self->row[i];

		while (item != NULL)
		{
			token *itemNext = item->next;

			if (item->value != NULL)
			{
				free((void *) item->value);
			}

			free(item);

			item = itemNext;
		}

		self->row[i] = NULL;
	}

	return 0;
}

/**
 * Count items in symbol table
 * @param self symbol_table
 * @returns number of items
*/
int ial_symbol_table_count_items( symbol_table *self)
{

	int counter = 0;

	for (unsigned int i = 0; i < self->size; ++i)
	{

		if (self->row[i] == NULL)
		{
			continue;
		}

		token *item = self->row[i];

		while (item != NULL)
		{
			counter++;
			item = item->next;
		}
	}

	return counter;
}

/**
 * Free all tokens and symbol table
 * @param self symbol_table
 * @returns 0 if successful
 */
int ial_symbol_table_drop ( symbol_table *self)
{

	for (unsigned int i = 0; i < self->size; ++i)
	{
		if (self->row[i] == NULL)
		{
			continue;
		}

		token *item = self->row[i];

		while (item != NULL)
		{
			token *itemNext = item->next;

			if (item->name != NULL)
			{
				free((char *) item->name);
			}

			if (item->value != NULL)
			{
				free((void *) item->value);
			}

			free(item);

			item = itemNext;
		}

		self->row[i] = NULL;
	}

	return 0;
}

/**
 * Symbol table constructor
 * @return new uninitialised symbol_table
 */
static symbol_table* ial_symbol_table_construct()
{
    symbol_table *table = (symbol_table*) malloc ( sizeof( struct _symbol_table ));

    if(!table)
    {
        fprintf(stderr,"ERROR: allocating symbol table structure!\n");
        exit(99);
    }

	table->size = TABLE_ROWS;
	table->row = (token**) malloc ( sizeof( token*) * table->size );

	if(!table->row)
    {
        fprintf(stderr,"ERROR: allocating symbol table!\n");
        exit(99);
    }

    return table;
}

/**
 * Symbol table initializer
 * @param self symbol_table
 */
static void ial_symbol_table_init( symbol_table *self )
{
	self->add_item	= &ial_symbol_table_add_item;
	self->get_item = &ial_symbol_table_get_item;
	self->count_items = &ial_symbol_table_count_items;
	self->drop = &ial_symbol_table_drop;

	for( int i = 0; i < self->size; i++)
	{
		self->row[i] = NULL;
	}
}

/**
 * Create new symbol table structure
 *  - contains default methods + hash structure
 * @return  new symbol table
 */
symbol_table *ial_symbol_table_new()
{
    symbol_table *table = ial_symbol_table_construct();
    ial_symbol_table_init(table);

    return table;
}

/**
 * Find first occurrence of string search in string s1
 * - use Knuth-Morris-Prattuv algorithm
 * @param input string
 * @param substring to be searched
 * @returns -1 if search string is not found, index if successfully found
*/
int ifj_find ( const char *s1, const char *search)
{
	return 0;
}

/**
 * Sort ordinary value of chars in inputString
 * - use List-Merge sort
 * @param input string
 * @returns sorted string
*/
const char * ifj_sort ( const char *inputString )
{
	return NULL;
}
