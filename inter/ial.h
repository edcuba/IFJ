/* ial.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#ifndef IAL_H
#define IAL_H

#include "ifj-inter.h"

struct _symbol_table
{
	unsigned int ( *hash_func	)();
	const token *( *add_item	)( token* );
	const token *( *get_item	)( const char*);
	//prev/next???
};

symbol_table *ial_symbol_table_new();

#endif
