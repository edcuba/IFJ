/* ifj-exp.c
*
* Copyright (C) 2016 SsYoloSwag41 Inc.
* Authors: Jan Demcak <xdemca01@stud.fit.vutbr.cz>
*/

#define SET_RETURN(code) if(!self->returnCode){self->returnCode = code;}

#include "ifj_exp.h"

//NOTE JANY endChar = 1 ak ; || endchar = 0 ak )
int condition(ifjInter *self, symbolTable *table, int endChar)
{
    ifjSyna *syna = self->syna;

    int b;
    int a; // first symbol on stack is automatically $ --> 7;
    int rc;
    int count = 0;
    int end_condition = 1;
    int end_char_1;

    token * instructHelp;
    token * top_stack;
    token * top_on_help_stack;
    token * active = lexa_next_token(self->lexa_module, table);
    ifj_stack_clear(syna->stack);
    ifj_stack_clear(syna->help_stack);
    ifj_stack_clear(syna->type_stack);


    if(self->debugMode)
    {
        fprintf(stderr, "Som v condition\n");
    }

    // If token is ID or constant add token into type_stack
    // Type_stack is using for type control in expression
    // There are 3 places where is this construction used
    // (always when function lexa_next_token() is called)
    if (active->type == T_IDENTIFIER ||
        active->type == T_STRING_C ||
        active->type == T_INTEGER_C ||
        active ->type == T_DOUBLE_C)
    {
        if(active->type == T_IDENTIFIER)
        {
            rc = resolve_identifier(self, table, &active, 0);
            if(!rc)
                return rc;
        }
        ifj_stack_push(syna->type_stack, active);
    }

    ifj_stack_push(syna->stack, syna->semicolon);
    top_stack = ifj_stack_top(syna->stack);

    do
    {
        switch (condition_check_active(self, active, &b))
        {
            case 0:
                return 0; // Nahradenie povodnej -1
                break;

            case 2:
            {
                if(!active->dataType)
                {
                    rc = resolve_identifier(self, table, &active, 0);
                    if(!rc)
                        return rc;
                }
                break;
            }
        }

        if (!condition_check_top_stack(self, top_stack, &a))
        {
            return 0; // nahradenie povodnej -1
        }
        switch ((*syna->predictCondition)[a][b])
        {
            case T_TERNARY:
                if (ifj_stack_top(syna->stack)->type == E_TYPE)
                {
                    end_condition = 0;
                }
                else
                {
                    print_unexpected(self, active);
                    SET_RETURN(2);
                    return 0;
                }
                break;

            case T_EQUAL:
                ifj_stack_push(syna->stack, active);
                top_stack = active;
                active = lexa_next_token(self->lexa_module, table);

                if(active->type == T_IDENTIFIER)
                {
                    rc = resolve_identifier(self, table, &active, 0);
                    if(!rc)
                    {
                        return rc;
                    }
                }

                if (active->type == T_IDENTIFIER ||
                    active->type == T_STRING_C ||
                    active->type == T_INTEGER_C ||
                    active ->type == T_DOUBLE_C)
                {
                    ifj_stack_push(syna->type_stack, active);
                }

                /*stale taha dalsie a dalsie tokeny bezime v do-while cykle*/
                break;

            case  T_LESS:
                if (ifj_stack_top(syna->stack)->type == E_TYPE)
                {
                    top_stack = active; // new top of stack
                    ifj_stack_pop(syna->stack);

                    // add T_LESS token on top of stack
                    ifj_stack_push(syna->stack, syna->t_less);
                    ifj_stack_push(syna->stack, syna->E);
                }
                else
                {
                    top_stack = active; // new top of stack

                    // add T_LESS token on top stack
                    ifj_stack_push(syna->stack, syna->t_less);
                }

                // add next token to the top of stack
                ifj_stack_push(syna->stack, top_stack);

                // take another token from scanner
                active = lexa_next_token(self->lexa_module, table);

                if(active->type == T_IDENTIFIER)
                {
                    rc = resolve_identifier(self, table, &active, 0);
                    if(!rc)
                    {
                        return rc;
                    }
                }

                if (active->type == T_IDENTIFIER ||
                    active->type == T_STRING_C ||
                    active->type == T_INTEGER_C ||
                    active ->type == T_DOUBLE_C)
                {
                    ifj_stack_push(syna->type_stack, active);
                }
                break;

            case  T_END:
                print_unexpected(self, active);
                SET_RETURN(2);
                return 0; // Nahradenie povodnej -1
                break;

            case T_GREATER:
                ///FIXME JANY OPRAVIT
                do //filling the help_stack
                {
                    if(!ifj_stack_empty(syna->stack))
                    {
                        ifj_stack_push(syna->help_stack, ifj_stack_pop(syna->stack));
                    }
                    else
                    {
                        SET_RETURN(2);
                        return 0;
                    }
                }
                while(!ifj_stack_empty(syna->stack) &&
                      ifj_stack_top(syna->stack)->type != syna->t_less->type);
                ///
                
                ifj_stack_pop(syna->stack); // POP  T_LESS form stack
                top_on_help_stack = ifj_stack_pop(syna->help_stack);
                switch (top_on_help_stack->type)
                {
                    case T_LPAREN:
                        /* saving new top
                         * after T_LPAREN check
                         * if stack is empty else return 0 */
                        top_on_help_stack = ifj_stack_pop(syna->help_stack);
                        if (top_on_help_stack->type == E_TYPE) // E hash number
                        {
                            top_on_help_stack = ifj_stack_pop(syna->help_stack);
                            if (top_on_help_stack->type == T_RPAREN)
                            {
                                if (ifj_stack_empty(syna->help_stack))
                                {
                                    top_stack = ifj_stack_top(syna->stack);
                                     // push E on the top of main stack
                                    ifj_stack_push(syna->stack, syna->E);
                                    if(self->debugMode)
                                    {
                                        fprintf(stderr, "E --> (E)\n");
                                    }
                                }
                                else
                                {
                                    print_unexpected(self, active);
                                    SET_RETURN(2);
                                    return 0;
                                }
                            }
                            else
                            {
                                print_unexpected(self, active);
                                SET_RETURN(2);
                                return 0;
                            }
                        }
                        else
                        {
                            print_unexpected(self, active);
                            SET_RETURN(2);
                            return 0;
                        }
                    break;

                case T_IDENTIFIER:
                    if (E_simple_reduct(self, 1, syna, &top_stack, top_on_help_stack, active) == 0)
                    {
                        return 0;
                    }
                    break;

                case T_STRING_C:
                    if (E_simple_reduct(self, 3, syna, &top_stack, top_on_help_stack, active) == 0)
                    {
                        return 0;
                    }
                    break;

                case T_INTEGER_C:
                    if (E_simple_reduct(self, 2, syna, &top_stack, top_on_help_stack, active) == 0)
                    {
                        return 0;
                    }
                    break;

                case T_DOUBLE_C:
                    if (E_simple_reduct(self, 4, syna, &top_stack, top_on_help_stack, active) == 0)
                    {
                        return 0;
                    }
                    break;

                case E_TYPE: //E value
                    top_on_help_stack = ifj_stack_pop(syna->help_stack);
                    instructHelp = top_on_help_stack;

                    switch (top_on_help_stack->type)
                    {
                        case T_ADD:
                            if (E_reduct(self, I_ADD, 1, 1, syna, active, top_on_help_stack, &top_stack, NULL) == 0)
                            {
                                return 0;
                            }
                            break;

                        case T_SUBTRACT:
                            if (E_reduct(self, I_SUB, 2, 0, syna, active, top_on_help_stack, &top_stack, NULL) == 0)
                            {
                                return 0;
                            }
                            break;

                        case T_DIVIDE:
                            if (E_reduct(self, I_DIV, 4, 0, syna, active, top_on_help_stack, &top_stack, NULL) == 0)
                            {
                                return 0;
                            }
                            break;

                        case T_MULTIPLY:
                            if (E_reduct(self, I_MUL, 3, 0, syna, active, top_on_help_stack, &top_stack, NULL) == 0)
                            {
                                return 0;
                            }
                            break;

                        case T_GREATER:
                        case T_LESS:
                        case T_EQUAL:
                        case T_GREATER_EQUAL:
                        case T_LESS_EQUAL:
                        case T_NOT_EQUAL:
                            count++;
                            if (E_reduct(
                                    self,
                                    I_CONDITION,
                                    5,
                                    0,
                                    syna,
                                    active,
                                    top_on_help_stack,
                                    &top_stack,
                                    instructHelp
                                ) == 0)
                            {
                                return 0;
                            }
                            break;

                        default:
                            print_unexpected(self, active);
                            SET_RETURN(2);
                            return 0;
                            break;
                    }
            }
            break;
        }
    } while(end_condition && !(end_char_1 = (active->type == T_SEMICOLON && top_stack->type == T_SEMICOLON)));

    if ((end_condition == 0 && endChar == 1) || (end_char_1 == 1 && endChar == 0))
    {
        print_unexpected(self, active);
        SET_RETURN(2);
        return 0;
    }

    if(self->debugMode)
    {
        fprintf(stderr, "vraciam sa z condition\n");
    }
    if (count == 0)
    {
        print_unexpected(self, active);
        SET_RETURN(4);
    }
    return 1;
}


//------------------------------------------------------------------------------------------------------------

//NOTE JANY endChar = 1 ak ; || endchar = 0 ak )
int expression(ifjInter *self, symbolTable *table, token *expected, int endChar)
{
    if(self->debugMode)
    {
        fprintf(stderr, "som v expression\n");
    }
    ifjSyna *syna = self->syna;

    int b;
    int a; // first symbol on stack is automatically $ --> 7;
    int rc;
    int end_condition = 1;
    int end_char_1;

    token * top_stack;
    token * top_on_help_stack;
    token * active = lexa_next_token(self->lexa_module, table);
    ifj_stack_clear(syna->stack);
    ifj_stack_clear(syna->help_stack);
    ifj_stack_clear(syna->type_stack);

    if(active->type == T_SEMICOLON)
    {
        print_unexpected(self, active);
        if(expected->childTable) //non-void function and we are in return
        {
            SET_RETURN(4);
            return 0;
        }
        else //we are in some statement
        {
            SET_RETURN(2);
            return 0;
        }
    }

    if (active->type == T_IDENTIFIER ||
        active->type == T_STRING_C ||
        active->type == T_INTEGER_C ||
        active ->type == T_DOUBLE_C)
    {
        if(active->type == T_IDENTIFIER)
        {
            rc = resolve_identifier(self, table, &active, 0);
            if(!rc)
                return rc;
        }
        ifj_stack_push(syna->type_stack, active);
    }

    ifj_stack_push(syna->stack, syna->semicolon);
    top_stack = ifj_stack_top(syna->stack);

    do
    {
        switch (expression_check_active(self, active, &b))
        {
            case 0:
                return 0; // Nahradenie povodnej -1
                break;

            case 2:
                if(!active->dataType)
                {
                    rc = resolve_identifier(self, table, &active, 0);
                    if(!rc)
                        return rc;
                }
                break;
        }

        if (!expression_check_top_stack(self, top_stack, &a))
        {
            return 0; // nahradenie povodnej -1
        }

        switch ((*syna->predictexpression)[a][b])
        {
            case T_EQUAL:
                ifj_stack_push(syna->stack, active);
                top_stack = ifj_stack_top(syna->stack);
                active = lexa_next_token(self->lexa_module, table);

                if(active->type == T_IDENTIFIER)
                {
                    rc = resolve_identifier(self, table, &active, 0);
                    if(!rc)
                    {
                        return rc;
                    }
                }

                if (active->type == T_IDENTIFIER ||
                    active->type == T_STRING_C ||
                    active->type == T_INTEGER_C ||
                    active ->type == T_DOUBLE_C)
                {
                    ifj_stack_push(syna->type_stack, active);
                }
                break;

            case  T_LESS:
                if (ifj_stack_top(syna->stack)->type == E_TYPE)
                {
                    ifj_stack_pop(syna->stack);
                    ifj_stack_push(syna->stack, syna->t_less); // add T_LESS token on top stack
                    ifj_stack_push(syna->stack, syna->E);
                    top_stack = active; // new top of stack
                }
                else
                {
                    ifj_stack_push(syna->stack, syna->t_less); // add T_LESS token on top stack
                    top_stack = active; // new top of stack
                }

                ifj_stack_push(syna->stack, top_stack); // add next token to the top of stack
                active = lexa_next_token(self->lexa_module, table); // take another token from scanner

                if(active->type == T_IDENTIFIER)
                {
                    rc = resolve_identifier(self, table, &active, 0);
                    if(!rc)
                    {
                        return rc;
                    }
                }

                if (active->type == T_IDENTIFIER ||
                    active->type == T_STRING_C ||
                    active->type == T_INTEGER_C ||
                    active ->type == T_DOUBLE_C)
                {
                    ifj_stack_push(syna->type_stack, active);
                }

                break;

            /* musim niekde ulozit token co je na vrchu zasobniku,
             * to je ID funkcie a predat ho rekurzivnemu sestupu
             * + nezabudnut pridat "("
            */
            case T_TERNARY:
                if (ifj_stack_top(syna->stack)->type == E_TYPE)
                {
                    end_condition = 0;
                }
                else
                {
                    print_unexpected(self, active);
                    SET_RETURN(2);
                    return 0;
                }
                break;

            case  T_COMMA:
                active = ifj_stack_top(syna->stack);
                if(active->dataType == T_VOID)
                {
                    SET_RETURN(8);
                    print_mistyped(self, active, expected);
                    return 0;
                }
                if(!check_typing(active, expected))
                {
                    SET_RETURN(4);
                    print_mistyped(self, active, expected);
                    return 0;
                }
                ifj_stack_pop(syna->stack);
                ifj_stack_pop(syna->stack);
                if (ifj_stack_top(syna->stack)->type  !=  T_SEMICOLON)
                {
                    print_unexpected(self, active);
                    SET_RETURN(2);
                    return 0;
                }
                return function_parameters(self, table, active) &&
                       ifj_insert_last(self->code, I_CALL, active, NULL, NULL);

            case  T_END:
                print_unexpected(self, active);
                SET_RETURN(2);
                return 0;

            case T_GREATER:

                do // will fulling help_stack which one will using next
                {
                    ifj_stack_push(syna->help_stack, ifj_stack_pop(syna->stack));
                } while(ifj_stack_top(syna->stack)->type != syna->t_less->type);

                ifj_stack_pop(syna->stack); // POP  T_LESS form stack
                top_on_help_stack = ifj_stack_pop(syna->help_stack);

                switch (top_on_help_stack->type)
                {
                    case T_LPAREN:
                        top_on_help_stack = ifj_stack_pop(syna->help_stack);
                        if (top_on_help_stack->type == E_TYPE) // E hash number
                        {
                            top_on_help_stack = ifj_stack_pop(syna->help_stack);
                            if (top_on_help_stack->type == T_RPAREN)
                            {
                                if (ifj_stack_empty(syna->help_stack))
                                {
                                    top_stack = ifj_stack_top(syna->stack);
                                    ifj_stack_push(syna->stack, syna->E); // push E on the top of main stack

                                    if(self->debugMode)
                                    {
                                        fprintf(stderr, "E --> (E)\n");

                                    }
                                }
                                else
                                {
                                    print_unexpected(self, active);
                                    SET_RETURN(2);
                                    return 0;
                                }
                            }
                            else
                            {
                                print_unexpected(self, active);
                                SET_RETURN(2);
                                return 0;
                            }
                        }
                        else
                        {
                            print_unexpected(self, active);
                            SET_RETURN(2);
                            return 0;
                        }
                        break;

                    case T_IDENTIFIER:
                        if (E_simple_reduct(self, 1, syna, &top_stack, top_on_help_stack, active) == 0)
                        {
                            return 0;
                        }
                        break;

                    case T_STRING_C:
                        if (E_simple_reduct(self, 3, syna, &top_stack, top_on_help_stack, active) ==0)
                        {
                            return 0;
                        }
                        break;

                    case T_INTEGER_C:
                        if (E_simple_reduct(self, 2, syna, &top_stack, top_on_help_stack, active) == 0)
                        {
                            return 0;
                        }
                        break;

                    case T_DOUBLE_C:
                        if (E_simple_reduct(self, 4, syna, &top_stack, top_on_help_stack, active) == 0)
                        {
                            return 0;
                        }
                        break;

                    case E_TYPE: //E value
                        top_on_help_stack = ifj_stack_pop(syna->help_stack);
                        switch (top_on_help_stack->type)
                        {
                            case T_ADD:
                                if (E_reduct(self, I_ADD, 1, 1, syna, active, top_on_help_stack, &top_stack, NULL) == 0)
                                {
                                    return 0;
                                }

                                break;

                            case T_SUBTRACT:
                                if (E_reduct(self, I_SUB, 2, 0, syna, active, top_on_help_stack, &top_stack, NULL) == 0)
                                {
                                    return 0;
                                }
                                break;

                            case T_DIVIDE:
                                if (E_reduct(self, I_DIV, 4, 0, syna, active, top_on_help_stack, &top_stack, NULL) == 0)
                                {
                                    return 0;
                                }
                                break;

                            case T_MULTIPLY:

                                if (E_reduct(self, I_MUL, 3, 0, syna, active, top_on_help_stack, &top_stack, NULL) == 0)
                                {
                                    return 0;
                                }
                                break;

                            default:
                                print_unexpected(self, active);
                                SET_RETURN(2);
                                return 0;
                        }
                        break;

                    default:
                        print_unexpected(self, active);
                        SET_RETURN(2);
                        return 0;
            }
        }

    } while(end_condition && !(end_char_1 = (active->type == T_SEMICOLON && top_stack->type == T_SEMICOLON)));

    if ((end_condition == 0 && endChar == 1) || (end_char_1 == 1 && endChar == 0))
    {
        print_unexpected(self, active);
        SET_RETURN(2);
        return 0;
    }



    if(self->debugMode)
    {
        fprintf(stderr, "vraciam sa z expression\n");
    }

    active = ifj_stack_top(syna->type_stack);
    if(!check_typing(active, expected))
    {
        SET_RETURN(4);
        print_mistyped(self, active, expected);
        return 0;
    }

    return 1;
}

inline int condition_check_active(ifjInter *self, token *active, int *b)
{
    switch (active->type)
    {
        case T_ADD:
        case T_SUBTRACT:
            *b = 0;
            return 1;

        case T_MULTIPLY:
        case T_DIVIDE:
            *b = 1;
            return 1;

        case T_LPAREN:
            *b = 2;
            return 1;

        case T_RPAREN:
            *b = 3;
            return 1;

        case T_SEMICOLON:
            *b= 7;
            return 1;

        case T_IDENTIFIER:
        case T_STRING_C:
        case T_INTEGER_C:
        case T_DOUBLE_C:
            *b = 4;
            return 1;

        case T_EQUAL:
        case T_NOT_EQUAL:
            *b = 5;
            return 1;

        case T_GREATER:
        case T_LESS:
        case T_GREATER_EQUAL:
        case T_LESS_EQUAL:
            *b = 6;
            return 1;
    }

    *b = -1;
    print_unexpected(self, active);
    SET_RETURN(2);
    return 0;
}

inline int condition_check_top_stack(ifjInter *self, token *top_stack, int *a)
{
    switch (top_stack->type)
    {
        case T_ADD:
        case T_SUBTRACT:
            *a = 0;
            return 1;

        case T_MULTIPLY:
        case T_DIVIDE:
            *a = 1;
            return 1;

        case T_LPAREN:
            *a = 2;
            return 1;

        case T_RPAREN:
            *a = 3;
            return 1;

        case T_SEMICOLON:
            *a = 7;
            return 1;

        case T_IDENTIFIER:
        case T_STRING_C:
        case T_INTEGER_C:
        case T_DOUBLE_C:
            *a = 4;
            return 1;

        case T_EQUAL:
        case T_NOT_EQUAL:
            *a = 5;
            return 1;

        case T_GREATER:
        case T_LESS:
        case T_GREATER_EQUAL:
        case T_LESS_EQUAL:
            *a = 6;
            return 1;


    }

    *a = -1;
    print_unexpected(self, top_stack);
    SET_RETURN(2);
    return 0;
}

inline int expression_check_active(ifjInter *self, token *active, int *b)
{
    switch (active->type)
    {
        case T_ADD:
        case T_SUBTRACT:
            *b = 0;
            return 1;

        case T_MULTIPLY:
        case T_DIVIDE:
            *b = 1;
            return 1;

        case T_LPAREN:
            *b = 2;
            return 1;

        case T_RPAREN:
            *b = 3;
            return 1;

        case T_SEMICOLON:
            *b = 5;
            return 1;

        case T_IDENTIFIER:
        case T_STRING_C:
        case T_INTEGER_C:
        case T_DOUBLE_C:
            *b = 4;
            return 1;
    }

    *b = -1;
    print_unexpected(self, active);
    SET_RETURN(2);
    return 0;
}

inline int expression_check_top_stack(ifjInter *self, token *top_stack, int *a)
{
    switch (top_stack->type)
    {
        case T_ADD:
        case T_SUBTRACT:
            *a = 0;
            return 1;

        case T_MULTIPLY:
        case T_DIVIDE:
            *a = 1;
            return 1;

        case T_LPAREN:
            *a = 2;
            return 1;

        case T_RPAREN:
            *a = 3;
            return 1;

        case T_SEMICOLON:
            *a = 5;
            return 1;

        case T_IDENTIFIER:
        case T_STRING_C:
        case T_INTEGER_C:
        case T_DOUBLE_C:
            *a = 4;
            return 1;


    }

    *a = -1;
    print_unexpected(self, top_stack);
    SET_RETURN(2);
    return 0;
}

int type_control(ifjInter *self)
{
    ifjSyna *syna = self->syna;
    token * first_type_stack_token = ifj_stack_pop(syna->type_stack);
    token * second_type_stack_token = ifj_stack_pop(syna->type_stack);

    if ((first_type_stack_token->dataType == T_STRING) || (second_type_stack_token->dataType == T_STRING))
    {
        print_mistyped(self, first_type_stack_token, second_type_stack_token);
        SET_RETURN(4);
        return 4;
    }
    else
    {
        ifj_stack_push(syna->type_stack, first_type_stack_token);
        return 0;
    }
}

int type_control_plus(ifjInter *self)
{
    ifjSyna *syna = self->syna;
    token * first_type_stack_token = ifj_stack_pop(syna->type_stack);
    token * second_type_stack_token = ifj_stack_pop(syna->type_stack);

    if (first_type_stack_token->dataType == T_STRING && second_type_stack_token->dataType == T_STRING)
    {
        ifj_stack_push(syna->type_stack, first_type_stack_token);
        return 0;
    }
    else if (first_type_stack_token->dataType == T_STRING || second_type_stack_token->dataType == T_STRING)
    {
        print_mistyped(self, first_type_stack_token, second_type_stack_token);
        SET_RETURN(4);
        return 4;
    }
    else
    {
        ifj_stack_push(syna->type_stack, first_type_stack_token);
        return 0;
    }

    return 0;
}


int E_reduct(ifjInter *self,
             int insert,
             int debug_constant,
             int plus_operation,
             ifjSyna *syna,
             token * active,
             token * top_on_help_stack,
             token **top_stack,
             token * instructHelp)
{
    top_on_help_stack = ifj_stack_pop(syna->help_stack);
    if (top_on_help_stack == syna->E)
    {
        if(ifj_stack_empty(syna->help_stack))
        {
            *top_stack = ifj_stack_top(syna->stack);

            ifj_stack_push(syna->stack, syna->E);
            if (plus_operation)
            {
                if(type_control_plus(self) == 4)
                {
                    print_unexpected(self,active);
                    SET_RETURN(4);
                    return 0;
                }
            }
            else
            {
                if(type_control(self) == 4)
                {
                    print_unexpected(self, active);
                    SET_RETURN(4);
                    return 0;
                }
            }
            if (insert == I_CONDITION)
            {
                ifj_insert_last(self->code, I_CONDITION, NULL, NULL, instructHelp);
            }
            else
            {
                ifj_insert_last(self->code, insert, NULL, NULL, NULL);
            }

            if(self->debugMode)
            {
                switch (debug_constant)
                {
                    case 1:
                        fprintf(stderr, "E --> E + E\n");
                        break;

                    case 2:
                        fprintf(stderr, "E --> E - E\n");
                        break;

                    case 3:
                        fprintf(stderr, "E --> E * E\n");
                        break;

                    case 4:
                        fprintf(stderr, "E --> E / E\n");
                        break;

                    case 5:
                        fprintf(stderr, "E --> E rel_operator E");

                }
            }
        }
        else
        {
            print_unexpected(self, active);
            SET_RETURN(2);
            return 0;
        }
    }
    else
    {
        print_unexpected(self, active);
        SET_RETURN(2);
        return 0;
    }
    return 1;

}

int E_simple_reduct(ifjInter *self,
                    int debug_constant,
                    ifjSyna *syna,
                    token **top_stack,
                    token *top_on_help_stack,
                    token *active)
{
    if (ifj_stack_empty(syna->help_stack))
    {
        *top_stack = ifj_stack_top(syna->stack);
        ifj_stack_push(syna->stack, syna->E);

        ifj_insert_last(self->code, I_PUSH, top_on_help_stack, NULL, NULL);

        if(self->debugMode)
        {
            switch (debug_constant)
            {
                case 1:
                    fprintf(stderr, "E --> id\n");
                    break;
                case 2:
                    fprintf(stderr, "E --> int\n");
                    break;
                case 3:
                    fprintf(stderr, "E --> string\n");
                    break;
                case 4:
                    fprintf(stderr, "E --> double\n");
                    break;
            }
        }
    }
    else
    {
        print_unexpected(self, active);
        SET_RETURN(2);
        return 0;
    }
    return 1;
}
