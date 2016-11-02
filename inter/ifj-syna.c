/* ifj-syna.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors:
 */

 #include "ifj-syna.h"
 #include "ifj-inter.h"
 #include <stdbool.h>

 //rules
 //terminals
 //nonterminals

 /**
  * Run syntactic analysis
  * @returns 0 when successful
  */
  bool start(ifjInter *self)
  {
    bool return_value = false;
    token * active;
    token * next;
    if ((active = lexa_next_token(self->lexa_module,self->table) ==  T_CLASS) && (lexa_next_token(self->lexa_module,self->table) == T_IDENTIFIER))
    {
      return_value = class_inside() && next();
      symbol_table * class_table = ial_symbol_table_new();
      class_table->parent = self->table;
      token->child = class_table;
    }
  }
  bool class_inside(ifjInter *self, token )


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
     // janny skusa pridat nieco noveho
 }
