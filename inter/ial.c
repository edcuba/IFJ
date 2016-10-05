/* ial.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#include "ial.h"
#include <stdlib.h>
#include <stdio.h>

#define TABLE_ROWS 3333 //some better hash func maybe..


/**
 * Add new item
 * - store pointer to token in hash table
 * - allows to access pointer later
 */
const token *ial_symbol_table_add_item	( token *item )
{

}

unsigned int ial_symbol_table_hash_func ( token *item )
{
	if (!item->name)
		return -1;
		
	unsigned int h=0;
	unsigned char *p;
	
	for( p = (unsigned char*)item->name; *p!='\0'; p++)
		h = 65599*h + *p;
	return h % TABLE_ROWS;
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
	self->hash_func	= &ial_symbol_table_hash_func;
	self->add_item	= &ial_symbol_table_add_item;
}

/**
 * Create new symbol table structure
 *  - contains default methods + hash structure
 * @return  new symbol table
 */
 symbol_table *ial_symbol_table_new()
 {
     symbol_table *table = _ial_symbol_table_construct();
     ial_symbol_table_init(table);
     return table;
 }
