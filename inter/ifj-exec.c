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
		if (self->debugMode)
		{
			fprintf(stderr, "%s %d\n", "Actual instruction: ", instruc->type);
		}

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
					double result = *((double *) instruc->op1->data) * *((double *) instruc->op2->data);
					tempToken = ifj_generate_temp(
						T_DOUBLE,
						&result
						);
				}
				else
				{
					int result = *((int *) instruc->op1->data) * *((int *) instruc->op2->data);
					tempToken = ifj_generate_temp(
						T_INTEGER,
						&result
						);
				}

				// Push temp token, and free temp tokens
				ifj_stack_push(stack, tempToken);
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
					ifj_stack_push(stack, tempToken);
				}
				else
				{
					if (instruc->op1->dataType == T_DOUBLE || instruc->op2->dataType == T_DOUBLE)
					{
						double result = *((double *) instruc->op1->data) + *((double *) instruc->op2->data);
						tempToken = ifj_generate_temp(
							T_DOUBLE,
							&result
							);
					}
					else
					{
						int result = *((int *) instruc->op1->data) + *((int *) instruc->op2->data);
						tempToken = ifj_generate_temp(
							T_INTEGER,
							&result
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
					double result = *((double *) instruc->op1->data) - *((double *) instruc->op2->data);
					tempToken = ifj_generate_temp(
						T_DOUBLE,
						&result
						);
				}
				else
				{
					int result = *((int *) instruc->op1->data) - *((int *) instruc->op2->data);
					tempToken = ifj_generate_temp(
						T_INTEGER,
						&result
						);
				}

				// Push temp token, and free temp tokens
				ifj_stack_push(stack, tempToken);
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
					double result = *((double *) instruc->op1->data) / *((double *) instruc->op2->data);
					tempToken = ifj_generate_temp(
						T_DOUBLE,
						&result
						);
				}
				else
				{
					int result = *((int *) instruc->op1->data) / *((int *) instruc->op2->data);
					tempToken = ifj_generate_temp(
						T_INTEGER,
						&result
						);
				}

				// Push temp token, and free temp tokens
				ifj_stack_push(stack, tempToken);
				freeTempTokens(instruc);
				break;
			}

			case I_PUSH:
			{
				ifj_stack_push(stack, instruc->op1);
				break;
			}

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

					instruc->op3->data = strdup(instruc->op1->data);
				}
				else
				{
					if (instruc->op3->dataType == T_DOUBLE)
					{
						// double a = 3.14(double) , double a = 1(int)
						*((double *) instruc->op3->data) = *((double *) instruc->op1->data);
					}
					
					if (instruc->op3->dataType == T_INTEGER)
					{
						// int a = 2(int) , int a = 3.14(double)
						*((int *) instruc->op3->data) = *((int *) instruc->op1->data);
					}
				}

				// Free temp token
				freeTempTokens(instruc);
				break;
			}

			case I_CALL:
			{
				// Daco urobit s argumentmy na stacku, _dakam_ = ifj_stack_pop(stack);
				//for (int i = 0; i < (op1->args->top + 1); )

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

				// Free temp token
				if (output->type == T_TMP)
					ifj_token_free(output);

				break;
			}

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

				// Free temp token
				if (label->type == T_TMP)
					ifj_token_free(label);

				break;
			}

			case I_CONDITION:
			{
				// Mozem mat op1 / op2 na stacku

				int output = checkCondition(instruc->op1, instruc->op2, instruc->op3);
				if (output == -1)
				{
					fprintf(stderr, "%s\n", "Executor ERROR: condition error");
					return;
				}

				output = !output;

				token *tempToken = ifj_generate_temp(
					T_INTEGER,
					&output
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
	if (a->dataType == T_DOUBLE || b->dataType == T_DOUBLE)
	{
		switch(rel->type)
		{
			case T_LESS:
				return *((double *) a->data) < *((double *) b->data);
				break;
			case T_GREATER:
				return *((double *) a->data) > *((double *) b->data);
				break;

			case T_LESS_EQUAL:
				return *((double *) a->data) <= *((double *) b->data);
				break;
			case T_GREATER_EQUAL:
				return *((double *) a->data) >= *((double *) b->data);
				break;

			case T_EQUAL:
				return *((double *) a->data) == *((double *) b->data);
				break;
			case T_NOT_EQUAL:
				return *((double *) a->data) != *((double *) b->data);
				break;

			default:
				return -1;
				break;
		}
	}
	else
	{
		switch(rel->type)
		{
			case T_LESS:
				return *((int *) a->data) < *((int *) b->data);
				break;
			case T_GREATER:
				return *((int *) a->data) > *((int *) b->data);
				break;

			case T_LESS_EQUAL:
				return *((int *) a->data) <= *((int *) b->data);
				break;
			case T_GREATER_EQUAL:
				return *((int *) a->data) >= *((int *) b->data);
				break;

			case T_EQUAL:
				return *((int *) a->data) == *((int *) b->data);
				break;
			case T_NOT_EQUAL:
				return *((int *) a->data) != *((int *) b->data);
				break;

			default:
				return -1;
				break;
		}
	}

	return -1;
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
