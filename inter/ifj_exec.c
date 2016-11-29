/* ifj-exec.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Robert Kolcun <xkolcu00@stud.fit.vutbr.cz>
 */

#include "ifj_exec.h"
#include "ifj_inter.h"
#include "ifj_util.h"
#include "ifj_lexa.h"
#include "ifj_token.h"
#include "ial.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

static inline void print_not_initialized (
	ifjInter *self,
	token *item,
	token_stack *inStack,
	token_stack *contextStack
	)
{
	if (item && item->value)
	{
		fprintf(stderr,"Error: variable \"%s\" is not inicialized\n",
			(char *) item->value);
	}
	else
	{
		fprintf(stderr,"Error: variable is not inicialized\n");
	}

	ifj_stack_drop(inStack);
	ifj_stack_drop(contextStack);
}

int exec_run ( ifjInter *self )
{
	if (self->debugMode)
	{
		fprintf(stderr, "%s\n", "---------------- Executor started ----------------");
	}

	instruction *instruc = self->code->first;
	token_stack *stack = ifj_stack_new();

	token_stack *contextStack = ifj_stack_new();
	ifj_stack_push(contextStack, NULL);

	bool jumped = false;

	while (instruc != NULL)
	{
		if (self->debugMode)
		{
			printInstruction(instruc);
			//ifj_stack_print(stack);
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

				token *dupOp1 = resolve_context(self, instruc->op1, ifj_stack_top(contextStack));
				token *dupOp2 = resolve_context(self, instruc->op2, ifj_stack_top(contextStack));

				// Check if variables are inicialized
				if (!dupOp1->data)
				{
					print_not_initialized(self, dupOp1, stack, contextStack);
					self->returnCode = 8;
					return 8;
				}
				if (!dupOp2->data)
				{
					print_not_initialized(self, dupOp2, stack, contextStack);
					self->returnCode = 8;
					return 8;
				}

				// Calculate (a * b)
				token *tempToken = NULL;
				if (dupOp1->dataType == T_DOUBLE)
				{
					double result;
					if (dupOp2->dataType == T_DOUBLE)
					{
						// double = double * double
						result = *((double *) dupOp1->data) * *((double *) dupOp2->data);
					}
					else
					{
						// double = double * int
						result = *((double *) dupOp1->data) * (double) *((int *) dupOp2->data);
					}

					tempToken = ifj_generate_temp(
						T_DOUBLE,
						&result
						);
				}
				else
				{
					if (dupOp2->dataType == T_DOUBLE)
					{
						// double = int * double
						double result = (double) *((int *) dupOp1->data) * *((double *) dupOp2->data);
						tempToken = ifj_generate_temp(
							T_DOUBLE,
							&result
							);
					}
					else
					{
						// int = int * int
						int result = *((int *) dupOp1->data) * *((int *) dupOp2->data);
						tempToken = ifj_generate_temp(
							T_INTEGER,
							&result
							);
					}
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

				token *dupOp1 = resolve_context(self, instruc->op1, ifj_stack_top(contextStack));
				token *dupOp2 = resolve_context(self, instruc->op2, ifj_stack_top(contextStack));

				// Check if variables are inicialized
				if (!dupOp1->data)
				{
					print_not_initialized(self, dupOp1, stack, contextStack);
					self->returnCode = 8;
					return 8;
				}
				if (!dupOp2->data)
				{
					print_not_initialized(self, dupOp2, stack, contextStack);
					self->returnCode = 8;
					return 8;
				}

				// Calculate (a + b)
				token *tempToken = NULL;
				if (dupOp1->dataType == T_STRING)
				{
					char *completString = ifj_join_strings(
						dupOp1->data,
						dupOp2->data
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
					if (dupOp1->dataType == T_DOUBLE)
					{
						double result;
						if (dupOp2->dataType == T_DOUBLE)
						{
							// double = double + double
							result = *((double *) dupOp1->data) + *((double *) dupOp2->data);
						}
						else
						{
							// double = double + int
							result = *((double *) dupOp1->data) + (double) *((int *) dupOp2->data);
						}

						tempToken = ifj_generate_temp(
							T_DOUBLE,
							&result
							);
					}
					else
					{
						if (dupOp2->dataType == T_DOUBLE)
						{
							// double =int + double
							double result = (double) *((int *) dupOp1->data) + *((double *) dupOp2->data);
							tempToken = ifj_generate_temp(
								T_DOUBLE,
								&result
								);
						}
						else
						{
							// int = int + int
							int result = *((int *) dupOp1->data) + *((int *) dupOp2->data);
							tempToken = ifj_generate_temp(
								T_INTEGER,
								&result
								);
						}
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

				token *dupOp1 = resolve_context(self, instruc->op1, ifj_stack_top(contextStack));
				token *dupOp2 = resolve_context(self, instruc->op2, ifj_stack_top(contextStack));

				// Check if variables are inicialized
				if (!dupOp1->data)
				{
					print_not_initialized(self, dupOp1, stack, contextStack);
					self->returnCode = 8;
					return 8;
				}
				if (!dupOp2->data)
				{
					print_not_initialized(self, dupOp2, stack, contextStack);
					self->returnCode = 8;
					return 8;
				}

				// Calculate (a - b)
				token *tempToken = NULL;
				if (dupOp1->dataType == T_DOUBLE)
				{
					double result;
					if (dupOp2->dataType == T_DOUBLE)
					{
						// double - double
						result = *((double *) dupOp1->data) - *((double *) dupOp2->data);
					}
					else
					{
						// double - int
						result = *((double *) dupOp1->data) - (double) *((int *) dupOp2->data);
					}

					tempToken = ifj_generate_temp(
						T_DOUBLE,
						&result
						);
				}
				else
				{
					if (dupOp2->dataType == T_DOUBLE)
					{
						// double = int - double
						double result = (double) *((int *) dupOp1->data) - *((double *) dupOp2->data);
						tempToken = ifj_generate_temp(
							T_DOUBLE,
							&result
							);
					}
					else
					{
						// int = int - int
						int result = *((int *) dupOp1->data) - *((int *) dupOp2->data);
						tempToken = ifj_generate_temp(
							T_INTEGER,
							&result
							);
					}
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

				token *dupOp1 = resolve_context(self, instruc->op1, ifj_stack_top(contextStack));
				token *dupOp2 = resolve_context(self, instruc->op2, ifj_stack_top(contextStack));

				// Check if variables are inicialized
				if (!dupOp1->data)
				{
					print_not_initialized(self, dupOp1, stack, contextStack);
					self->returnCode = 8;
					return 8;
				}
				if (!dupOp2->data)
				{
					print_not_initialized(self, dupOp2, stack, contextStack);
					self->returnCode = 8;
					return 8;
				}

				// Check division by zero
				if ((dupOp2->dataType == T_INTEGER && *((int *) dupOp2->data) == 0) ||
					(dupOp2->dataType == T_DOUBLE  && *((double *) dupOp2->data) == (double) 0))
				{
					ifj_stack_drop(contextStack);
					ifj_stack_drop(stack);
					fprintf(stderr, "%s\n", "Division by zero");
					self->returnCode = 9;
					return 9;
				}

				// Calculate (a / b)
				token *tempToken = NULL;
				if (dupOp1->dataType == T_DOUBLE)
				{
					double result;
					if (dupOp2->dataType == T_DOUBLE)
					{
						// double = double / double
						result = *((double *) dupOp1->data) / *((double *) dupOp2->data);
					}
					else
					{
						// double = double / int
						result = *((double *) dupOp1->data) / (double) *((int *) dupOp2->data);
					}

					tempToken = ifj_generate_temp(
						T_DOUBLE,
						&result
						);
				}
				else
				{
					if (dupOp2->dataType == T_DOUBLE)
					{
						// double = int / double
						double result = (double) *((int *) dupOp1->data) / *((double *) dupOp2->data);
						tempToken = ifj_generate_temp(
							T_DOUBLE,
							&result
							);
					}
					else
					{
						// int = int / int
						int result = *((int *) dupOp1->data) / *((int *) dupOp2->data);
						tempToken = ifj_generate_temp(
							T_INTEGER,
							&result
							);
					}
				}

				// Push temp token, and free temp tokens
				ifj_stack_push(stack, tempToken);
				freeTempTokens(instruc);
				break;
			}

			case I_PUSH:
			{
				token *dupOp1 = resolve_context(self, instruc->op1, ifj_stack_top(contextStack));
				ifj_stack_push(stack, dupOp1);
				break;
			}

			case I_SET:
			{
				// Get token from stack
				if (instruc->op1 == NULL)
				{
					instruc->op1 = ifj_stack_pop(stack);
				}

				token *dupOp1 = resolve_context(self, instruc->op1, ifj_stack_top(contextStack));
				token *dupOp3 = resolve_context(self, instruc->op3, ifj_stack_top(contextStack));

				// Check if variable is inicialized
				if (dupOp1 && dupOp1->data == NULL)
				{
					print_not_initialized(self, dupOp1, stack, contextStack);
					self->returnCode = 8;
					return 8;
				}

				// Function called with return value,
				// but return value is not needed to set
				if (dupOp3 == NULL)
				{
					ifj_stack_pop(stack);
					break;
				}

				// Set value
				if (dupOp1->dataType == T_STRING)
				{
					if ((char *) dupOp3->data)
						free((char *) dupOp3->data);

					dupOp3->data = (void *) strdup(dupOp1->data);
				}
				else
				{
					if (dupOp3->dataType == T_DOUBLE)
					{
						if(!dupOp3->data)
						{
							dupOp3->data = malloc(sizeof(double));
						}

						if (dupOp1->dataType == T_DOUBLE)
						{
							// double a = 3.14(double)
							*((double *) dupOp3->data) = *((double *) dupOp1->data);
						}
						else
						{
							// double a = 1(int)
							*((double *) dupOp3->data) = (double) *((int *) dupOp1->data);
						}
					}

					if (dupOp3->dataType == T_INTEGER)
					{
						if(!dupOp3->data)
						{
							dupOp3->data = malloc(sizeof(int));
						}

						if (dupOp1->dataType == T_DOUBLE)
						{
							// int a = 3.14(double)
							*((int *) dupOp3->data) = (int) *((double *) dupOp1->data);
						}
						else
						{
							// int a = 2(int)
							*((int *) dupOp3->data) = *((int *) dupOp1->data);
						}
					}
				}

				// Free temp token
				freeTempTokens(instruc);
				break;
			}

			case I_CALL:
			{
				// Context....
				token *dupOp1 = instruc->op1;

				//0 for variable 1 for statics > 100 for ifj16 methods 
				if (dupOp1->method == 1)
				{
					dupOp1 = duplicate_context(instruc->op1);
					ifj_stack_push(contextStack, dupOp1);
				}

				// Stack of arguments for IFJ16 functions
				token_stack *argsStack = ifj_stack_new();

				if (dupOp1->args != NULL)
				{
					for (int i = dupOp1->args->top; i >= 0; i--)
					{
						token *myToken = ifj_stack_pop(stack);

						// Check if variable is inicialized
						if (myToken->data == NULL)
						{
							ifj_stack_drop(argsStack);
							print_not_initialized(self, myToken, stack, contextStack);
							self->returnCode = 8;
							return 8;
						}

						ifj_stack_push(argsStack, myToken);
						token *argToken = dupOp1->args->elements[i];

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
								if (myToken->dataType == T_DOUBLE)
								{
									// double a = 3.14(double)
									*((double *) argToken->data) = *((double *) myToken->data);
								}
								else
								{
									// double a = 1(int)
									*((double *) argToken->data) = (double) *((int *) myToken->data);
								}
								break;

							case T_INTEGER:
								if (myToken->dataType == T_DOUBLE)
								{
									// int a = 3.14(double)
									ifj_stack_drop(argsStack);
									ifj_stack_drop(contextStack);
									ifj_stack_drop(stack);
									fprintf(stderr, "%s\n", "Invalid implicit declaration in function call");
									self->returnCode = 4;
									return 4;
								}
								else
								{
									// int a = 2(int)
									*((int *) argToken->data) = *((int *) myToken->data);
								}
								break;

							default:
								ifj_stack_drop(argsStack);
								ifj_stack_drop(contextStack);
								ifj_stack_drop(stack);
								fprintf(stderr, "%s %d\n", "Executor ERROR, invalid dataType: ", argToken->dataType);
								self->returnCode = 10;
								return 10;
						}
					}
				}

				token *output = NULL;
				switch(dupOp1->method)
				{
					case IFJ16_PRINT:
						output = ifj_stack_pop(stack);
						ifj_print(stack, *((int *) output->data));
						output = NULL;
						break;

					case IFJ16_READINT:
						output = ifj_read_int();
						if (!output)
						{
							self->returnCode = 7;
						}
						break;

					case IFJ16_READDOUBLE:
						output = ifj_read_double();
						if (!output)
						{
							self->returnCode = 7;
						}
						break;

					case IFJ16_READSTRING:
						output = ifj_read_string();
						if (!output)
						{
							self->returnCode = 10;
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
						}
						break;

					case IFJ16_SORT:
						output = ifj_sort(
							(char *) ifj_stack_pop(argsStack)->data
							);
						if (!output)
						{
							self->returnCode = 10;
						}
						break;

					case IFJ16_LENGTH:
						output = ifj_length(
							(char *) ifj_stack_pop(argsStack)->data
							);
						if (!output)
						{
							self->returnCode = 10;
						}
						break;

					case IFJ16_SUBSTR:
						output = ifj_substr(
							*((int *) ifj_stack_pop(argsStack)->data),
							*((int *) ifj_stack_pop(argsStack)->data),
							(char *) ifj_stack_pop(argsStack)->data
							);
						if (!output)
						{
							self->returnCode = 10;
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
						}
						break;

					default:
						output = ifj_generate_temp(T_VOID, NULL);
						output->jump = instruc->next;
						instruc = dupOp1->jump->next;
						jumped = true;
						break;
				}

				if (output != NULL)
				{
					ifj_stack_push(stack, output);
				}

				ifj_stack_drop(argsStack);

				if (self->returnCode == 10 || self->returnCode == 7)
				{
					ifj_stack_drop(contextStack);
					ifj_stack_drop(stack);
					return self->returnCode;
				}

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
					else
					{
						instruc->op1 = NULL;
					}
				}

				token *dupOp1 = resolve_context(self, instruc->op1, ifj_stack_top(contextStack));
				token *label = ifj_stack_pop(stack);

				if (dupOp1 != NULL)
				{
					// Check if variable is inicialized
					if (dupOp1->data == NULL)
					{
						if (label->type == T_TMP)
						{
							ifj_token_free(label);
						}
						
						instruc->op1 = NULL;
						print_not_initialized(self, dupOp1, stack, contextStack);
						self->returnCode = 8;
						return 8;
					}

					// Generate temp token with return value
					token *temp = NULL;
					if (dupOp1->dataType == T_STRING)
					{
						temp = ifj_generate_temp(T_STRING, strdup(dupOp1->data));
					}
					else
					{
						temp = ifj_generate_temp(dupOp1->dataType, dupOp1->data);
					}

					ifj_stack_push(stack, temp);

					if (dupOp1->type == T_TMP)
					{
						ifj_token_free(dupOp1);
					}
				}

				instruction *tempInstruct = instruc;
				instruc = label->jump;
				jumped = true;

				// Free temp token
				if (label->type == T_TMP)
					ifj_token_free(label);

				tempInstruct->op1 = NULL;

				token *context = ifj_stack_pop(contextStack);
				if (context)
					ifj_token_free(context);

				break;
			}

			case I_CONDITION:
			{
				// Get tokens from stack
				if (instruc->op2 == NULL)
				{
					instruc->op2 = ifj_stack_pop(stack);
				}

				token *dupOp2 = resolve_context(self, instruc->op2, ifj_stack_top(contextStack));

				int output = 0;
				if (instruc->op3 != NULL)
				{
					if (instruc->op1 == NULL)
					{
						instruc->op1 = ifj_stack_pop(stack);
					}

					token *dupOp1 = resolve_context(self, instruc->op1, ifj_stack_top(contextStack));

					output = checkCondition(dupOp1, dupOp2, instruc->op3);
					if (output == 10)
					{
						ifj_stack_drop(contextStack);
						ifj_stack_drop(stack);
						fprintf(stderr, "%s\n", "Executor ERROR: condition error");
						self->returnCode = 10;
						return 10;
					}

					output = !output;
				}

				token *tempToken = ifj_generate_temp(
					T_INTEGER,
					&output
					);

				// Push condition result
				ifj_stack_push(stack, tempToken);

				// Free temp tokens
				freeTempTokens(instruc);
				break;
			}

			case I_LABEL:
			{
				break;
			}

			case I_END:
			{
				token *item = instruc->op1;
				fprintf(stderr, "Error: no return value in non-void function \"%s\"\n",
						(char *) item->value);

				ifj_stack_drop(contextStack);
				ifj_stack_drop(stack);
				self->returnCode = 8;
				return 8;

			}

			case I_RUN_END:
			{
				// Correct end of program
				if (self->debugMode)
				{
					fprintf(stderr, "%s\n", "---------------- Executor ended ----------------");
				}

				ifj_stack_drop(contextStack);
				ifj_stack_drop(stack);
				self->returnCode = 0;
				return 0;
			}

			default:
			{
				fprintf(stderr, "%s %d\n", "Executor ERROR: Undefined instruction, value: ", instruc->type);

				ifj_stack_drop(contextStack);
				ifj_stack_drop(stack);
				self->returnCode = 10;
				return 10;
			}
		}

		if (!jumped)
		{
			instruc = instruc->next;
		}
		else
		{
			jumped = false;
		}
	}

	ifj_stack_drop(contextStack);
	ifj_stack_drop(stack);
	if (self->debugMode)
	{
		fprintf(stderr, "%s\n", "---------------- Executor ended Incorectly ----------------");
	}

	self->returnCode = 10;
	return 10;
}

int checkCondition (	token *a,
						token *b,
						token *rel )
{	
	switch(rel->type)
	{
		case T_LESS:
			if (a->dataType == T_DOUBLE)
			{
				if (b->dataType == T_DOUBLE)
				{
					return *((double *) a->data) < *((double *) b->data);
				}
				else
				{
					return *((double *) a->data) < (double) *((int *) b->data);
				}
			}
			else
			{
				if (b->dataType == T_DOUBLE)
				{
					return (double) *((int *) a->data) < *((double *) b->data);
				}
				else
				{
					return *((int *) a->data) < *((int *) b->data);
				}
			}
			break;
	
		case T_GREATER:
			if (a->dataType == T_DOUBLE)
			{
				if (b->dataType == T_DOUBLE)
				{
					return *((double *) a->data) > *((double *) b->data);
				}
				else
				{
					return *((double *) a->data) > (double) *((int *) b->data);
				}
			}
			else
			{
				if (b->dataType == T_DOUBLE)
				{
					return (double) *((int *) a->data) > *((double *) b->data);
				}
				else
				{
					return *((int *) a->data) > *((int *) b->data);
				}
			}
			break;

		case T_LESS_EQUAL:
			if (a->dataType == T_DOUBLE)
			{
				if (b->dataType == T_DOUBLE)
				{
					return *((double *) a->data) <= *((double *) b->data);
				}
				else
				{
					return *((double *) a->data) <= (double) *((int *) b->data);
				}
			}
			else
			{
				if (b->dataType == T_DOUBLE)
				{
					return (double) *((int *) a->data) <= *((double *) b->data);
				}
				else
				{
					return *((int *) a->data) <= *((int *) b->data);
				}
			}
			break;
	
		case T_GREATER_EQUAL:
			if (a->dataType == T_DOUBLE)
			{
				if (b->dataType == T_DOUBLE)
				{
					return *((double *) a->data) >= *((double *) b->data);
				}
				else
				{
					return *((double *) a->data) >= (double) *((int *) b->data);
				}
			}
			else
			{
				if (b->dataType == T_DOUBLE)
				{
					return (double) *((int *) a->data) >= *((double *) b->data);
				}
				else
				{
					return *((int *) a->data) >= *((int *) b->data);
				}
			}
			break;
	
		case T_EQUAL:
			if (a->dataType == T_DOUBLE)
			{
				if (b->dataType == T_DOUBLE)
				{
					return *((double *) a->data) == *((double *) b->data);
				}
				else
				{
					return *((double *) a->data) == (double) *((int *) b->data);
				}
			}
			else
			{
				if (b->dataType == T_DOUBLE)
				{
					return (double) *((int *) a->data) == *((double *) b->data);
				}
				else
				{
					return *((int *) a->data) == *((int *) b->data);
				}
			}
			break;

		case T_NOT_EQUAL:
			if (a->dataType == T_DOUBLE)
			{
				if (b->dataType == T_DOUBLE)
				{
					return *((double *) a->data) != *((double *) b->data);
				}
				else
				{
					return *((double *) a->data) != (double) *((int *) b->data);
				}
			}
			else
			{
				if (b->dataType == T_DOUBLE)
				{
					return (double) *((int *) a->data) != *((double *) b->data);
				}
				else
				{
					return *((int *) a->data) != *((int *) b->data);
				}
			}
			break;

		default:
			return 10;
			break;
	}
}

void freeTempTokens (instruction *inputInstruc)
{
	if (inputInstruc->op1 && inputInstruc->op1->type == T_TMP)
	{
		ifj_token_free(inputInstruc->op1);
	}

	if (inputInstruc->op2 && inputInstruc->op2->type == T_TMP)
	{
		ifj_token_free(inputInstruc->op2);
	}

	inputInstruc->op1 = NULL;
	inputInstruc->op2 = NULL;
}

void printInstruction(instruction *instruc)
{
	switch(instruc->type)
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
			if(instruc->op3)
				fprintf(stderr, "%s %s\n", "SET", (char *) instruc->op3->value);
			else
				fprintf(stderr, "%s\n", "SET");
			break;
		case I_CALL:
			if(instruc->op1)
				fprintf(stderr, "%s %s\n", "CALL", (char *) instruc->op1->value);
			else
				fprintf(stderr, "%s\n", "CALL");
			break;
		case I_GOTO:
			if(instruc->op3)
				fprintf(stderr, "%s %s\n", "GOTO", (char *) instruc->op3->value);
			else
				fprintf(stderr, "%s\n", "GOTO");
			break;
		case I_GOTO_CONDITION:
			fprintf(stderr, "%s\n", "GOTO_CONDITION");
			break;
		case I_RETURN:
			fprintf(stderr, "%s\n", "RETURN");
			break;
		case I_CONDITION:
			if (instruc->op3)
				fprintf(stderr, "%s %d\n", "CONDITION", instruc->op3->type);
			else
				fprintf(stderr, "%s\n", "CONDITION");
			break;
		case I_LABEL:
			fprintf(stderr, "%s\n", "LABEL");
			break;
		case I_END:
			fprintf(stderr, "%s\n", "END");
			break;
		case I_RUN_END:
			fprintf(stderr, "%s\n", "RUN_END");
			break;
	}
}
