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

#define TABLE_ROWS 997

/**
 * Add new item
 * - store pointer to token in hash table
 * @param self symbol table
 * @param item token
 * @return stored token pointer or NULL when unsuccessful
 */
token *ial_symbol_table_add_item	( 	symbolTable *self,
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
 * @return token pointer or NULL when unsuccessful
 */
token *ial_symbol_table_get_item	( 	symbolTable *self,
	 									const char *hashname,
										int type,
										char *(*generate_hashname)(void*))
{
	unsigned int hash = ial_symbol_table_hash_func(hashname);

	token *item = self->row[hash];

	while (item != NULL)
	{
		if(type && item->type != type)
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
		else if (type && !item->value) //type with no value - symbol
		{
			return item;
		}
		else if (strlen((char *) item->value) == strlen (hashname) && !strcmp(item->value, hashname))
		{
			return item;;
		}

		item = item->next;
	}

	return NULL;
}

/**
 * sdbm hashing algorithm
 * @param item token
 * @return hash table row
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
 * @param self symbolTable
 * @return number of items
*/
int ial_symbol_table_count_items( symbolTable *self)
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
 * @param self symbolTable
 * @return 0 if successful
 */
int ial_symbol_table_drop ( symbolTable *self)
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

			ifj_token_free(item);

			item = itemNext;
		}

		self->row[i] = NULL;
	}

	free(self->row);
	free(self);
	self = NULL;

	return 0;
}

/**
 * Create new symbol table structure
 *  - contains initialized default methods and hash structure
 * @return new symbol table
 */
symbolTable *ial_symbol_table_new()
{
	symbolTable *table = calloc(1, sizeof(symbolTable));

	if(!table)
	{
		fprintf(stderr,"ERROR: allocating symbol table structure!\n");
		exit(99);
	}

	table->size = TABLE_ROWS;
	table->row = (token**) malloc ( table->size * sizeof( token*) );

	if(!table->row)
	{
		fprintf(stderr,"ERROR: allocating symbol table!\n");
		exit(99);
	}

	table->add_item	= &ial_symbol_table_add_item;
	table->get_item = &ial_symbol_table_get_item;
	table->count_items = &ial_symbol_table_count_items;
	table->drop = &ial_symbol_table_drop;
	table->parent = NULL;

	for( int i = 0; i < table->size; i++)
	{
		table->row[i] = NULL;
	}

    return table;
}

/**
 * Find first occurrence of string search in string s1
 * - use Knuth-Morris-Prattuv algorithm
 * @param input string
 * @param substring to be searched
 * @return -1 if search string is not found, index if successfully found
*/
int ifj_find ( const char *s1, const char *search)
{

	if ( !s1 || !search )
	{
		return -1;
	}

	int inputLength = strlen(s1);
	int searchLength = strlen(search);

	if ( searchLength > inputLength ) {
		return -1;
	}

	int kmpArray[ searchLength ];

	int i = 0;
	int j = kmpArray[0] = -1;
	while ( i < searchLength )
	{
		while ( j > -1 && search[i] != search[j] )
		{
			j = kmpArray[j];
		}

		i++;
		j++;

		if ( search[i] == search[j] )
		{
			kmpArray[i] = kmpArray[j];
		}
		else
		{
			kmpArray[i] = j;
		}
	}

	i = 0;
	j = 0;

	while ( i < inputLength )
	{
		if ( search[i] == s1[j] )
		{
			if ( i == searchLength - 1 )
			{
				return( j - searchLength + 1 );
			}
			else
			{
				i++;
				j++;
			}
		}
		else if ( kmpArray[i] == -1 )
		{
			i = 0;
			j++;
		}
		else
		{
			i = kmpArray[i];
		}
	}
	return -1;
}

/**
 * Function sort input strings
 * @param first input string
 * @param second input string
 * @return sorted string or NULL if error
*/
static char * ifj_sort_help( char *l1, char *l2)
{
	char *output = malloc(sizeof(char) * (strlen(l1) + strlen(l2) + 1));

	if (output == NULL)
	{
		return NULL;
	}

	unsigned int outLength = (strlen(l1) + strlen(l2)) - 1;
	unsigned int counter = 0;

	while (strlen(l1) != 0 && strlen(l2) != 0)
	{
		unsigned int l1Length = strlen(l1) - 1;
		unsigned int l2Length = strlen(l2) - 1;

		if (l1[l1Length] > l2[l2Length])
		{
			output[outLength - counter] = l1[l1Length];
			l1[l1Length] = '\0';
			counter++;
		}
		else
		{
			output[outLength - counter] = l2[l2Length];
			l2[l2Length] = '\0';
			counter++;
		}
	}

	while (strlen(l1) != 0)
	{
		output[outLength - counter] = l1[strlen(l1) - 1];
		l1[strlen(l1) - 1] = '\0';
		counter++;
	}

	while (strlen(l2) != 0)
	{
		output[outLength - counter] = l2[strlen(l2) - 1];
		l2[strlen(l2) - 1] = '\0';
		counter++;
	}

	output[outLength + 1] = '\0';

	free(l1);
	free(l2);

	return output;
}

/**
 * Function divide string into elements
 * @param input string
 * @return sorted string or NULL if error
*/
static char * ifj_sort_divide( const char *s1)
{
	if (strlen(s1) == 1)
	{
		return (char *) s1;
	}

	unsigned int halfLength = strlen(s1) / 2;

	char *l1 = malloc(sizeof(char) * (halfLength + 1));
	char *l2 = strlen(s1) % 2 == 1 ? malloc(sizeof(char) * (halfLength + 2)) : malloc(sizeof(char) * (halfLength + 1));

	if (l1 == NULL || l2 == NULL)
	{
		return NULL;
	}

	for (unsigned int i = 0; i < halfLength; ++i)
	{
		l1[i] = s1[i];
		l2[i] = s1[i + halfLength];
	}

	l1[halfLength] = '\0';
	l2[halfLength] = '\0';

	if (strlen(s1) % 2 == 1)
	{
		l2[halfLength] = s1[halfLength * 2];
		l2[halfLength + 1] = '\0';
	}

	char *l1p = ifj_sort_divide(l1);
	if (l1p != l1)
	{
		free(l1);
	}

	char *l2p = ifj_sort_divide(l2);
	if (l2p != l2)
	{
		free(l2);
	}

	return ifj_sort_help(l1p, l2p);
}

/**
 * Sort ordinary value of chars in inputString
 * - use List-Merge sort
 * @param input string
 * @return sorted string or NULL if error
*/
char * ifj_sort( const char *s1 )
{
	if (strlen(s1) == 1)
	{
		return strdup(s1);
	}
	else
	{
		return ifj_sort_divide(s1);
	}
}
