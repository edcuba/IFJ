/* ifj-exp.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Jan Demcak <xdemca01@stud.fit.vutbr.cz>
 */
 #include "ifj-exp.h"



  int condition(ifjInter *self)
  {
    int return_value = 0;


    int predict_array [17][17] =
    {
      {T_GREATER, T_GREATER, T_LESS, T_LESS, T_LESS, T_GREATER, T_LESS, T_GREATER, T_LESS, T_LESS, T_LESS,  T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER },
      {T_GREATER, T_GREATER, T_LESS, T_LESS, T_LESS, T_GREATER, T_LESS, T_GREATER, T_LESS, T_LESS, T_LESS,  T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER },
      {T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_LESS, T_GREATER, T_LESS, T_GREATER, T_LESS, T_LESS, T_LESS,  T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER },
      {T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_LESS, T_GREATER, T_LESS, T_GREATER, T_LESS, T_LESS, T_LESS,  T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER },
      {T_LESS, T_LESS, T_LESS, T_LESS, T_LESS, T_EQUAL, T_LESS, T_END, T_LESS, T_LESS, T_LESS, T_LESS, T_LESS, T_LESS, T_LESS, T_LESS, T_LESS},
      {T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_END, T_GREATER, T_END, T_GREATER, T_END, T_END, T_END, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER},
      {T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_END, T_GREATER, T_END, T_GREATER, T_END, T_END, T_END, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER},
      {T_LESS, T_LESS, T_LESS, T_LESS, T_LESS, T_END, T_LESS, T_END, T_LESS, T_LESS, T_LESS, T_LESS, T_LESS, T_LESS, T_LESS, T_LESS, T_LESS},
      {T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_END, T_GREATER, T_END, T_GREATER, T_END, T_END, T_END, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER},
      {T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_END, T_GREATER, T_END, T_GREATER, T_END, T_END, T_END, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER},
      {T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_END, T_GREATER, T_END, T_GREATER, T_END, T_END, T_END, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER},
      {T_LESS, T_LESS, T_LESS, T_LESS, T_LESS, T_GREATER, T_LESS, T_GREATER, T_LESS, T_LESS, T_LESS, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER},
      {T_LESS, T_LESS, T_LESS, T_LESS, T_LESS, T_GREATER, T_LESS, T_GREATER, T_LESS, T_LESS, T_LESS, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER},
      {T_LESS, T_LESS, T_LESS, T_LESS, T_LESS, T_GREATER, T_LESS, T_GREATER, T_LESS, T_LESS, T_LESS, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER},
      {T_LESS, T_LESS, T_LESS, T_LESS, T_LESS, T_GREATER, T_LESS, T_GREATER, T_LESS, T_LESS, T_LESS, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER},
      {T_LESS, T_LESS, T_LESS, T_LESS, T_LESS, T_GREATER, T_LESS, T_GREATER, T_LESS, T_LESS, T_LESS, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER},
      {T_LESS, T_LESS, T_LESS, T_LESS, T_LESS, T_GREATER, T_LESS, T_GREATER, T_LESS, T_LESS, T_LESS, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_GREATER}
    };


    int b;
    int a; // first symbol on stack is automatically $ --> 7;
    token * semicolon_on_top = ifj_generate_token(self->table, T_LBLOCK);
    token* t_less_on_top =  ifj_generate_token(self->table, T_LARRAY);
    token* E =  ifj_generate_token(self->table, 2222);

    token * top_stack;
    token * top_on_help_stack;
    token * active = lexa_next_token(self->lexa_module,self->table);

    token_stack *stack  = ifj_stack_new(); // new stack MAIN stack
    token_stack *help_stack = ifj_stack_new(); // helpfull stack using while POPing the MAIN

    ifj_stack_push(stack, semicolon_on_top);
    top_stack = ifj_stack_top(stack);



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

      switch (predict_array[a][b])
      {
        case T_EQUAL:
          ifj_stack_push(stack,active);
          top_stack = ifj_stack_top(stack);
          active = lexa_next_token(self->lexa_module,self->table);
          break;


        case  T_LESS:
          if (ifj_stack_top(stack)->type == 2222)
          {
            ifj_stack_pop(stack);
            ifj_stack_push(stack, t_less_on_top); // add T_LESS token on top stack
            top_stack = active; // newone top of stack
            ifj_stack_push(stack,E);
          }
          else
          {
          ifj_stack_push(stack, t_less_on_top); // add T_LESS token on top stack
          top_stack = active; // newone top of stack
          }

          ifj_stack_push(stack, top_stack); // add next token to the top of stack
          active = lexa_next_token(self->lexa_module,self->table); // take another token from scanner
          break;


        case  T_END:
          return -1;
          break;

        case T_GREATER:
          do // will fulling help_stack which one will using next
          {
              ifj_stack_push(help_stack, ifj_stack_top(stack));
              ifj_stack_pop(stack);
          } while(ifj_stack_top(stack)->type != t_less_on_top->type);

          ifj_stack_pop(stack); // POP  T_LESS form stack
          top_on_help_stack = ifj_stack_top(help_stack);

          switch (top_on_help_stack->type)
          {

            case T_LPAREN:
              ifj_stack_pop(help_stack); // POP  save new TOP and waiting E then POP sav new TOP waiting T_LPAREN and if it is OK control if stack is empty else return -1
              top_on_help_stack = ifj_stack_top(help_stack);
              if (top_on_help_stack->type == 2222) // E hash number
              {
                  ifj_stack_pop(help_stack);
                  top_on_help_stack = ifj_stack_top(help_stack);
                  if (top_on_help_stack->type == T_RPAREN)
                  {
                    ifj_stack_pop(help_stack);
                    if (ifj_stack_empty(help_stack) == true)
                    {
                      top_stack = ifj_stack_top(stack);
                      ifj_stack_push(stack, E); // push E on the top of main stack

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
              ifj_stack_pop(help_stack);
              if (ifj_stack_empty(help_stack) == true)
              {

                top_stack = ifj_stack_top(stack);
                ifj_stack_push(stack, E);
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
              ifj_stack_pop(help_stack);
              if (ifj_stack_empty(help_stack) == true)
              {
                top_stack = ifj_stack_top(stack);
                ifj_stack_push(stack, E);
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
              ifj_stack_pop(help_stack);
              if (ifj_stack_empty(help_stack) == true)
              {
                top_stack = ifj_stack_top(stack);
                ifj_stack_push(stack, E);
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
              ifj_stack_pop(help_stack);
              if (ifj_stack_empty(help_stack) == true)
              {
                top_stack = ifj_stack_top(stack);
                ifj_stack_push(stack, E);
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

            case 2222: //E value

              ifj_stack_pop(help_stack);
              top_on_help_stack = ifj_stack_top(help_stack);


              switch (top_on_help_stack->type)
              {
                case T_ADD:
                  ifj_stack_pop(help_stack);
                  top_on_help_stack = ifj_stack_top(help_stack);
                  if (top_on_help_stack == E)
                  {
                    ifj_stack_pop(help_stack);
                    if(ifj_stack_empty(help_stack) == true)
                    {
                      top_stack = ifj_stack_top(stack);
                      ifj_stack_push(stack, E);
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

                  ifj_stack_pop(help_stack);
                  top_on_help_stack = ifj_stack_top(help_stack);
                  if (top_on_help_stack == E)
                  {
                    ifj_stack_pop(help_stack);
                    if(ifj_stack_empty(help_stack) == true)
                    {
                      top_stack = ifj_stack_top(stack);
                      ifj_stack_push(stack, E);
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

                case T_DIVIDE:ifj_stack_pop(help_stack);
                  top_on_help_stack = ifj_stack_top(help_stack);
                  if (top_on_help_stack == E)
                  {
                    ifj_stack_pop(help_stack);
                    if(ifj_stack_empty(help_stack) == true)
                    {
                      top_stack = ifj_stack_top(stack);
                      ifj_stack_push(stack, E);
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
                  ifj_stack_pop(help_stack);
                  top_on_help_stack = ifj_stack_top(help_stack);
                  if (top_on_help_stack == E)
                  {
                    ifj_stack_pop(help_stack);
                    if(ifj_stack_empty(help_stack) == true)
                    {
                      top_stack = ifj_stack_top(stack);
                      ifj_stack_push(stack, E);
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
                  ifj_stack_pop(help_stack);
                  top_on_help_stack = ifj_stack_top(help_stack);
                  if (top_on_help_stack == E)
                  {
                    ifj_stack_pop(help_stack);
                    if (ifj_stack_empty(help_stack) == true)
                    {
                      top_stack = ifj_stack_top(stack);
                      ifj_stack_push(stack,E);
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
                  ifj_stack_pop(help_stack);
                  top_on_help_stack = ifj_stack_top(help_stack);
                  if (top_on_help_stack == E)
                  {
                    ifj_stack_pop(help_stack);
                    if (ifj_stack_empty(help_stack) == true)
                    {
                      top_stack = ifj_stack_top(stack);
                      ifj_stack_push(stack,E);
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
                  ifj_stack_pop(help_stack);
                  top_on_help_stack = ifj_stack_top(help_stack);
                  if (top_on_help_stack == E)
                  {
                    ifj_stack_pop(help_stack);
                    if (ifj_stack_empty(help_stack) == true)
                    {
                      top_stack = ifj_stack_top(stack);
                      ifj_stack_push(stack,E);
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
                      ifj_stack_pop(help_stack);
                      top_on_help_stack = ifj_stack_top(help_stack);
                      if (top_on_help_stack == E)
                      {
                        ifj_stack_pop(help_stack);
                        if (ifj_stack_empty(help_stack) == true)
                        {
                        top_stack = ifj_stack_top(stack);
                        ifj_stack_push(stack,E);
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
                      ifj_stack_pop(help_stack);
                      top_on_help_stack = ifj_stack_top(help_stack);
                      if (top_on_help_stack == E)
                      {
                        ifj_stack_pop(help_stack);
                        if (ifj_stack_empty(help_stack) == true)
                        {
                          top_stack = ifj_stack_top(stack);
                          ifj_stack_push(stack,E);
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
                      ifj_stack_pop(help_stack);
                      top_on_help_stack = ifj_stack_top(help_stack);
                      if (top_on_help_stack == E)
                      {
                        ifj_stack_pop(help_stack);
                        if (ifj_stack_empty(help_stack) == true)
                        {
                          top_stack = ifj_stack_top(stack);
                          ifj_stack_push(stack,E);
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

  int expresion(ifjInter *self)
    {
      int return_value = 0;

      if(self->debugMode)
      {
          fprintf(stderr, "som v expresion\n");
      }

      int predict_array [11][11] = {
        {T_GREATER, T_GREATER, T_LESS, T_LESS, T_LESS, T_GREATER, T_LESS, T_GREATER, T_LESS, T_LESS, T_LESS },
        {T_GREATER, T_GREATER, T_LESS, T_LESS, T_LESS, T_GREATER, T_LESS, T_GREATER, T_LESS, T_LESS, T_LESS },
        {T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_LESS, T_GREATER, T_LESS, T_GREATER, T_LESS, T_LESS, T_LESS },
        {T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_LESS, T_GREATER, T_LESS, T_GREATER, T_LESS, T_LESS, T_LESS },
        {T_LESS, T_LESS, T_LESS, T_LESS, T_LESS, T_EQUAL, T_LESS, T_END, T_LESS, T_LESS, T_LESS},
        {T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_END, T_GREATER, T_END, T_GREATER, T_END, T_END, T_END},
        {T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_COMMA, T_GREATER, T_END, T_GREATER, T_END, T_END, T_END},
        {T_LESS, T_LESS, T_LESS, T_LESS, T_LESS, T_END, T_LESS, T_END, T_LESS, T_LESS, T_LESS},
        {T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_END, T_GREATER, T_END, T_GREATER, T_END, T_END, T_END},
        {T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_END, T_GREATER, T_END, T_GREATER, T_END, T_END, T_END},
        {T_GREATER, T_GREATER, T_GREATER, T_GREATER, T_END, T_GREATER, T_END, T_GREATER, T_END, T_END, T_END}
      };

      int b;
      int a; // first symbol on stack is automatically $ --> 7;
      token * semicolon_on_top = ifj_generate_token(self->table, T_SEMICOLON); // tu  treba asi najprv zalozit token edo skontroluj ci dobre
      token* t_less_on_top =  ifj_generate_token(self->table, T_LESS); // tu  treba asi najprv zalozit token edo skontroluj ci dobre
      token* E =  ifj_generate_token(self->table, 2222); //  edo tu  treba asi najprv zalozit token edo skontroluj ci dobre vymyslel som si hodnotu snad je to ok

      token * top_stack;
      token * top_on_help_stack;
      token * active = lexa_next_token(self->lexa_module,self->table);

      token_stack *stack  = ifj_stack_new(); // new stack MAIN stack
      token_stack *help_stack = ifj_stack_new(); // helpfull stack using while POPing the MAIN

      ifj_stack_push(stack, semicolon_on_top);
      top_stack = ifj_stack_top(stack);


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

        switch (predict_array[a][b])
        {
          case T_EQUAL:
            ifj_stack_push(stack,active);
            top_stack = ifj_stack_top(stack);
            active = lexa_next_token(self->lexa_module,self->table);
            break;


          case  T_LESS:
            if (ifj_stack_top(stack)->type == 2222)
            {
              ifj_stack_pop(stack);
              ifj_stack_push(stack, t_less_on_top); // add T_LESS token on top stack
              top_stack = active; // newone top of stack
              ifj_stack_push(stack,E);
            }
            else
            {
            ifj_stack_push(stack, t_less_on_top); // add T_LESS token on top stack
            top_stack = active; // newone top of stack
            }

            ifj_stack_push(stack, top_stack); // add next token to the top of stack
            active = lexa_next_token(self->lexa_module,self->table); // take another token from scanner
            break;

          case  T_COMMA: // musim niekde ulozit token co je na vrchu zasobniku, to je ID funkcie a predat ho rekurzivnemu sestupu + nezabudnut pridat "("

            return function_parameters_for_exp(self);
            break;

          case  T_END:
            return -1;
            break;

          case T_GREATER:
            do // will fulling help_stack which one will using next
            {
                ifj_stack_push(help_stack, ifj_stack_top(stack));
                ifj_stack_pop(stack);

            } while(ifj_stack_top(stack)->type != t_less_on_top->type);

            ifj_stack_pop(stack); // POP  T_LESS form stack
            top_on_help_stack = ifj_stack_top(help_stack);

            switch (top_on_help_stack->type)
            {

              case T_LPAREN:
                ifj_stack_pop(help_stack); // POP  save new TOP and waiting E then POP sav new TOP waiting T_LPAREN and if it is OK control if stack is empty else return -1
                top_on_help_stack = ifj_stack_top(help_stack);
                if (top_on_help_stack->type == 2222) // E hash number
                {
                    ifj_stack_pop(help_stack);
                    top_on_help_stack = ifj_stack_top(help_stack);
                    if (top_on_help_stack->type == T_RPAREN)
                    {
                      ifj_stack_pop(help_stack);
                      if (ifj_stack_empty(help_stack) == true)
                      {
                        top_stack = ifj_stack_top(stack);
                        ifj_stack_push(stack, E); // push E on the top of main stack

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
                ifj_stack_pop(help_stack);
                if (ifj_stack_empty(help_stack) == true)
                {

                  top_stack = ifj_stack_top(stack);
                  ifj_stack_push(stack, E);
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
                ifj_stack_pop(help_stack);
                if (ifj_stack_empty(help_stack) == true)
                {
                  top_stack = ifj_stack_top(stack);
                  ifj_stack_push(stack, E);
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
                ifj_stack_pop(help_stack);
                if (ifj_stack_empty(help_stack) == true)
                {
                  top_stack = ifj_stack_top(stack);
                  ifj_stack_push(stack, E);
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
                ifj_stack_pop(help_stack);
                if (ifj_stack_empty(help_stack) == true)
                {
                  top_stack = ifj_stack_top(stack);
                  ifj_stack_push(stack, E);
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

              case 2222: //E value

                ifj_stack_pop(help_stack);
                top_on_help_stack = ifj_stack_top(help_stack);


                switch (top_on_help_stack->type)
                {
                  case T_ADD:
                    ifj_stack_pop(help_stack);
                    top_on_help_stack = ifj_stack_top(help_stack);
                    if (top_on_help_stack == E)
                    {
                      ifj_stack_pop(help_stack);
                      if(ifj_stack_empty(help_stack) == true)
                      {
                        top_stack = ifj_stack_top(stack);
                        ifj_stack_push(stack, E);
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

                    ifj_stack_pop(help_stack);
                    top_on_help_stack = ifj_stack_top(help_stack);
                    if (top_on_help_stack == E)
                    {
                      ifj_stack_pop(help_stack);
                      if(ifj_stack_empty(help_stack) == true)
                      {
                        top_stack = ifj_stack_top(stack);
                        ifj_stack_push(stack, E);
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

                  case T_DIVIDE:ifj_stack_pop(help_stack);
                    top_on_help_stack = ifj_stack_top(help_stack);
                    if (top_on_help_stack == E)
                    {
                      ifj_stack_pop(help_stack);
                      if(ifj_stack_empty(help_stack) == true)
                      {
                        top_stack = ifj_stack_top(stack);
                        ifj_stack_push(stack, E);
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
                    ifj_stack_pop(help_stack);
                    top_on_help_stack = ifj_stack_top(help_stack);
                    if (top_on_help_stack == E)
                    {
                      ifj_stack_pop(help_stack);
                      if(ifj_stack_empty(help_stack) == true)
                      {
                        top_stack = ifj_stack_top(stack);
                        ifj_stack_push(stack, E);
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
