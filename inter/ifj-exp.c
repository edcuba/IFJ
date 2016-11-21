/* ifj-exp.c
*
* Copyright (C) 2016 SsYoloSwag41 Inc.
* Authors: Jan Demcak <xdemca01@stud.fit.vutbr.cz>
*/

/*TODO JANY Jiné než uvedené kombinace typů (včetně případných povolených implicitních kon-
verzí) ve výrazech pro popsané operátory jsou považovány za chybu 4. */

/*TODO JANY upavit tabulku priority operatorov != a == maju najnizsiu */


/*TODO JANY  vytvorit dalsi zasobnik ktory ked  dostanem konstantu alebo premennu pushnem ,
v momente ked sa bude vykonvat niejaka redukčná akcia ktora bude vyzadovat 2 operandy
skontroluje ak bude treba urobit typovie zavola Edovu funkciu na pretypovanie a
vysledny typ pushne na zasobnik s5 , nevyhodnocujem  vysledok  resp. nenazim sa manualne pocitat vysledok
to budu robit robove instrukcie ktorych volanie ja en d pisem do kodu ? TODO EDO skontroluj ci som dobre pochopil*/



/* TODO EDO , toto riesis ty  pri typovani ci ja ? Relační operátory nepodporují porovnání řetězců
(viz vestavěná funkce ifj16.compare ) */

#include "ifj-exp.h"

int condition(ifjInter *self, symbolTable *table)
{
    int return_value = 1;
    ifjSyna *syna = self->syna;

    int b;
    int a; // first symbol on stack is automatically $ --> 7;

    token * top_stack;
    token * top_on_help_stack;
    token * active = lexa_next_token(self->lexa_module, table);
    ifj_stack_clear(syna->stack);
    ifj_stack_clear(syna->help_stack);

    fprintf(stderr, "Som v condition\n");

    ifj_stack_push(syna->stack, syna->lblock);
    top_stack = ifj_stack_top(syna->stack);

    do
    {
        switch (active->type)
        {
            case T_ADD:
                b = 0;
                break;
            case T_SUBTRACT:
                b = 1;
                break;
            case T_MULTIPLY:
                b = 2;
                break;
            case T_DIVIDE:
                b = 3;
                break;
            case T_LPAREN:
                b = 4;
                break;
            case T_RPAREN:
                b = 5;
                break;
            case T_IDENTIFIER:
                return_value = resolve_identifier(self, table, &active, 0);
                b = 6;
                break;
            case T_LBLOCK:
                b = 7;
                break;
            case T_STRING_C:
                b = 8;
                break;
            case T_INTEGER_C:
                b = 9;
                break;
            case T_DOUBLE_C:
                b = 10;
                break;
            case T_GREATER:
                b = 11;
                break;
            case T_LESS:
                b = 12;
                break;
            case T_EQUAL:
                b = 13;
                break;
            case T_GREATER_EQUAL:
                b = 14;
                break;
            case T_LESS_EQUAL:
                b = 15;
                break;
            case T_NOT_EQUAL:
                b = 16;
                break;
            default:
                return -1;
                break;
        }

        switch (top_stack->type)
        {
            case T_ADD:
                a = 0;
                break;
            case T_SUBTRACT:
                a = 1;
                break;
            case T_MULTIPLY:
                a = 2;
                break;
            case T_DIVIDE:
                a = 3;
                break;
            case T_LPAREN:
                a = 4;
                break;
            case T_RPAREN:
                a = 5;
                break;
            case T_IDENTIFIER:
                a = 6;
                break;
            case T_LBLOCK:
                a = 7;
                break;
            case T_STRING_C:
                a = 8;
                break;
            case T_INTEGER_C:
                a = 9;
                break;
            case T_DOUBLE_C:
                a = 10;
                break;
            case T_GREATER:
                a = 11;
                break;
            case T_LESS:
                a = 12;
                break;
            case T_EQUAL:
                a = 13;
                break;
            case T_GREATER_EQUAL:
                a = 14;
                break;
            case T_LESS_EQUAL:
                a = 15;
                break;
            case T_NOT_EQUAL:
                a = 16;
                break;
            default:
                return -1;
                break;
        }

        switch ((*syna->predictCondition)[a][b])
        {
            case T_EQUAL:
                ifj_stack_push(syna->stack, active);
                top_stack = active;
                active = lexa_next_token(self->lexa_module, table);
                /*stale taha dalsie a dalsie tokeny bezime v do-while cykle*/
                break;

            case  T_LESS:
                if (ifj_stack_top(syna->stack)->type == E_TYPE)
                {
                    ifj_stack_pop(syna->stack);
                    // add T_LESS token on top of stack
                    ifj_stack_push(syna->stack, syna->t_less);
                    top_stack = active; // newone top of stack
                    ifj_stack_push(syna->stack, syna->E);
                }
                else
                {
                    // add T_LESS token on top stack
                    ifj_stack_push(syna->stack, syna->t_less);
                    top_stack = active; // newone top of stack
                }
                // add next token to the top of stack
                ifj_stack_push(syna->stack, top_stack);
                // take another token from scanner
                active = lexa_next_token(self->lexa_module, table);
            break;

            case  T_END:
                return -1;
                break;

            case T_GREATER:
                do // will fulling help_stack which one will using next
                {
                    ifj_stack_push(syna->help_stack, ifj_stack_top(syna->stack));
                    ifj_stack_pop(syna->stack);
                }
                while(ifj_stack_top(syna->stack)->type != syna->t_less->type);
                ifj_stack_pop(syna->stack); // POP  T_LESS form stack
                top_on_help_stack = ifj_stack_top(syna->help_stack);

                switch (top_on_help_stack->type)
                {
                    case T_LPAREN:
                        /* POP  save new TOP and waiting E then POP sav new TOP
                         * waiting T_LPAREN and if it is OK
                         * control if stack is empty else return -1 */
                        ifj_stack_pop(syna->help_stack);
                        top_on_help_stack = ifj_stack_top(syna->help_stack);
                        if (top_on_help_stack->type == E_TYPE) // E hash number
                        {
                            ifj_stack_pop(syna->help_stack);
                            top_on_help_stack = ifj_stack_top(syna->help_stack);
                            if (top_on_help_stack->type == T_RPAREN)
                            {
                                ifj_stack_pop(syna->help_stack);
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
                                    return -1;
                                }
                            }
                            else
                            {
                                return -1;
                            }
                        }
                        else
                        {
                            return -1;
                        }
                    break;

                case T_IDENTIFIER:
                    ifj_stack_pop(syna->help_stack);
                    if (ifj_stack_empty(syna->help_stack))
                    {

                        top_stack = ifj_stack_top(syna->stack);
                        ifj_stack_push(syna->stack, syna->E);
                        if(self->debugMode)
                        {
                            fprintf(stderr, "E --> id\n");
                        }
                    }
                    else
                    {
                        return -1;
                    }
                    break;
                case T_STRING_C:
                    ifj_stack_pop(syna->help_stack);
                    if (ifj_stack_empty(syna->help_stack))
                    {
                        top_stack = ifj_stack_top(syna->stack);
                        ifj_stack_push(syna->stack, syna->E);
                        if(self->debugMode)
                        {
                            fprintf(stderr, "E --> string\n");
                        }
                    }
                    else
                    {
                        return -1;
                    }
                    break;
                case T_INTEGER_C:
                    ifj_stack_pop(syna->help_stack);
                    if (ifj_stack_empty(syna->help_stack))
                    {
                        top_stack = ifj_stack_top(syna->stack);
                        ifj_stack_push(syna->stack, syna->E);
                        if(self->debugMode)
                        {
                            fprintf(stderr, "E --> integer\n");
                        }
                    }
                    else
                    {
                        return -1;
                    }
                    break;
                case T_DOUBLE_C:
                    ifj_stack_pop(syna->help_stack);
                    if (ifj_stack_empty(syna->help_stack))
                    {
                        top_stack = ifj_stack_top(syna->stack);
                        ifj_stack_push(syna->stack, syna->E);
                        if(self->debugMode)
                        {
                            fprintf(stderr, "E --> double\n");
                        }
                    }
                    else
                    {
                        return -1;
                    }
                    break;
                case E_TYPE: //E value
                ifj_stack_pop(syna->help_stack);
                top_on_help_stack = ifj_stack_top(syna->help_stack);


                switch (top_on_help_stack->type)
                {
                    case T_ADD:
                        ifj_stack_pop(syna->help_stack);
                        top_on_help_stack = ifj_stack_top(syna->help_stack);
                        if (top_on_help_stack == syna->E)
                        {
                            ifj_stack_pop(syna->help_stack);
                            if(ifj_stack_empty(syna->help_stack))
                            {
                                top_stack = ifj_stack_top(syna->stack);
                                ifj_stack_push(syna->stack, syna->E);
                                if(self->debugMode)
                                {
                                    fprintf(stderr, "E --> E + E\n");
                                }
                            }
                            else
                            {
                                return -1;
                            }
                        }
                        else
                        {
                            return -1;
                        }
                        break;
                    case T_SUBTRACT:
                    ifj_stack_pop(syna->help_stack);
                    top_on_help_stack = ifj_stack_top(syna->help_stack);
                    if (top_on_help_stack == syna->E)
                    {
                        ifj_stack_pop(syna->help_stack);
                        if(ifj_stack_empty(syna->help_stack))
                        {
                            top_stack = ifj_stack_top(syna->stack);
                            ifj_stack_push(syna->stack, syna->E);
                            if(self->debugMode)
                            {
                                fprintf(stderr, "E --> E - E\n");

                            }
                            else
                            {
                                return -1;
                            }

                        }
                        else
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        return -1;
                    }

                    break;

                    case T_DIVIDE:ifj_stack_pop(syna->help_stack);
                    top_on_help_stack = ifj_stack_top(syna->help_stack);
                    if (top_on_help_stack == syna->E)
                    {
                        ifj_stack_pop(syna->help_stack);
                        if(ifj_stack_empty(syna->help_stack))
                        {
                            top_stack = ifj_stack_top(syna->stack);
                            ifj_stack_push(syna->stack, syna->E);
                            if(self->debugMode)
                            {
                                fprintf(stderr, "E --> E / E\n");

                            }
                        }
                        else
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        return -1;
                    }
                    break;

                    case T_MULTIPLY:
                    ifj_stack_pop(syna->help_stack);
                    top_on_help_stack = ifj_stack_top(syna->help_stack);
                    if (top_on_help_stack == syna->E)
                    {
                        ifj_stack_pop(syna->help_stack);
                        if(ifj_stack_empty(syna->help_stack))
                        {
                            top_stack = ifj_stack_top(syna->stack);
                            ifj_stack_push(syna->stack, syna->E);
                            if(self->debugMode)
                            {
                                fprintf(stderr, "E --> E * E\n");

                            }
                        }
                        else
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        return -1;
                    }
                    break;

                    case T_GREATER:
                        ifj_stack_pop(syna->help_stack);
                        top_on_help_stack = ifj_stack_top(syna->help_stack);
                        if (top_on_help_stack == syna->E)
                        {
                            ifj_stack_pop(syna->help_stack);
                            if (ifj_stack_empty(syna->help_stack))
                            {
                                top_stack = ifj_stack_top(syna->stack);
                                ifj_stack_push(syna->stack, syna->E);
                                if(self->debugMode)
                                {
                                    fprintf(stderr, "E --> E > E\n");

                                }
                            }
                            else
                            {
                                return -1;
                            }
                        }
                        else
                        {
                            return -1;
                        }
                        break;
                    case T_LESS:
                        ifj_stack_pop(syna->help_stack);
                        top_on_help_stack = ifj_stack_top(syna->help_stack);
                        if (top_on_help_stack == syna->E)
                        {
                            ifj_stack_pop(syna->help_stack);
                            if (ifj_stack_empty(syna->help_stack))
                            {
                                top_stack = ifj_stack_top(syna->stack);
                                ifj_stack_push(syna->stack, syna->E);
                                if(self->debugMode)
                                {
                                    fprintf(stderr, "E --> E < E\n");
                                }
                            }
                            else
                            {
                                return -1;
                            }

                        }
                        else
                        {
                            return -1;
                        }
                        break;
                    case T_EQUAL:
                        ifj_stack_pop(syna->help_stack);
                        top_on_help_stack = ifj_stack_top(syna->help_stack);
                        if (top_on_help_stack == syna->E)
                        {
                            ifj_stack_pop(syna->help_stack);
                            if (ifj_stack_empty(syna->help_stack))
                            {
                                top_stack = ifj_stack_top(syna->stack);
                                ifj_stack_push(syna->stack, syna->E);
                                if(self->debugMode)
                                {
                                    fprintf(stderr, "E --> E == E\n");
                                }
                            }
                            else
                            {
                                return -1;
                            }

                        }
                        else
                        {
                            return -1;
                        }
                        break;
                    case T_GREATER_EQUAL:
                        ifj_stack_pop(syna->help_stack);
                        top_on_help_stack = ifj_stack_top(syna->help_stack);
                        if (top_on_help_stack == syna->E)
                        {
                            ifj_stack_pop(syna->help_stack);
                            if (ifj_stack_empty(syna->help_stack))
                            {
                                top_stack = ifj_stack_top(syna->stack);
                                ifj_stack_push(syna->stack, syna->E);
                                if(self->debugMode)
                                {
                                    fprintf(stderr, "E --> E >= E\n");
                                }
                            }
                            else
                            {
                                return -1;
                            }

                        }
                        else
                        {
                            return -1;
                        }
                        break;
                    case T_LESS_EQUAL:
                        ifj_stack_pop(syna->help_stack);
                        top_on_help_stack = ifj_stack_top(syna->help_stack);
                        if (top_on_help_stack == syna->E)
                        {
                            ifj_stack_pop(syna->help_stack);
                            if (ifj_stack_empty(syna->help_stack))
                            {
                                top_stack = ifj_stack_top(syna->stack);
                                ifj_stack_push(syna->stack, syna->E);
                                if(self->debugMode)
                                {
                                    fprintf(stderr, "E --> E <= E\n");
                                }
                            }
                            else
                            {
                                return -1;
                            }
                        }
                        else
                        {
                            return -1;
                        }
                        break;
                    case T_NOT_EQUAL:
                        ifj_stack_pop(syna->help_stack);
                        top_on_help_stack = ifj_stack_top(syna->help_stack);
                        if (top_on_help_stack == syna->E)
                        {
                            ifj_stack_pop(syna->help_stack);
                            if (ifj_stack_empty(syna->help_stack))
                            {
                                top_stack = ifj_stack_top(syna->stack);
                                ifj_stack_push(syna->stack, syna->E);
                                if(self->debugMode)
                                {
                                    fprintf(stderr, "E --> E != E\n");
                                }
                            }
                            else
                            {
                                return -1;
                            }

                        }
                        else
                        {
                            return -1;
                        }
                        break;
                    default:
                    return -1;
                    break;
                }
            }
            break;
        }
    } while((active->type != T_LBLOCK) || (top_stack->type != T_LBLOCK));

    return_value = 1;
    if(self->debugMode)
    {
        fprintf(stderr, "vraciam sa z condition\n");
    }
    return return_value;
}

int expresion(ifjInter *self, symbolTable *table)
{
    int return_value = 1;

    if(self->debugMode)
    {
        fprintf(stderr, "som v expresion\n");
    }
    ifjSyna *syna = self->syna;

    int b;
    int a; // first symbol on stack is automatically $ --> 7;
    token * top_stack;
    token * top_on_help_stack;
    token * active = lexa_next_token(self->lexa_module, table);
    ifj_stack_clear(syna->stack);
    ifj_stack_clear(syna->help_stack);
    ifj_stack_push(syna->stack, syna->semicolon);
    top_stack = ifj_stack_top(syna->stack);

    do
    {
        switch (active->type)
        {
            case T_ADD:
                b = 0;
                break;
            case T_SUBTRACT:
                b = 1;
                break;
            case T_MULTIPLY:
                b = 2;
                break;
            case T_DIVIDE:
                b = 3;
                break;
            case T_LPAREN:
                b = 4;
                break;
            case T_RPAREN:
                b = 5;
                break;
            case T_IDENTIFIER:
                return_value = resolve_identifier(self, table, &active, 0);
                b = 6;
                break;
            case T_SEMICOLON:
                b = 7;
                break;
            case T_STRING_C:
                b = 8;
                break;
            case T_INTEGER_C:
                b = 9;
                break;
            case T_DOUBLE_C:
                b = 10;
                break;
            default:
                return -1;
        }

        switch (top_stack->type)
        {
            case T_ADD:
                a = 0;
                break;
            case T_SUBTRACT:
                a = 1;
                break;
            case T_MULTIPLY:
                a = 2;
                break;
            case T_DIVIDE:
                a = 3;
                break;
            case T_LPAREN:
                a = 4;
                break;
            case T_RPAREN:
                a = 5;
                break;
            case T_IDENTIFIER:
                a = 6;
                break;
            case T_SEMICOLON:
                a = 7;
                break;
            case T_STRING_C:
                a = 8;
                break;
            case T_INTEGER_C:
                a = 9;
                break;
            case T_DOUBLE_C:
                a = 10;
                break;
            default:
                return -1;
                break;
        }

      /*  if(self->debugMode)
        {
            fprintf(stderr, "a je %d a b je %d\n",a,b);
        }*/
        switch ((*syna->predictExpresion)[a][b])
        {
            case T_EQUAL:
            ifj_stack_push(syna->stack,active);
            top_stack = ifj_stack_top(syna->stack);
            active = lexa_next_token(self->lexa_module, table);
            break;

            case  T_LESS:
                if (ifj_stack_top(syna->stack)->type == E_TYPE)
                {
                    ifj_stack_pop(syna->stack);
                    ifj_stack_push(syna->stack, syna->t_less); // add T_LESS token on top stack
                    top_stack = active; // newone top of stack
                    ifj_stack_push(syna->stack, syna->E);
                }
                else
                {
                    ifj_stack_push(syna->stack, syna->t_less); // add T_LESS token on top stack
                    top_stack = active; // newone top of stack
                }

                ifj_stack_push(syna->stack, top_stack); // add next token to the top of stack
                active = lexa_next_token(self->lexa_module, table); // take another token from scanner
                break;
            /* musim niekde ulozit token co je na vrchu zasobniku,
            to je ID funkcie a predat ho rekurzivnemu sestupu
            + nezabudnut pridat "(" */
            case  T_COMMA:
            //FIXME volanie funkcie ako exp, budem potrebovať identifikátor funkcie
            //TODO JANY skontroluj toto...
                return function_parameters_for_exp(self, table, ifj_stack_top(syna->stack)); //TODO XXX
            break;
            case  T_END:
                return -1;
                break;
            case T_GREATER:
                do // will fulling help_stack which one will using next
                {
                    ifj_stack_push(syna->help_stack, ifj_stack_top(syna->stack));
                    ifj_stack_pop(syna->stack);

                }
                while(ifj_stack_top(syna->stack)->type != syna->t_less->type);

                ifj_stack_pop(syna->stack); // POP  T_LESS form stack
                top_on_help_stack = ifj_stack_top(syna->help_stack);

                switch (top_on_help_stack->type)
                {
                    case T_LPAREN:
                    ifj_stack_pop(syna->help_stack); // POP  save new TOP and waiting E then POP sav new TOP waiting T_LPAREN and if it is OK control if stack is empty else return -1
                    top_on_help_stack = ifj_stack_top(syna->help_stack);
                    if (top_on_help_stack->type == E_TYPE) // E hash number
                    {
                        ifj_stack_pop(syna->help_stack);
                        top_on_help_stack = ifj_stack_top(syna->help_stack);
                        if (top_on_help_stack->type == T_RPAREN)
                        {
                            ifj_stack_pop(syna->help_stack);
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
                                return -1;
                            }
                        }
                        else
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        return -1;
                    }
                    break;
                case T_IDENTIFIER:
                    ifj_stack_pop(syna->help_stack);
                    if (ifj_stack_empty(syna->help_stack))
                    {
                        top_stack = ifj_stack_top(syna->stack);
                        ifj_stack_push(syna->stack, syna->E);
                        if(self->debugMode)
                        {
                            fprintf(stderr, "E --> id\n");
                        }
                    }
                    else
                    {
                        return -1;
                    }
                    break;
                case T_STRING_C:
                    ifj_stack_pop(syna->help_stack);
                    if (ifj_stack_empty(syna->help_stack))
                    {
                        top_stack = ifj_stack_top(syna->stack);
                        ifj_stack_push(syna->stack, syna->E);
                        if(self->debugMode)
                        {
                            fprintf(stderr, "E --> string\n");
                        }
                    }
                    else
                    {
                        return -1;
                    }
                    break;
                case T_INTEGER_C:
                    ifj_stack_pop(syna->help_stack);
                    if (ifj_stack_empty(syna->help_stack))
                    {
                        top_stack = ifj_stack_top(syna->stack);
                        ifj_stack_push(syna->stack, syna->E);
                        if(self->debugMode)
                        {
                            fprintf(stderr, "E --> integer\n");
                        }
                    }
                    else
                    {
                        return -1;
                    }
                    break;
                case T_DOUBLE_C:
                ifj_stack_pop(syna->help_stack);
                    if (ifj_stack_empty(syna->help_stack))
                    {
                        top_stack = ifj_stack_top(syna->stack);
                        ifj_stack_push(syna->stack, syna->E);
                        if(self->debugMode)
                        {
                            fprintf(stderr, "E --> double\n");
                        }
                    }
                    else
                    {
                        return -1;
                    }
                    break;
                case E_TYPE: //E value

                    ifj_stack_pop(syna->help_stack);
                    top_on_help_stack = ifj_stack_top(syna->help_stack);
                    switch (top_on_help_stack->type)
                    {
                        case T_ADD:
                        ifj_stack_pop(syna->help_stack);
                        top_on_help_stack = ifj_stack_top(syna->help_stack);
                        if (top_on_help_stack == syna->E)
                        {
                            ifj_stack_pop(syna->help_stack);
                            if(ifj_stack_empty(syna->help_stack))
                            {
                                top_stack = ifj_stack_top(syna->stack);
                                ifj_stack_push(syna->stack, syna->E);
                                if(self->debugMode)
                                {
                                    fprintf(stderr, "E --> E + E\n");
                                }
                            }
                            else
                            {
                                return -1;
                            }
                        }
                        else
                        {
                            return -1;
                        }
                        break;

                        case T_SUBTRACT:
                            ifj_stack_pop(syna->help_stack);
                            top_on_help_stack = ifj_stack_top(syna->help_stack);
                            if (top_on_help_stack == syna->E)
                            {
                                ifj_stack_pop(syna->help_stack);
                                if(ifj_stack_empty(syna->help_stack))
                                {
                                    top_stack = ifj_stack_top(syna->stack);
                                    ifj_stack_push(syna->stack, syna->E);
                                    if(self->debugMode)
                                    {
                                        fprintf(stderr, "E --> E - E\n");
                                    }
                                    else
                                    {
                                        return -1;
                                    }

                                }
                                else
                                {
                                    return -1;
                                }
                            }
                            else
                            {
                                return -1;
                            }
                            break;
                        case T_DIVIDE:
                            ifj_stack_pop(syna->help_stack);
                            top_on_help_stack = ifj_stack_top(syna->help_stack);
                            if (top_on_help_stack == syna->E)
                            {
                                ifj_stack_pop(syna->help_stack);
                                if(ifj_stack_empty(syna->help_stack))
                                {
                                    top_stack = ifj_stack_top(syna->stack);
                                    ifj_stack_push(syna->stack, syna->E);
                                    if(self->debugMode)
                                    {
                                        fprintf(stderr, "E --> E / E\n");

                                    }
                                }
                                else
                                {
                                    return -1;
                                }
                            }
                            else
                            {
                                return -1;
                            }
                            break;
                        case T_MULTIPLY:
                            ifj_stack_pop(syna->help_stack);
                            top_on_help_stack = ifj_stack_top(syna->help_stack);
                            if (top_on_help_stack == syna->E)
                            {
                                ifj_stack_pop(syna->help_stack);
                                if(ifj_stack_empty(syna->help_stack))
                                {
                                    top_stack = ifj_stack_top(syna->stack);
                                    ifj_stack_push(syna->stack, syna->E);
                                    if(self->debugMode)
                                    {
                                        fprintf(stderr, "E --> E * E\n");
                                    }
                                }
                                else
                                {
                                    return -1;
                                }
                            }
                            else
                            {
                                return -1;
                            }
                            break;
                        default:
                            return -1;
                            break;
                    }
                    break;
                default:
                    return -1;
                    break;
            }
        }

    } while((active->type != T_SEMICOLON) || (top_stack->type != T_SEMICOLON));
    return_value = 1;
    if(self->debugMode)
    {
        fprintf(stderr, "vraciam sa z expresion\n");
    }
    return return_value;
}
