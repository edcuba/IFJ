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
	linear_item *actualItem = self->code->first;
	token_stack *stack = ifj_stack_new();

	while (actualItem != NULL)
	{
		instruction *instruc = actualItem->data;

		switch (instruc->type)
		{
			case I_MUL:
				/*
				if (instruc->op1 == NULL)
					instruc->op1 = ifj_stack_pop(stack);
				if (instruc->op2 == NULL)
					instruc->op2 = ifj_stack_pop(stack);
				*/

				// Calculate c = a * b
				//ifj_stack_push(c);

				break;

			case I_ADD:
				/*
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
				*/
				break;

			case I_SUB:
				/*
				if (instruc->op1 == NULL)
					instruc->op1 = ifj_stack_pop(stack);
				if (instruc->op2 == NULL)
					instruc->op2 = ifj_stack_pop(stack);
				*/

				// Calculate c = a - b
				//ifj_stack_push(stack, c);
				break;

			case I_DIV:
				/*
				if (instruc->op1 == NULL)
					instruc->op1 = ifj_stack_pop(stack);
				if (instruc->op2 == NULL)
					instruc->op2 = ifj_stack_pop(stack);

				if (instruc->op2->value == 0)
					fprintf(stderr, "%s\n", "Division by zero");
					break;
				*/

				// Calculate c = a / b
				//ifj_stack_push(stack, c);
				break;

			case I_PUSH:
				ifj_stack_push(stack, instruc->op1);
				break;

			case I_SET:
				/*
				if (instruc->op1 == NULL)
					instruc->op1 = ifj_stack_pop(stack);

				if (instruc->op1->type == T_STRING)
				{
					if (instruc->op3->value)
						free(instruc->op3->value);
					instruc->op3->value = strdup(instruc->op1->value);
				}
				else
				{
					instruc->op3 = instruc->op1;
				}
				*/

				break;

			case I_CALL:
				// Daco urobit s argumentmy na stacku, _dakam_ = ifj_stack_pop(stack);
				//ifj_stack_push(stuck, instruc->op1->next);	// adresa nasledujuceho itemu v lin.zozname
				// goto(a->label)	// Asi vytvorit novy item v lin.zozname s goto instrukciou
				break;

			case I_GOTO:
				//actualItem = instruc->op1;
				break;

			case I_GOTO_CONDITION:
				//output = ifj_stack_pop(stack);
				//if (output)
					//goto(instruc->op3);
				break;

			case I_RETURN:
				//label = ifj_stack_pop(stack);
				if (instruc->op1 != NULL)
					ifj_stack_push(stack, instruc->op1);
				// goto(label)
				break;

			case I_CONDITION:
			{
				bool output = checkCondition(instruc->op1, instruc->op2, instruc->op3);
				output = !output;
				//ifj_stack_push(stack, output);
				break;
			}

			case I_LABEL:
				break;

			default:
				/*
				fprintf(stderr, "%s %d\n", "Undefined instruction, value: " + actualItem->data->type);
				*/
				break;
		}

		actualItem = actualItem->next;
	}
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
