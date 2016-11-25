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
#include "ial.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int exec_run ( ifjInter *self )
{
	if (self->debugMode)
	{
		fprintf(stderr, "%s\n", "---------------- Executor started ----------------");
	}

	instruction *instruc = self->code->first;
	token_stack *stack = ifj_stack_new();

	bool jumped = false;

	while (instruc != NULL)
	{
		if (self->debugMode)
		{
			fprintf(stderr, "%s %d ", "instruction: ", instruc->type);
			printInstruction(instruc->type);

			//ifj_stack_print(stack);

			//ifj_list_print(self->code);
			//return 0;
		}

		switch (instruc->type)
		{
			case I_MUL:
			{
				// Get tokens from stack
				if (instruc->op2 == NULL)
					instruc->op2 = ifj_stack_pop(stack);
				if (instruc->op1 == NULL)
					instruc->op1 = ifj_stack_pop(stack);

				// Check if variables are inicialized
				if (instruc->op1->data == NULL || instruc->op2->data == NULL)
				{
					fprintf(stderr, "%s\n", "Variable is not inicialized");
					self->returnCode = 8;
					return 0;
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
				if (instruc->op2 == NULL)
					instruc->op2 = ifj_stack_pop(stack);
				if (instruc->op1 == NULL)
					instruc->op1 = ifj_stack_pop(stack);

				// Check if variables are inicialized
				if (instruc->op1->data == NULL || instruc->op2->data == NULL)
				{
					fprintf(stderr, "%s\n", "Variable is not inicialized");
					self->returnCode = 8;
					return 0;
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
				if (instruc->op2 == NULL)
					instruc->op2 = ifj_stack_pop(stack);
				if (instruc->op1 == NULL)
					instruc->op1 = ifj_stack_pop(stack);

				// Check if variables are inicialized
				if (instruc->op1->data == NULL || instruc->op2->data == NULL)
				{
					fprintf(stderr, "%s\n", "Variable is not inicialized");
					self->returnCode = 8;
					return 0;
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
				if (instruc->op2 == NULL)
					instruc->op2 = ifj_stack_pop(stack);
				if (instruc->op1 == NULL)
					instruc->op1 = ifj_stack_pop(stack);

				// Check if variables are inicialized
				if (instruc->op1->data == NULL || instruc->op2->data == NULL)
				{
					fprintf(stderr, "%s\n", "Variable is not inicialized");
					self->returnCode = 8;
					return 0;
				}

				// Check division by zero
				if ( *((double *) instruc->op2->data) == (double) 0 )
				{
					fprintf(stderr, "%s\n", "Division by zero");
					self->returnCode = 9;
					return 0;
				}

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
				{
					instruc->op1 = ifj_stack_pop(stack);
				}

				// Check if variable is inicialized
				if (instruc->op1 && instruc->op1->data == NULL)
				{
					fprintf(stderr, "%s\n", "Variable is not inicialized");
					self->returnCode = 8;
					return 0;
				}

				// Function called with return value,
				// but return value is not needed to set
				if (instruc->op3 == NULL)
				{
					ifj_stack_pop(stack);
					break;
				}

				// Set value
				if (instruc->op1->dataType == T_STRING)
				{
					if ((char *) instruc->op3->data)
						free((char *) instruc->op3->data);

					instruc->op3->data = (void *) strdup(instruc->op1->data);
				}
				else
				{
					if (instruc->op3->dataType == T_DOUBLE)
					{
						if(!instruc->op3->data)
						{
							instruc->op3->data = malloc(sizeof(double));
						}
						// double a = 3.14(double) , double a = 1(int)
						*((double *) instruc->op3->data) = *((double *) instruc->op1->data);
					}

					if (instruc->op3->dataType == T_INTEGER)
					{
						if(!instruc->op3->data)
						{
							instruc->op3->data = malloc(sizeof(int));
						}
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
				// Stack of arguments for IFJ16 functions
				token_stack *argsStack = ifj_stack_new();

				if (instruc->op1->args != NULL)
				{
					for (int i = instruc->op1->args->top; i >= 0; i--)
					{
						token *myToken = ifj_stack_pop(stack);

						// Check if variable is inicialized
						if (myToken->data == NULL)
						{
							fprintf(stderr, "%s\n", "Variable is not inicialized");
							self->returnCode = 8;
							return 0;
						}

						ifj_stack_push(argsStack, myToken);
						token *argToken = instruc->op1->args->elements[i];

						// Alloc memory for uninicialized variables
						if (argToken->data == NULL)
						{
							if (argToken->dataType == T_DOUBLE)
							{
								argToken->data = (void*) malloc (sizeof(double));
							}
							if (argToken->dataType == T_INTEGER)
							{
								argToken->data = (void*) malloc (sizeof(int));
							}
						}

						// Set data to argToken
						switch(argToken->dataType)
						{
							case T_STRING:
								if(argToken->data != NULL)
								{
									free(argToken->data);
								}
								argToken->data = (void *) strdup(myToken->data);
								break;

							case T_DOUBLE:
								*((double *) argToken->data) = *((double *) myToken->data);
								break;

							case T_INTEGER:
								*((int *) argToken->data) = *((int *) myToken->data);
								break;

							default:
								fprintf(stderr, "%s %d\n", "Executor ERROR, invalid dataType: ", argToken->dataType);
								self->returnCode = 10;
								return 0;
						}
					}
				}

				token *output = NULL;
				switch(instruc->op1->method)
				{
					case IFJ16_PRINT:
						output = ifj_stack_pop(stack);
						ifj_print(stack, *((int *) output->data));
						ifj_token_free(output);
						output = NULL;
						break;

					case IFJ16_READINT:
						output = ifj_read_int();
						if (!output)
						{
							self->returnCode = 7;
							return 0;
						}
						break;

					case IFJ16_READDOUBLE:
						output = ifj_read_double();
						if (!output)
						{
							self->returnCode = 7;
							return 0;
						}
						break;

					case IFJ16_READSTRING:
						output = ifj_read_string();
						if (!output)
						{
							self->returnCode = 10;
							return 0;
						}
						break;

					case IFJ16_FIND:
						output = ifj_find(
							(char *) ifj_stack_pop(argsStack)->data,
							(char *) ifj_stack_pop(argsStack)->data
							);
						if (!output)
						{
							self->returnCode = 10;
							return 0;
						}
						break;

					case IFJ16_SORT:
						output = ifj_sort(
							(char *) ifj_stack_pop(argsStack)->data
							);
						if (!output)
						{
							self->returnCode = 10;
							return 0;
						}
						break;

					case IFJ16_LENGTH:
						output = ifj_length(
							(char *) ifj_stack_pop(argsStack)->data
							);
						if (!output)
						{
							self->returnCode = 10;
							return 0;
						}
						break;

					case IFJ16_SUBSTR:
						output = ifj_substr(
							(char *) ifj_stack_pop(argsStack)->data,
							*((int *) ifj_stack_pop(argsStack)->data),
							*((int *) ifj_stack_pop(argsStack)->data)
							);
						if (!output)
						{
							self->returnCode = 10;
							return 0;
						}
						break;

					case IFJ16_COMPARE:
						output = ifj_compare(
							(char *) ifj_stack_pop(argsStack)->data,
							(char *) ifj_stack_pop(argsStack)->data
							);
						if (!output)
						{
							self->returnCode = 10;
							return 0;
						}
						break;

					default:
						output = ifj_generate_temp(T_VOID, NULL);
						output->jump = instruc->next;
						instruc = instruc->op1->jump->next;
						jumped = true;
						break;
				}
				if (output != NULL)
					ifj_stack_push(stack, output);

				ifj_stack_drop(argsStack);
				break;
			}

			case I_GOTO:
			{
				instruc = instruc->op3->jump->next;
				jumped = true;
				break;
			}

			case I_GOTO_CONDITION:
			{
				token *output = ifj_stack_pop(stack);

				if ( *((int *)output->data) )
				{
					instruc = instruc->op3->jump->next;
					jumped = true;
				}

				// Free temp token
				if (output->type == T_TMP)
					ifj_token_free(output);

				break;
			}

			case I_RETURN:
			{
				if (instruc->op1 == NULL)
				{
					instruc->op1 = ifj_stack_top(stack);
					if (instruc->op1->dataType != T_VOID)
					{
						ifj_stack_pop(stack);
					}
				}

				token *label = ifj_stack_pop(stack);

				if (instruc->op1 != NULL)
				{
					// Check if variable is inicialized
					if (instruc->op1->data == NULL)
					{
						fprintf(stderr, "%s\n", "Variable is not inicialized");
						self->returnCode = 8;
						return 0;
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
				// Get tokens from stack
				if (instruc->op2 == NULL)
					instruc->op2 = ifj_stack_pop(stack);
				if (instruc->op1 == NULL)
					instruc->op1 = ifj_stack_pop(stack);

				int output = checkCondition(instruc->op1, instruc->op2, instruc->op3);
				if (output == 10)
				{
					fprintf(stderr, "%s\n", "Executor ERROR: condition error");
					self->returnCode = 10;
					return 0;
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
				if (self->debugMode)
				{
					fprintf(stderr, "%s\n", "---------------- Executor ended ----------------");
				}

				ifj_stack_drop(stack);
				self->returnCode = 0;
				return 0;
			}

			default:
			{
				fprintf(stderr, "%s %d\n", "Executor ERROR: Undefined instruction, value: ", instruc->type);
				break;
			}
		}

		if (!jumped)
			instruc = instruc->next;
		else
		{
			jumped = false;
		}
	}

	ifj_stack_drop(stack);

	if (self->debugMode)
	{
		fprintf(stderr, "%s\n", "---------------- Executor ended 2 ----------------");
	}

	self->returnCode = 0;
	return 0;
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
				return 10;
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
				return 10;
				break;
		}
	}

	return 10;
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

void printInstruction(int type)
{
	switch(type)
	{
		case I_MUL:
			fprintf(stderr, "%s\n", "MUL");
			break;
		case I_ADD:
			fprintf(stderr, "%s\n", "ADD");
			break;
		case I_SUB:
			fprintf(stderr, "%s\n", "SUB");
			break;
		case I_DIV:
			fprintf(stderr, "%s\n", "DIV");
			break;
		case I_PUSH:
			fprintf(stderr, "%s\n", "PUSH");
			break;
		case I_SET:
			fprintf(stderr, "%s\n", "SET");
			break;
		case I_CALL:
			fprintf(stderr, "%s\n", "CALL");
			break;
		case I_GOTO:
			fprintf(stderr, "%s\n", "GOTO");
			break;
		case I_GOTO_CONDITION:
			fprintf(stderr, "%s\n", "GOTO_CONDITION");
			break;
		case I_RETURN:
			fprintf(stderr, "%s\n", "RETURN");
			break;
		case I_CONDITION:
			fprintf(stderr, "%s\n", "CONDITION");
			break;
		case I_LABEL:
			fprintf(stderr, "%s\n", "LABEL");
			break;
	}
}
