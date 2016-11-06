/* ifj-syna.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors:
 */

 #include "ifj-syna.h"
 #include "ifj-inter.h"

 //rules
 //terminals
 //nonterminals

 /**
  * Run syntactic analysis
  * @returns 0 when successful
  */
  int start(ifjInter *self)
  {
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if ((active ) ==  T_CLASS) && () == T_IDENTIFIER))
    {
      return_value = (is_ID() && class_inside() && next_class());
      //take 2. token , this mean ID and save it to the class table like class ID
      // make table for new class

      /*symbol_table * class_table = ial_symbol_table_new();
      class_table->parent = self->table;
      token->child = class_table;*/
    }
    else
    {
      return_value = 0;
    }
    return  return_value;

  }

  int next_class(ifjInter *self)
  {
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if ((active) == T_END)
    {
      return_value =  1;
    }
    else if (active == T_CLASS && is_ID())
         {
            return_value = (class_inside() && next());
            //take 2. token , this mean ID and save it to the class table like class ID
            // make table for new class
         }
         else
         {
           return_value = 0;
         }
    return return_value;

  }


  int class_inside(ifjInter *self)
  {
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active == T_LBLOCK)
    {
      return_value = class_inside1();
    }
    else
    {
      return_value = 0;
    }
    return return_value;
  }


  int class_inside1(ifjInter *self)
  {
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active == T_RBLOCK)
    {
      return_value == 1;
    }
    else
    {
      if (active == T_STATIC)
      {
        return_value = (tell_me_type_with_void() && is_ID() && class_inside2())
      }
      else
      {
        return_value = 0;
      }
    }
    return return_value;
  }


  int is_ID(ifjInter *self)
  {
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active == T_IDENTIFIER)
    {
      return_value = 1;
    }
    else
    {
      return_value = 0;
    }
    return return_value;
  }


  int class_inside2(ifjInter *self)
  {
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active == T_LPAREN)
    {
      return_value = (function_declar() && function_inside() && class_inside1());
    }
    else if (active == T_SEMICOLON)
         {
           return_value = class_inside1();
         }
         else if (active == T_COMMA)
              {
                return_value = id_declar() && class_inside1();
              }
              else
              {
                return_value = 0;
              }
    return return_value;
  }


  int tell_me_type_with_void(ifjInter *self)
  {
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    switch(active)
    {
      case T_INTEGER:
        return_value = 1;
        break;
      case T_DOUBLE:
        return_value = 1;
        break;
      case T_STRING:
        return_value = 1;
        break;
      case T_VOID:
        return_value = 1;
        break;
      default:
        return_value = 0;
        break;
    }
    return return_value;
  }


  int tell_me_type_without_void(ifjInter *self)
  {
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    switch(active)
    {
      case T_INTEGER:
        return_value = 1;
        break;
      case T_DOUBLE:
        return_value = 1;
        break;
      case T_STRING:
        return_value = 1;
        break;
      default:
        return_value = 0;
        break;
    }
    return return_value;
  }

  int id_declar(ifjInter *self)
  {
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active == T_IDENTIFIER)
    {
      return_value = id_declar1();
    }
    else
    {
      return_value = 0;
    }
    return return_value;
  }


  int id_declar1(ifjInter *self)
  {
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active == T_SEMICOLON)
    {
      return_value = 1;
    }
    else if (active == T_COMMA)
         {
           return_value = id_declar();
         }
         else
         {
           return_value = 0;
         }
    return return_value;
  }


  int function_declar(ifjInter *self)
  {
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    switch(active)
    {
      case T_INTEGER:

      case T_DOUBLE:

      case T_STRING:
        return_value = (is_ID() && next_function_param());
        break;
      case T_RPAREN:
        return_value = 1;
        break;
      default:
        return_value = 0;
        break;
    }
    return return_value;
  }


  int next_function_param(ifjInter * self)
  {
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active == T_RPAREN)
    {
      return_value = 1;
    }
    else if (active == T_COMMA)
         {
           return_value = (tell_me_type_without_void() && is_ID() && next_function_param());
         }
         else
         {
           return_value = 0;
         }
  }


  int function_inside(ifjInter *self)
  {
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active == T_LBLOCK)
    {
      return_value = function_inside1();
    }
    else
    {
      return_value = 0;
    }
    return return_value;
  }


  int function_inside1(ifjInter *self)
  {
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    switch (active)
    {
      case T_RBLOCK:
        return_value = 1;
        break;
      case T_WHILE:
        return_value = (is_LPAREN() && condition() && is_RPAREN() && statement_inside() && function_inside1());
        break;
      case T_FOR:
        return_value = (is_LPAREN() tell_me_type_without_void() && is_ID() && is_ASSIGN() && expresion() && is_semicolon() && condition() && is_semicolon() && is_ID() && is_ASSIGN() && expresion()&& is_RPAREN() && statement_inside() && function_inside1());
        break;
      case T_DO:
        return_value = (statement_inside() is_while() && is_LPAREN() && condition() && is_RPAREN() is_semicolon() && function_inside1());
        break;
      case T_BREAK:
        return_value = (is_semicolon() && function_inside1());
        break;
      case T_CONTINUE:
        return_value = (is_semicolon() && function_inside1());
        break;
      case T_IF:
        return_value = (is_LPAREN() && condition() && is_RPAREN() && statement_inside() && if_else1() && function_inside1());
        break;
      case T_RETURN:
        return_value = (expresion() && is_semicolon() && statement_inside1());
        break;
      case T_INTEGER:
      case T_STRING:
      case T_DOUBLE:
        return_value = (is_ID() && sth_next() && function_inside1());
        break;
      case T_IDENTIFIER:
        return_value = (fce() && function_inside1());
        break;
      default:
        return_value = 0;
        break;
      }
    return return_value;
  }


  int is_semicolon(ifjInter *self)
  {
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active == T_SEMICOLON)
    {
      return_value = 1;
    }
    else
    {
      return_value = 0;
    }
    return return_value;
  }


  int is_while(ifjInter *self)
  {
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active == T_WHILE)
    {
      return_value = 1;
    }
    else
    {
      return_value = 0;
    }
    return return_value;
  }


  int is_LPAREN(ifjInter *self)
  {
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active == T_LPAREN)
    {
      return_value = 1;
    }
    else
    {
      return_value = 0;
    }
    return return_value;
  }


  int is_RPAREN(ifjInter *self)
  {
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active == T_RPAREN)
    {
      return_value = 1;
    }
    else
    {
      return_value = 0;
    }
    return return_value;
  }



  int is_ASSIGN(ifjInter *self)
  {
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active == T_ASSIGN)
    {
      return_value = 1;
    }
    else
    {
      return_value = 0;
    }
    return return_value;
  }


  int if_else1(ifjInter *self)
  {
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active == T_ELSE)
    {
      return_value = statement_inside();
    }
    // tu nastava problem   treba dat else akurat co moyze byt v tokene  ak tam nebude else?

  }

 int statement_inside(ifjInter *self)
 {
   int return_value = 0;
   token * active = lexa_next_token(self->lexa_module,self->table);
   if (active == T_LBLOCK)
   {
     return_value = statement_inside1();
   }
   else
   {
     return_value = 0;
   }
   return return_value;

 }


 int statement_inside1(ifjInter *self])
 {
   int return_value = 0;
   token * active = lexa_next_token(self->lexa_module,self->table);
   switch (active)
   {
     case T_RBLOCK:
       return_value = 1;
       break;
     case T_WHILE:
       return_value = (is_LPAREN() && condition() && is_RPAREN() && statement_inside() && statement_inside1());
       break;
     case T_FOR:
       return_value = (is_LPAREN() tell_me_type_without_void() && is_ID() && is_ASSIGN() && expresion() && is_semicolon() && condition() && is_semicolon() && is_ID() && is_ASSIGN() && expresion()&& is_RPAREN() && statement_inside() && statement_inside1());
       break;
     case T_DO:
       return_value = (statement_inside() is_while() && is_LPAREN() && condition() && is_RPAREN() is_semicolon() && statement_inside1());
       break;
     case T_BREAK:
       return_value = (is_semicolon() && statement_inside1());
       break;
     case T_CONTINUE:
       return_value = (is_semicolon() && statement_inside1());
       break;
     case T_IF:
       return_value = (is_LPAREN() && condition() && is_RPAREN() && statement_inside() && if_else1() && statement_inside1());
       break;
     case T_RETURN:
       return_value = (expresion() && is_semicolon() && statement_inside1());
       break;
     case T_IDENTIFIER:
       return_value = (fce() && statement_inside1());
       break;
     default:
       return_value = 0;
       break;
     }
   return return_value;
 }


 int condition(ifjInter *self)
 {
   int return_value = 0;
   return_value = (expresion() && rel_operator() && expresion());
   return return_value;

 }


 int fce(ifjInter *self)
 {
   int return_value = 0;
   token * active = lexa_next_token(self->lexa_module,self->table);
   if (active == T_LPAREN)
   {
     return_value = (function_parameters() && is_semicolon());
   }
   else if (active == T_ASSIGN)
        {
          return_value = (expresion() && is_semicolon();
        }
        else if (active == T_DOT)
             {
               return_value =  (is_ID() && method_call());
             }
             else
             {
                return_value = 0;
             }
   return return_value;

 }


 int method_call(ifjInter *self)
 {
   int return_value = 0;
   token * active = lexa_next_token(self->lexa_module,self->table);
   if (active == T_LPAREN)
   {
     return_value = (function_parameters()  && is_semicolon());
   }
   else if (active == T_ASSIGN)
        {
          return_value = (expresion() && is_semicolon();
        }
        else
        {
          return_value = 0;
        }
   return return_value;

 }


int function_parameters(ifjInter *self)
{
  int return_value = 0;
  token * active = lexa_next_token(self->lexa_module,self->table);
  if (active == T_RPAREN)
  {
    return_value = 1;
  }
  else if (active = T_IDENTIFIER)
       {
         return_value = next_function_parameters();
       }
       else
       {
         return_value = 0;
       }
  return return_value;
}


int next_function_parameters(ifjInter *self)
{
  int return_value = 0;
  token * active = lexa_next_token(self->lexa_module,self->table);
  if (active == T_RPAREN)
  {
    return_value = 1;
  }
  else if (active = T_COMMA)
       {
         return_value = (is_ID() && next_function_parameters());
       }
       else
       {
         return_value = 0;
       }
  return return_value;
}




 int sth_next(ifjInter self*)
 {
   int return_value = 0;
   token * active = lexa_next_token(self->lexa_module,self->table);
   if (active == T_SEMICOLON)
   {
     return_value = 1;
   }
   else if (active == T_ASSIGN)
        {
          return_value = ( expresion() && is_semicolon());
        }
        else
        {
          return_value = 0;
        }
   return = return_value;

 }


 int rel_operator(ifjInter *self)
 {
   int return_value = 0;
   token * active = lexa_next_token(self->lexa_module,self->table);
   switch (active)
   {
     case T_LESS:
     case T_GREATER:
     case T_LESS_EQUAL:
     case T_GREATER_EQUAL:
     case T_EQUAL:
     case T_NOT_EQUAL:
      return_value = 1;
      break;
    default:
      return_value = 0;
   }
   return return_value;
 }



 int syna_run( ifjInter *self)
 {
     token * active;
     symbol_table * context = self->table; //set context to global table

     do
     {
         active = lexa_next_token(self->lexa_module, context);
         switch(active->type)
         {
            case T_IDENTIFIER:
                printf("id: %s\n", (char *)active->value);
                break;
            case T_INTEGER_C:
                printf("int: %d\n", *((int *)active->value));
                break;
            case T_DOUBLE_C:
                printf("double: %f\n", *((double *)active->value));
                break;
            case T_STRING_C:
                printf("string: %s\n", (char *)active->value);
                break;
            default:
                printf("symbol: %d\n", active->type);
                break;
         }
     }
     while(active && active->type != T_END);

     return 0;
 }
