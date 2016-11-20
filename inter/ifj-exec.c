/* ifj-exec.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Robert Kolcun <xkolcu00@stud.fit.vutbr.cz>
 */

#include "ifj-exec.h"
#include "ifj-inter.h"
#include "ifj-util.h"
#include "ifj-lexa.h"
#include "ifj-token.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void run_exec ( ifjInter *self )
{
	instruction *instruc = self->code->first;
	token_stack *stack = ifj_stack_new();

	bool jumped = false;

	// ZAPNUT DEBUG, vypisat aku intrukciu robim
	while (instruc != NULL)
	{
		switch (instruc->type)
		{
			case I_MUL:
			{
				// Get tokens from stack
				if (instruc->op1 == NULL)
					instruc->op1 = ifj_stack_pop(stack);
				if (instruc->op2 == NULL)
					instruc->op2 = ifj_stack_pop(stack);

				// Check if variables are inicialized
				if (instruc->op1->data == NULL || instruc->op2->data == NULL)
				{
					fprintf(stderr, "%s\n", "Variable is not inicialized");
					return;
				}

				// Calculate (a * b)
				token *tempToken = NULL;
				if (instruc->op1->dataType == T_DOUBLE || instruc->op2->dataType == T_DOUBLE)
				{
					tempToken = ifj_generate_temp(
						T_DOUBLE,
						*((double *) instruc->op1->data) * *((double *) instruc->op2->data)
						);
				}
				else
				{
					tempToken = ifj_generate_temp(
						T_INTEGER,
						*((int *) instruc->op1->data) * *((int *) instruc->op2->data)
						);
				}

				// Push temp token, and free temp tokens
				ifj_stack_push(tempToken);
				freeTempTokens(instruc);
				break;
			}

			case I_ADD:
			{
				// Get tokens from stack
				if (instruc->op1 == NULL)
					instruc->op1 = ifj_stack_pop(stack);
				if (instruc->op2 == NULL)
					instruc->op2 = ifj_stack_pop(stack);

				// Check if variables are inicialized
				if (instruc->op1->data == NULL || instruc->op2->data == NULL)
				{
					fprintf(stderr, "%s\n", "Variable is not inicialized");
					return;
				}
				
				// Calculate (a + b)
				token *tempToken = NULL;
				if (instruc->op1->dataType == T_STRING)
				{
					char *completString = ifj_join_strings(
						instruc->op1->data,
						instruc->op2->data
						);

					tempToken = ifj_generate_temp(
						T_STRING,
						(char *) completString
						);

					// Push temp token
					ifj_stack_push(tempToken);
				}
				else
				{
					if (instruc->op1->dataType == T_DOUBLE || instruc->op2->dataType == T_DOUBLE)
					{
						tempToken = ifj_generate_temp(
							T_DOUBLE,
							*((double *) instruc->op1->data) + *((double *) instruc->op2->data)
							);
					}
					else
					{
						tempToken = ifj_generate_temp(
							T_INTEGER,
							*((int *) instruc->op1->data) + *((int *) instruc->op2->data)
							);
					}

					// Push temp token
					ifj_stack_push(stack, tempToken);
				}

				// Free temp tokens
				freeTempTokens(instruc);
				break;
			}

			case I_SUB:
			{
				// Get tokens from stack
				if (instruc->op1 == NULL)
					instruc->op1 = ifj_stack_pop(stack);
				if (instruc->op2 == NULL)
					instruc->op2 = ifj_stack_pop(stack);

				// Check if variables are inicialized
				if (instruc->op1->data == NULL || instruc->op2->data == NULL)
				{
					fprintf(stderr, "%s\n", "Variable is not inicialized");
					return;
				}

				// Calculate (a - b)
				token *tempToken = NULL;
				if (instruc->op1->dataType == T_DOUBLE || instruc->op2->dataType == T_DOUBLE)
				{
					tempToken = ifj_generate_temp(
						T_DOUBLE,
						*((double *) instruc->op1->data) - *((double *) instruc->op2->data)
						);
				}
				else
				{
					tempToken = ifj_generate_temp(
						T_INTEGER,
						*((int *) instruc->op1->data) - *((int *) instruc->op2->data)
						);
				}

				// Push temp token, and free temp tokens
				ifj_stack_push(tempToken);
				freeTempTokens(instruc);
				break;
			}

			case I_DIV:
			{
				// Get tokens from stack
				if (instruc->op1 == NULL)
					instruc->op1 = ifj_stack_pop(stack);
				if (instruc->op2 == NULL)
					instruc->op2 = ifj_stack_pop(stack);

				// Check if variables are inicialized
				if (instruc->op1->data == NULL || instruc->op2->data == NULL)
				{
					fprintf(stderr, "%s\n", "Variable is not inicialized");
					return;
				}

				// Check division by zero
				if ( *((int *) instruc->op2->data) == 0 )
					fprintf(stderr, "%s\n", "Division by zero");
					return;

				// Calculate (a / b)
				token *tempToken = NULL;
				if (instruc->op1->dataType == T_DOUBLE || instruc->op2->dataType == T_DOUBLE)
				{
					tempToken = ifj_generate_temp(
						T_DOUBLE,
						*((double *) instruc->op1->data) / *((double *) instruc->op2->data)
						);
				}
				else
				{
					tempToken = ifj_generate_temp(
						T_INTEGER,
						*((int *) instruc->op1->data) / *((int *) instruc->op2->data)
						);
				}

				// Push temp token, and free temp tokens
				ifj_stack_push(tempToken);
				freeTempTokens(instruc);
				break;
			}

			case I_PUSH:
			{
				ifj_stack_push(stack, instruc->op1);
				break;
			}

			//c = a, a je neinializovane
			case I_SET:
			{
				// Get token from stack
				if (instruc->op1 == NULL)
					instruc->op1 = ifj_stack_pop(stack);

				// Check if variable is inicialized
				if (instruc->op1->data == NULL)
				{
					fprintf(stderr, "%s\n", "Variable is not inicialized");
					return;
				}

				// Set value
				if (instruc->op1->dataType == T_STRING)
				{
					if ((char *) instruc->op3->data)
						free((char *) instruc->op3->data);

					//EDO
					instruc->op3->data = strdup(instruc->op1->data);
				}
				else
				{
					//EDO
					instruc->op3->data = instruc->op1->data;
				}

				break;
			}

			case I_CALL:
			{
				// Daco urobit s argumentmy na stacku, _dakam_ = ifj_stack_pop(stack);

				ifj_stack_push(stack, instruc->op1);

				instruc = instruc->op1->jump;
				jumped = true;
				break;
			}

			case I_GOTO:
			{
				instruc = instruc->op3->jump;
				jumped = true;
				break;
			}

			case I_GOTO_CONDITION:
			{
				token *output = ifj_stack_pop(stack);

				if ( *((int *)output->data) )
				{
					instruc = instruc->op3->jump;
					jumped = true;
				}
				break;
			}

			// Tak isto op1
			case I_RETURN:
			{
				token *label = ifj_stack_pop(stack);

				if (instruc->op1 != NULL)
				{
					// Check if variable is inicialized
					if (instruc->op1->data == NULL)
					{
						fprintf(stderr, "%s\n", "Variable is not inicialized");
						return;
					}
					
					ifj_stack_push(stack, instruc->op1);
				}
				
				instruc = label->jump;
				jumped = true;

				break;
			}

			case I_CONDITION:
			{
				int output = checkCondition(instruc->op1, instruc->op2, instruc->op3);
				output = !output;

				token *tempToken = ifj_generate_temp(
					T_INTEGER,
					output
					);

				ifj_stack_push(stack, tempToken);
				break;
			}

			case I_LABEL:
			{
				break;
			}

			default:
			{
				fprintf(stderr, "%s %d\n", "Undefined instruction, value: ", instruc->type);
				break;
			}
		}

		if (!jumped)
			instruc = instruc->next;
		else
			jumped = false;
	}

	ifj_stack_drop(stack);
}

int checkCondition (	token *a,
						token *b,
						token *rel )
{
	switch(rel->type)
	{
		//EDO
		case T_LESS:
			return a->data < b->data;
			break;
		case T_GREATER:
			return a->data > b->data;
			break;

		case T_LESS_EQUAL:
			return a->data <= b->data;
			break;
		case T_GREATER_EQUAL:
			return a->data >= b->data;
			break;

		case T_EQUAL:
			return a->data == b->data;
			break;
		case T_NOT_EQUAL:
			return a->data != b->data;
			break;
	}
}

void freeTempTokens (instruction *inputInstruc)
{
	if (inputInstruc->op1 && inputInstruc->op1->type == T_TMP)
	{
		ifj_token_free(inputInstruc->op1);
		inputInstruc->op1 = NULL;
	}

	if (inputInstruc->op2 && inputInstruc->op2->type == T_TMP)
	{
		ifj_token_free(inputInstruc->op2);
		inputInstruc->op2 = NULL;
	}
}
