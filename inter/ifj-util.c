/* ifj-util.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Robert Kolcun <xkolcu00@stud.fit.vutbr.cz>
 *          Jan Demcak < DOPISAT >
 *          Ondrej Kurak < DOPISAT >
 */

#include "ifj-util.h"
#include <stdlib.h>
#include <string.h>


/**
 * Initialize and allocate new token_stack
 * @return allocated token_stack
*/
token_stack *ifj_stack_new ()
{
	token_stack *newStack = malloc(sizeof(token_stack));
	if (newStack == NULL)
	{
		return NULL;
	}

	newStack->elements = malloc(32 * sizeof(token *));
	if (newStack->elements == NULL)
	{
		free(newStack);
		return NULL;
	}

	newStack->top = -1;
	newStack->size = 32;

	return newStack;
}

/**
 * Add input token to the stack
 * Memory reallocated if stack is full
 * @param stack token_stack
 * @param token this token is added to the stack
*/
void ifj_stack_push (	token_stack *inStack,
						token *inToken )
{
	if (ifj_stack_full(inStack))
	{
		inStack->size += 32;
	    inStack->elements = realloc(inStack->elements, inStack->size * sizeof(token *));
	}

	inStack->top++;
	inStack->elements[inStack->top] = inToken;
}

/**
 * Pop token from top of stack
 * @param stack token_stack
 * @return token top token of the stack
*/
token *ifj_stack_pop ( token_stack *inStack )
{
	if (ifj_stack_empty(inStack))
	{
		return NULL;
	}

	return inStack->elements[inStack->top--];
}

/**
 * Get token from top of stack
 * token is not poped
 * @param stack token_stack
 * @return token top token of the stack
*/
token *ifj_stack_top ( token_stack *inStack )
{
	if (ifj_stack_empty(inStack))
	{
		return NULL;
	}

	return inStack->elements[inStack->top];
}

/**
 * Get if stack is full
 * @return true/false
*/
bool ifj_stack_full ( token_stack *inStack )
{
	return inStack->top == (inStack->size - 1) ? true : false;
}

/**
 * Get if stack is empty
 * @return true/false
*/
bool ifj_stack_empty ( token_stack *inStack )
{
	return inStack->top == -1 ? true : false;
}

/**
 * Free input stack
*/
void ifj_stack_drop ( token_stack *inStack )
{
	free(inStack->elements);
	free(inStack);
}

/**
 * Initialize and allocate new linear_list
 * @return allocated linear_list
*/
linear_list *ifj_list_new ()
{
	linear_list *newList = malloc(sizeof(linear_list));

	if (newList == NULL)
	{
		return NULL;
	}

	newList->active = NULL;
	newList->first = NULL;

	return newList;
}

/**
 * Add new item at first place in linear_list
 * @param list linear_list
 * @param instruction stored in variable data in item
 * @return 0, -1 when unsuccessful
*/
int ifj_insert_first (	linear_list *list,
						instruction *item )
{
	linear_item *newItem = malloc(sizeof(linear_item));

	if (newItem == NULL)
	{
		return -1;
	}

	newItem->next = list->first;
	list->first = newItem;

	newItem->data = item;

	return 0;
}

/**
 * Add new item at last place in linear_list
 * @param list linear_list
 * @param instruction stored in variable data in item
 * @return 0, -1 when unsuccessful
*/
int ifj_insert_last (	linear_list *list,
						instruction *item )
{
	linear_item *newItem = malloc(sizeof(linear_item));

	if (newItem == NULL)
	{
		return -1;
	}

	linear_item *tempItem = list->first;

	if (tempItem == NULL)
	{
		free(newItem);
		return ifj_insert_first(list, item);
	}

	while (tempItem->next != NULL)
	{
		tempItem = tempItem->next;
	}

	tempItem->next = newItem;
	newItem->next = NULL;
	newItem->data = item;

	return 0;
}

/**
 * Free all items in list and set
 * pointer in list to NULL
 * @param list linear_list
 * @return 0
*/
int ifj_drop_list ( linear_list *list )
{
	if (list == NULL)
	{
		return 0;
	}

	linear_item *tempItem = list->first;

	list->active = NULL;
	list->first = NULL;

	while (tempItem != NULL)
	{
		linear_item *nextItem = tempItem->next;

		tempItem->next = NULL;
		free(tempItem);

		tempItem = nextItem;
	}

    free(list);

	return 0;
}

/**
 * Set first item as active item
 * @param list linear_list
*/
void ifj_set_active_first ( linear_list *list )
{
	list->active = list->first;
}

/**
 * Set last item as active item
 * @param list linear_list
*/
void ifj_set_active_last ( linear_list *list )
{
	linear_item *tempItem = list->first;

	if (tempItem == NULL)
	{
		return;
	}

	while (tempItem->next != NULL)
	{
		tempItem = tempItem->next;
	}

	list->active = tempItem;
}

/**
 * Set active item, item after currently active item
 * @param list linear_list
*/
void ifj_set_active_next ( linear_list *list )
{
	if (list->active == NULL)
	{
		return;
	}

	list->active = list->active->next;
}

/**
 * Read integer from stdin
 * @return integer number
*/
int ifj_read_int ()
{
	return 0;
}

/**
 * Read double from stdin
 * @return floating point number
*/
double ifj_read_double ()
{
	return 0;
}

/**
 * Read string from stdin
 * @return string
*/
const char * ifj_read_string ()
{
	return NULL;
}

/**
 * Print input at stdout
 * - int convert to string
 * - double convert to string and print by '%g'
 * @param input string
 * @return void
*/
void ifj_print ( const char *input )
{
	return;
}

/**
 * Calculate length of inputString
 * @param inputString
 * @return number length of inputString
*/
int ifj_length ( const char *inputString )
{
	return strlen(inputString);
}

/**
 * Return substring of the inputString
 * - simulate substring function from Java
 * @param string inputString
 * @param index beginning of substring
 * @param length substring length
 * @return
*/
char * ifj_substr (	const char *inputString,
					int i,
					int n )
{
	char *outputString = malloc(sizeof(*outputString) * (strlen(inputString) + 1));

	if (outputString == NULL)
	{
		return NULL;
	}

	for (int j = i; j < i + n; ++j)
	{
		if (inputString[j] == '\0')
		{
			break;
		}

		outputString[j - i] = inputString[j];
		outputString[j - i + 1] = '\0';
	}

	return outputString;
}

/**
 * Compare two input strings
 * - simulate function compareTo from Java
 * @param first input string
 * @param second input string
 * @return 0 - s1 and s2 are same, 1 - s1 is greater than s2, -1 - another situation
*/
int ifj_compare (	const char *s1,
					const char *s2 )
{
	for (int i = 0; ; ++i)
	{
		if (s1[i] == s2[i])
		{
			if (s1[i] == '\0' || s2[i] == '\0')
			{
				break;
			}

			continue;
		}
		else
		{
			if (s1[i] == '\0' || s2[i] == '\0')
			{
				return ifj_length(s1) - ifj_length(s2) > 0 ? 1 : -1;
			}

			return s1[i] - s2[i] > 0 ? 1 : -1;
		}
	}

	return 0;
}
