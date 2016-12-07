/* ifj-util.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Robert Kolcun <xkolcu00@stud.fit.vutbr.cz>
 *          Ondrej Kurak < DOPISAT >
 */

#include "ifj_util.h"
#include "ifj_inter.h"
#include "ifj_token.h"
#include "ifj_lexa.h"
#include "ifj_exec.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>

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

	//newStack->elements = malloc(32 * sizeof(token *));
	newStack->elements = calloc(32, sizeof(token *));
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

	    if (inStack->elements == NULL)
	    {
	    	// TODO chyba pamate
	    	return;
	    }
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
	while (!ifj_stack_empty(inStack))
	{
		token *checkToken = ifj_stack_pop(inStack);
		if (checkToken && checkToken->type == T_TMP)
		{
			ifj_token_free(checkToken);
		}
	}

	free(inStack->elements);
	free(inStack);
}

/**
 * Clear input stack
*/
void ifj_stack_clear ( token_stack *inStack )
{
	/*free(inStack->elements);
	inStack->elements = malloc(inStack->size * sizeof(token *));

	if (inStack->elements == NULL)
	{
		// TODO chyba pamate
		return;
	}*/

	inStack->top = -1;
}

/**
 * Print type of every token in stack
*/
void ifj_stack_print ( token_stack *inStack )
{
	for (int i = inStack->top; i >= 0; i--)
	{
		if (inStack->elements[i])
			fprintf(stdout, "%d %s - token type: %d %d\n",
					i,
					(char *)inStack->elements[i]->value,
					inStack->elements[i]->type,
					inStack->elements[i]->dataType);
		else
			fprintf(stderr, "%s\n", "NULL");
	}
	fprintf(stdout, "%s\n", "--------------------");
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
	newList->last = NULL;

	return newList;
}

/**
 * Add new item at first place in linear_list
 * @param list linear_list
 * @param instruction stored in variable data in item
 * @return 0, -1 when unsuccessful
*/
int ifj_insert_first (	linear_list *list,
						int inputType,
						token *oper1,
						token *oper2,
						token *oper3 )
{
	instruction *newInstruction = malloc(sizeof(instruction));

	if (newInstruction == NULL)
	{
		return 99;
	}

	newInstruction->next = list->first;
	list->first = newInstruction;

	if (list->last == NULL)
	{
		list->last = newInstruction;
	}

	newInstruction->type = inputType;
	newInstruction->op1 = oper1;
	newInstruction->op2 = oper2;
	newInstruction->op3 = oper3;

	return 0;
}

/**
 * Add new item at last place in linear_list
 * @param list linear_list
 * @param instruction stored in variable data in item
 * @return 1, 0 when unsuccessful
*/
int ifj_insert_last_instruc(linear_list *list,
							instruction *newInstruction)
{
	instruction *temp = list->last;
	list->last = newInstruction;

	if (list->first == NULL)
	{
		list->first = newInstruction;
		temp = NULL;
	}
	else
	{
		temp->next = newInstruction;
	}

	return 1;
}

/**
 * Add new item at last place in linear_list
 * @param list linear_list
 * @param instruction stored in variable data in item
 * @return 1, 0 when unsuccessful
*/
int ifj_insert_last (	linear_list *list,
						int inputType,
						token *oper1,
						token *oper2,
						token *oper3 )
{
	instruction *newInstruction = malloc(sizeof(instruction));

	if (newInstruction == NULL)
	{
		//TODO ROBO global flag
		return 0;
	}

	newInstruction->next = NULL;
	instruction *temp = list->last;
	list->last = newInstruction;

	if (list->first == NULL)
	{
		list->first = newInstruction;
		temp = NULL;
	}
	else
	{
		temp->next = newInstruction;
	}

	newInstruction->type = inputType;
	newInstruction->op1 = oper1;
	newInstruction->op2 = oper2;
	newInstruction->op3 = oper3;

	return 1;
}

/**
 * Free all items in list and set
 * pointer in list to NULL
 * @param list linear_list
 * @return 0
*/
void ifj_drop_list ( linear_list *list )
{
	if (list == NULL)
	{
		return;
	}

	instruction *tempIntruction = list->first;

	list->active = NULL;
	list->first = NULL;
	list->last = NULL;

	while (tempIntruction != NULL)
	{
		instruction *nextInstruction = tempIntruction->next;

		if (tempIntruction->op3 && tempIntruction->op3->type == T_TMP)
		{
			ifj_token_free(tempIntruction->op3);
		}

		if (tempIntruction->type != I_FOR_START &&
			tempIntruction->op1 &&
			tempIntruction->op1->type == T_TMP)
		{
			ifj_token_free(tempIntruction->op1);
		}

		if (tempIntruction->op2 && tempIntruction->op2->type == T_TMP)
		{
			ifj_token_free(tempIntruction->op2);
		}

		tempIntruction->next = NULL;
		free(tempIntruction);

		tempIntruction = nextInstruction;
	}

    free(list);
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
	instruction *tempIntruction = list->first;

	if (tempIntruction == NULL)
	{
		return;
	}

	while (tempIntruction->next != NULL)
	{
		tempIntruction = tempIntruction->next;
	}

	list->active = tempIntruction;
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

void ifj_list_print ( linear_list *list )
{
	instruction *temp = list->first;
	while (temp != NULL)
	{
		fprintf(stderr, "%d ", temp->type);
		printInstruction(temp);
		temp = temp->next;
	}
}

/**
 * Read integer from stdin
 * @return const int token
*/
token *ifj_read_int ()
{
	dyn_buffer *buffer = dyn_buffer_init(64);
	int ch;
	while ((ch = getchar()) != EOF) {
		if (ch == '\n') {
			break;
		}
		if (!isdigit(ch)) {
			dyn_buffer_free(buffer);
			return NULL;
		}
		dyn_buffer_append(buffer, ch);
	}

    if (buffer->top == -1) {
        dyn_buffer_free(buffer);
        return NULL;
    }

	char *endptr = NULL;
	errno = 0;
	long in = strtol(dyn_buffer_get_content(buffer), &endptr, 10);

    char end = *endptr;
    dyn_buffer_free(buffer);

	if (*endptr != '\0') {
		return NULL;
	}

	if (errno != 0 || in > INT_MAX) {
		return NULL;
	}

	return ifj_generate_temp(T_INTEGER, &in);
}

/**
 * Read double from stdin
 * @return floating point number
*/
token *ifj_read_double ()
{
    dyn_buffer *buffer = dyn_buffer_init(64);
    int ch;
    while ((ch = getchar()) != EOF) {
        if (ch == '\n') {
            break;
        }
		if (isdigit(ch) || ch == '.' || ch == 'e' || ch == 'E') {
			dyn_buffer_append(buffer, ch);
		} else {
            dyn_buffer_free(buffer);
			return NULL;
		}

    }

    if (buffer->top == -1) {
        dyn_buffer_free(buffer);
        return NULL;
    }

    char *endptr = NULL;
	errno = 0;
    double in = strtod(dyn_buffer_get_content(buffer), &endptr);

    char end = *endptr;
    dyn_buffer_free(buffer);

    if (end != '\0') {
        return NULL;
    }

    if (errno != 0) {
        return NULL;
    }

    return ifj_generate_temp(T_DOUBLE, &in);
}

/**
 * Read string from stdin
 * @return string
*/
token *ifj_read_string ()
{
    dyn_buffer *buffer = dyn_buffer_init(128);
    int ch;
    while ((ch = getchar()) != EOF) {
        if (ch == '\n') {
            break;
        }
        dyn_buffer_append(buffer, ch);
    }
    token *t = ifj_generate_temp(T_STRING, strdup(dyn_buffer_get_content(buffer)));
    dyn_buffer_free(buffer);
	return t;
}

/**
 * Print input at stdout
 * - int convert to string
 * - double convert to string and print by '%g'
 * @param token stack
 * @param number of items to pop from stack
 * @return void
*/
int ifj_print ( token_stack *inStack, int popNum )
{
	token *item;

	for ( int i = popNum - 1 ; i >= 0; --i)
	{
		if (inStack->elements[inStack->top - i]->data == NULL)
		{
			return 0;
		}
	}

	for ( int i = popNum - 1 ; i >= 0; --i)
	{
		item = inStack->elements[inStack->top - i];
		switch ( item->dataType )
		{
	        case T_INTEGER:
	            printf("%d", *((int *)item->data));
	            break;
	        case T_DOUBLE:
	            printf("%g", *((double *)item->data));
	            break;
	        case T_STRING:
	            printf("%s", (char *)item->data);
	            break;
		}
	}

	for ( int i = 0; i < popNum; ++i)
	{
		item = ifj_stack_pop(inStack);
	}

	return 1;
}

/**
 * Calculate length of inputString
 * @param inputString
 * @return number length of inputString
*/
token *ifj_length ( const char *inputString )
{
	int result = (int) strlen(inputString);
	token *temp = ifj_generate_temp(T_INTEGER, &result);
	return temp;
}

/**
 * Return substring of the inputString
 * - simulate substring function from Java
 * @param length substring length
 * @param index beginning of substring
 * @param string inputString
 * @return
*/
token * ifj_substr (int n,
					int i,
				 	const char *inputString)
{
	if (n < 0 || i < 0 || i >= strlen(inputString))
	{
		return NULL;
	}

	if ((n + i) > strlen(inputString))
	{
		return NULL;
	}

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

	token *temp = ifj_generate_temp(T_STRING, (char *) outputString);
	return temp;
}

static int ifj_compare_help (	const char *s1,
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
				return (int) strlen(s1) - (int) strlen(s2) > 0 ? 1 : -1;
			}
			return s1[i] - s2[i] > 0 ? 1 : -1;
		}
	}

	return 0;
}

/**
 * Compare two input strings
 * - simulate function compareTo from Java
 * @param first input string
 * @param second input string
 * @return 0 - s1 and s2 are same, 1 - s1 is greater than s2, -1 - another situation
*/
token *ifj_compare (const char *s1,
					const char *s2 )
{
	int output = ifj_compare_help(s2, s1);
	token *temp = ifj_generate_temp(T_INTEGER, &output);
	return temp;
}

/**
 * Concatenate str1 and str2 into res
 * @param str1 First string
 * @param str2 Second string
 * @return concatenation of str1 and str2
 */
char *ifj_join_strings(const char *str1, const char *str2)
{
	size_t len1 = strlen(str1) + 1;
	size_t len2 = strlen(str2);
	char *res = malloc((len1 + len2) * sizeof(char));

	if (res == NULL)
		return NULL;

	memcpy(res, str1, len1);
	return strncat(res, str2, len2);
}
