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
