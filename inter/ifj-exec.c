/* ifj-exec.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Robert Kolcun <xkolcu00@stud.fit.vutbr.cz>
 */

#include "ifj-exec.h"
#include "ifj-inter.h"
#include "ifj-util.h"
#include "ifj-lexa.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void run_exec ( ifjInter *self )
{
	instruction *instruc = self->code->first;
	token_stack *stack = ifj_stack_new();

	bool jumped = false;

	while (instruc != NULL)
	{
		switch (instruc->type)
		{
			case I_MUL:
			{
				if (instruc->op1 == NULL)
					instruc->op1 = ifj_stack_pop(stack);
				if (instruc->op2 == NULL)
					instruc->op2 = ifj_stack_pop(stack);

				// Calculate c = a * b
				//ifj_stack_push(c);

				break;
			}

			case I_ADD:
			{
				if (instruc->op1 == NULL)
					instruc->op1 = ifj_stack_pop(stack);
				if (instruc->op2 == NULL)
					instruc->op2 = ifj_stack_pop(stack);
				
				if (instruc->op1->type == T_STRING)
				{
					// Allokuje string v garbarage collectore
					// Spoji 2 string a ulozi do C
					//ifj_stack_push(c);
				}
				else
				{
					// Calculate a + b
					//ifj_stack_push(c);
				}
				break;
			}

			case I_SUB:
			{
				if (instruc->op1 == NULL)
					instruc->op1 = ifj_stack_pop(stack);
				if (instruc->op2 == NULL)
					instruc->op2 = ifj_stack_pop(stack);

				// Calculate c = a - b
				//ifj_stack_push(stack, c);
				break;
			}

			case I_DIV:
			{
				if (instruc->op1 == NULL)
					instruc->op1 = ifj_stack_pop(stack);
				if (instruc->op2 == NULL)
					instruc->op2 = ifj_stack_pop(stack);

				if (instruc->op2->value == 0)
					fprintf(stderr, "%s\n", "Division by zero");
					break;

				// Calculate c = a / b
				//ifj_stack_push(stack, c);
				break;
			}

			case I_PUSH:
			{
				ifj_stack_push(stack, instruc->op1);
				break;
			}

			case I_SET:
			{
				if (instruc->op1 == NULL)
					instruc->op1 = ifj_stack_pop(stack);

				if (instruc->op1->type == T_STRING)
				{
					if (instruc->op3->value)
						free((char *) instruc->op3->value);

					instruc->op3->value = strdup(instruc->op1->value);
				}
				else
				{
					instruc->op3->value = instruc->op1->value;
				}

				break;
			}

			case I_CALL:
			{
				// Daco urobit s argumentmy na stacku, _dakam_ = ifj_stack_pop(stack);

				ifj_stack_push(stack, instruc->op1);	// token s adresou nasledujucej instrukcie v lin.zozname

				instruc = instruc->op1->jump;	// goto(a->label)
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

				if (output->value)
				{
					instruc = instruc->op3->jump;
					jumped = true;
				}
				break;
			}

			case I_RETURN:
			{
				token *label = ifj_stack_pop(stack);
				if (instruc->op1 != NULL)
					ifj_stack_push(stack, instruc->op1);
				
				instruc = label->jump;
				jumped = true;

				break;
			}

			case I_CONDITION:
			{
				bool output = checkCondition(instruc->op1, instruc->op2, instruc->op3);
				output = !output;
				//Create TMP token with bool value(integer)
				//ifj_stack_push(stack, outputToken);
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

bool checkCondition (	token *a,
						token *b,
						token *rel )
{
	switch(rel->type)
	{
		case T_LESS:
			return a->value < b->value;
			break;
		case T_GREATER:
			return a->value > b->value;
			break;

		case T_LESS_EQUAL:
			return a->value <= b->value;
			break;
		case T_GREATER_EQUAL:
			return a->value >= b->value;
			break;

		case T_EQUAL:
			return a->value == b->value;
			break;
		case T_NOT_EQUAL:
			return a->value != b->value;
			break;
	}
}
