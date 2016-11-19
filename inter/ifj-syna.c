/* ifj-syna.c
*
* Copyright (C) 2016 SsYoloSwag41 Inc.
* Authors: Jan Demcak <xdemca01@stud.fit.vutbr.cz>
*/

#include "ifj-syna.h"
#include "ifj-inter.h"

//rules
//terminals
//nonterminals

/**
* Run syntactic analysis
* @return 1 when successful
*/

/*TODO JANY funcia run je bezparametricka staticka   */
/*TODO JANY ak je void funkcia tak  nesmie obsahovat return*/
/*TODO JANNY Každá funkce vrací hodnotu danou vyhodnocením výrazu v příkazu return . V pří-
padě chybějící návratové hodnoty kvůli neprovedení příkazu return dojde k chybě 8.*/
/* TODO JANNY Nedošlo-li k vykonání žádného příkazu return a nejedná se o void -funkci, nastává
běhová chyba 8. */
/*TODO JANY Pokus o vytvoření uživa-
telské třídy ifj16 je chyba 3*/

/* TODO JANY Pokus o přiřazení návratové hodnoty z void -funkce vede na chybu 8. */
/*TODO JANNY Pokud je vyhodnocený výraz pravdivý,
vykoná se složený_příkaz 1 , jinak se vykoná složený_příkaz 2 . Pokud výsledná hod-
nota výrazu není pravdivostní (tj. pravda či nepravda), nastává chyba 4. */
/* TODO JANY V případě, že příkaz volání funkce obsahuje jiný počet nebo typy
parametrů, než funkce očekává (tedy než je uvedeno v její hlavičce, a to i u vesta-
věných funkcí) včetně případné aplikace implicitních konverzí, jedná se o chybu 4.*/
/*TODO JANY  opravit navratove kody
1 - chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexé-
mu).
• 2 - chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu).
• 3 - sémantická chyba v programu – nedefinovaná třída/funkce/proměnná, pokus o re-
definici třídy/funkce/proměnné, atd. ak chyba funkcia RUN
• 4 - sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních
výrazech, příp. špatný počet či typ parametrů u volání funkce.
• 6 - ostatní sémantické chyby.
• 7 - běhová chyba při načítání číselné hodnoty ze vstupu.
• 8 - běhová chyba při práci s neinicializovanou proměnnou.
• 9 - běhová chyba dělení nulou.
• 10 - ostatní běhové chyby.
• 99 - interní chyba interpretu tj. neovlivněná vstupním programem (např. chyba a-
lokace paměti, chyba při otvírání souboru s řídicím programem, špatné parametry
příkazové řádky atd.).*/

/* TODO EDO  na zaciatku by mala byt vytvorena niejaka tabulka  globalna kde bude vsetko
+ 1 tabulka pre classu Main sa urobi hned na zaciatku este pred pustenim  analyz*/
/*TODO EDO kazda funkcia obsahuje premennu active kde je ulozeny akutualny  token
2. parameter je vzdy self->table  nastaveny, len mi nieje jasne ako to urobit aby sa napriklad pri kazdom volani
funkcie ukladalo do inej tabulky kedze stale sa bude volat ten isty kod, dufam ze tebe je :D*/
/* TODO EDO  pri kazdom volani tokenu najdes popis o co sa jedna a  kde sa nachadza*/

int start(ifjInter *self)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active->type ==  T_CLASS)
    {
        /*
            TODO XXX JANY myslím, že by sme tu mali skočiť rovno do next_class
        */
        /* TODO EDO  uplne prve slovo  v zdrojaku cize  token class k prvej classe Main*/
        return_value = (is_ID(self) &&
        /*TODO  JANY tu treba overit ci to is_ID je Main, cize bud  upravit funkciu
        a spravit v nej 2 volania lexa_next_token,
        alebo si urobit static premennu counter ktora to skontroluje ked sa
        1.x zavola is_ID --> prve ID musi byt main

        XXX tu to podla mna nemusime riešiť, to či je metóda Run v classe Main
        definovaná si vieme jednoducho overiť po behu syntaktickej a rovno
        aj nastaviť štartovací bod pre exekúT_FOR
        */
                        class_inside(self) &&
                        next_class(self));
    }
    else
    {
        return_value = 0;
    }
    return  return_value;

}

/**
 * Next class or EOF
 * @param self global structure
 * @return 1 if EOF or some proper class definition, 0 when some garbage found
 **/
int next_class(ifjInter *self)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module, self->table);
    if ((active->type) == T_END)
    {
        return_value =  0;
    }
    else if (active->type == T_CLASS && !is_ID(self, self->table, &active))
    {
        active->childTable = ial_symbol_table_new();
        active->parent = self->table;
        return_value = class_inside(self, active->childTable) &&
                       next_class(self);
    }
    else
    {
        print_unexpected(active);
        return_value = 2;
    }
    return return_value;

}

/**
 * New class beginning with {
 * @param self global structure
 * @param table symbol table for current class
 * @return 0 is successfull
 **/
int class_inside(ifjInter *self, symbolTable *table)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module, self->table);
    if (active->type == T_LBLOCK)
    {
        return_value = class_inside1(self);
    }
    else
    {
        print_unexpected(active);
        return_value = 2;
    }

    return return_value;
}

/**
 * Inside class, static function, static variable or } expected
 * @param self global structure
 * @param table symbol table for current class
 * @return 0 if successfull
 **/
int class_inside1(ifjInter *self, symbolTable *table)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module, self->table);
    if (active->type != T_RBLOCK)
    {
        if (active->type == T_STATIC)
        {
            return_value = get_type_with_void(self, &active) &&
                           is_ID(self, &active) &&
                           class_inside2(self, table, active);
        }
        else
        {
            print_unexpected(active);
            return_value = 2;
        }
    }
    return return_value;
}

/**
 * Fetch next token, assert idenfifier type, set datatype
 * @param self global structure
 * @param item reference to last token, reference to new token returned
 * @return 0 if successfull
 **/
int is_ID(ifjInter *self, symbolTable *table, token **item)
{
    int return_value = 0;
    token *active = lexa_next_token(self->lexa_module, table);
    token *prev = *item;
    if (active->type == T_IDENTIFIER)
    {
        switch (prev->type)
        {
            //declarations
            case T_CLASS:
                return_value = resolve_identifier(self, table, &active, 1);
                break;
            case T_INTEGER:
                active->dataType = T_INTEGER;
                return_value = resolve_identifier(self, table, &active, 1);
                break;
            case T_DOUBLE:
                active->dataType = T_DOUBLE;
                return_value = resolve_identifier(self, table, &active, 1);
                break;
            case T_STRING:
                active->dataType = T_STRING;
                return_value = resolve_identifier(self, table, &active, 1);
                break;
            case T_VOID:
                active->dataType = T_VOID;
                return_value = resolve_identifier(self, table, &active, 1);
                break;
            //not a declaration
            default:
                return_value = resolve_identifier(self, table, &active, 0);
                break;
        }
    }
    else
    {
        print_unexpected(active);
        return_value = 4;
    }
    *item = active;
    self->return_code = return_value;
    return return_value;
}

int is_ID_number(ifjInter *self)
{
  int return_value = 0;
  token * active = lexa_next_token(self->lexa_module,self->table);
  /* TODO EDO  pouziva sa iba pri volani funkcie,
  kontroluje ci je v argumentoch iba to co tam moze byt (syntakticky nie sematicky)
  , mozes dorobit aj sematicku kontrolu ci sedia typy tak ako je v deklaracii,
  funkcie s tym suvisiace next_function_parameters, function_parameters,
  a  function_parameters_for_exp, next_function_parameters_for_exp
  presne to iste len to 2.  komunikuje  s exp.c, pre teba by to nemalo znamenat rozdiel*/
  if (active->type == T_IDENTIFIER ||
      active->type == T_INTEGER_C ||
      active->type == T_DOUBLE_C ||
      active->type == T_STRING_C)

  {
       return_value = 1;
  }
  else
  {
       return_value = 0;
  }
  return return_value;
}

/**
 * Fetch next token. Now we are in state "type identifier". We expect "(" for
 * function declaration, ";" or "=" for variable declaration
 * @param self global structure
 * @param table symbol table for current class
 * @param item last token / identifier (TODO solve this for each T_IDENTIFIER)
 * @return 0 if successfull
 **/
int class_inside2(ifjInter *self, symbolTable *table, token *item)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module, self->table);
    //function
    if (active->type == T_LPAREN)
    {
        item->childTable = ial_symbol_table_new();
        item->parent = table;
        return_value = !function_declar(self, item) &&
                       !function_inside(self, item) &&
                       !class_inside1(self, table);
    }
    //variable
    else if (active->type == T_SEMICOLON)
    {
        //check if not void here
        return_value = class_inside1(self, table);
    }
    else if (active->type == T_ASSIGN)
    {
        return_value = !expresion(self) && !class_inside1(self, table);
    }
    //some garbage
    else
    {
        print_unexpected(active);
        return_value = 2;
    }
    return return_value;
}

/**
 * Fetch next token. Expects type definition (int, double, String, void)
 * @param self global structure
 * @param item returns reference to fetched token
 * @return 0 if successfull
 **/
int get_type_with_void(ifjInter *self, token **item)
{
    int return_value = 2;
    token * active = lexa_next_token(self->lexa_module, self->table);
    switch(active->type)
    {
        case T_INTEGER:
        case T_DOUBLE:
        case T_STRING:
        case T_VOID:
            return_value = 0;
            break;
    }
    if(return_value)
    {
        print_unexpected(active);
    }
    *item = active;
    return return_value;
}

/**
 * Fetch next token. Expects type definition (int, double, String)
 * @param self global structure
 * @param item returns reference to fetched token
 * @return 0 if successfull
 **/
int get_type_without_void(ifjInter *self, token **item)
{
    int return_value = 2;
    token * active = lexa_next_token(self->lexa_module, self->table);
    switch(active->type)
    {
        case T_INTEGER:
        case T_DOUBLE:
        case T_STRING:
            return_value = 0;
            break;
    }
    if(return_value)
    {
        print_unexpected(active);
    }
    *item = active;
    return return_value;
}

/**
 * Fetch first argument for function declaration ("type identifier" or ")")
 * - save arguments in child table
 * - set args stack
 * @param self global structure
 * @param item function token
 * @return 0 if successfull
 **/
int function_declar(ifjInter *self, token *item)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module, item->childTable);
    switch(active->type)
    {
        case T_INTEGER:
        case T_DOUBLE:
        case T_STRING:
            if(!is_ID(self, item->childTable, &active))
            {
                item->args = ifj_stack_new();
                ifj_stack_push(item->args, active);
                return_value = next_function_param(self, item);
            }
            else
            {
                print_unexpected(active);
                return_value = 2;
            }
            break;
        case T_RPAREN:
            return_value = 0;
            break;
        default:
            return_value = 2;
            print_unexpected(active);
            break;
    }
    return return_value;
}

/**
 * Fetch n-th argument for function declaration (")", "type identifier" or ",")
 * - save arguments in child table
 * - set args stack
 * @param self global structure
 * @param item function token
 * @return 0 if successfull
 **/
int next_function_param(ifjInter *self, token *item)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module, self->table);
    if (active->type == T_COMMA)
    {
        if(!get_type_without_void(self, &active) &&
           !is_ID(self, item->childTable, &active))
        {
            ifj_stack_push(item->args, active);
            return_value = next_function_param(self, item);
        }
        else
        {
            print_unexpected(active);
            return_value = 2;
        }

    }
    else if(active->type != T_RPAREN)
    {
        return_value = 2;
        print_unexpected(active);
    }
    return return_value;
}

/**
 * Fetch next token. Expects "{".
 * @param self global structure
 * @param current function token
 * @return 0 if successfull
 **/
int function_inside(ifjInter *self, token *item)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module, self->table);
    if (active->type == T_LBLOCK)
    {
        return_value = function_inside1(self, item);
    }
    else
    {
        print_unexpected(active);
        return_value = 2;
    }
    return return_value;
}

/**
 * Inside function. Expects "}", "while", "break", "continue", "if", "return",
 * "int", "double", "String", "identifier"
 * @param self global structure
 * @param item current function token
 * @return 0 if successfull
 **/
int function_inside1(ifjInter *self, token *item)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module, self->table);
    switch (active->type)
    {
        case T_RBLOCK:
            return_value = 0;
            break;
        case T_WHILE:
            return_value = (
                condition(self, item->childTable) && //TODO XXX NOTE FIXME TU SOM
                statement_inside(self) &&
                function_inside1(self)
            );
            break;
      /*  case T_FOR:
        return_value = (is_LPAREN(self) &&
                        tell_me_type_without_void(self) &&
                        is_ID(self) &&
                        is_ASSIGN(self) &&
                        expresion(self) &&
                        is_semicolon(self) &&
                        condition(self) &&
                        is_semicolon(self) &&
                        is_ID(self) &&
                        is_ASSIGN(self) &&
                        expresion(self)&&
                        is_RPAREN(self) &&
                        statement_inside(self) &&
                        function_inside1(self));
        break;  */
      /*  case T_DO:
        return_value = (statement_inside(self) &&
                        is_while(self) &&
                        is_LPAREN(self) &&
                        condition(self) &&
                        is_RPAREN(self) &&
                        is_semicolon(self) &&
                        function_inside1(self));
        break;*/
        case T_BREAK:
            return_value = (is_semicolon(self) && function_inside1(self));
            break;
        case T_CONTINUE:
            return_value = (is_semicolon(self) && function_inside1(self));
            break;
        case T_IF:
            return_value = (
                condition(self) &&
                statement_inside(self) &&
                if_else1(self) &&
                function_inside1(self)
            );
            break;
        case T_RETURN:
            return_value = (expresion(self)  && statement_inside1(self));
            break;
        case T_INTEGER:
        case T_STRING:
        case T_DOUBLE:
            return_value = (is_ID(self) && sth_next(self) && function_inside1(self));
            break;
        case T_IDENTIFIER:
            return_value = (fce(self) && function_inside1(self));
            break;
        default:
            return_value = 2;
            print_unexpected(active);
            break;
    }
    return return_value;
}


int is_semicolon(ifjInter *self)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    /* TODO EDO   vyuzivane vyhradne vo vnutri funkcie*/
    if (active->type == T_SEMICOLON)
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
    /* TODO EDO   vyuzivane vyhradne vo vnutri funkcie*/
    if (active->type == T_WHILE)
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
    /* TODO EDO   vyuzivane vyhradne vo vnutri funkcie*/
    if (active->type == T_LPAREN)
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
    /* TODO EDO   vyuzivane vyhradne vo vnutri funkcie*/
    if (active->type == T_RPAREN)
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
    /* TODO EDO   vyuzivane vyhradne vo vnutri funkcie, resp, momentalne
    nevyuzivane vobec pouzivam pri DO a FOR ktore zatial niesu plne funkcne*/
    if (active->type == T_ASSIGN)
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
    /* TODO EDO   vyuzivane vyhradne vo vnutri funkcie*/
    if (active->type == T_ELSE)
    {
        return_value = is_LBLOCK(self) && statement_inside(self);
    }
    else
    {
      return_value = -1;
    }
    // tu nastava problem treba dat else akurat co moze byt v tokene  ak tam nebude else?
    return return_value;
}

int is_LBLOCK(ifjInter *self)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    /* TODO EDO   vyuzivane vyhradne vo vnutri funkcie*/
    if (active->type == T_LBLOCK)
    {
        return_value = 1;
    }
    else
    {
        return_value = 0;
    }
    return return_value;
}

int statement_inside(ifjInter *self)
{
    /*int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active->type == T_LBLOCK)
    {
        return_value = statement_inside1(self);
    }
    else
    {
        return_value = 0;
    }
    return return_value;*/
    /* TODO JANY   uprav to je to zbytocne*/
    return statement_inside1(self);
}

int statement_inside1(ifjInter *self)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    /* TODO EDO   vyuzivane vyhradne vo vnutri funkcie*/
    switch (active->type)
    {
        case T_RBLOCK:
          return_value = 1;
          break;

        case T_WHILE:
        return_value = (  condition(self) /*&& statement_inside(self) */&& statement_inside1(self)  && statement_inside1(self));
        break;
      /*  case T_FOR:
        return_value = (is_LPAREN(self) &&
                        tell_me_type_without_void(self) &&
                        is_ID(self) &&
                        is_ASSIGN(self) &&
                        expresion(self) &&
                        is_semicolon(self) &&
                        condition(self) &&
                        is_semicolon(self) &&
                        is_ID(self) &&
                        is_ASSIGN(self) &&
                        expresion(self) &&
                        is_RPAREN(self) &&
                        statement_inside(self) &&
                        statement_inside1(self));
        break;
        case T_DO:
        return_value = (statement_inside(self) &&
                        is_while(self) &&
                        is_LPAREN(self) &&
                        condition(self) &&
                        is_RPAREN(self) &&
                        is_semicolon(self) &&
                        statement_inside1(self));
        break; */
        case T_BREAK:
        return_value = (is_semicolon(self) && statement_inside1(self));
        break;
        case T_CONTINUE:
        return_value = (is_semicolon(self) && statement_inside1(self));
        break;
        case T_IF:
        return_value = ( condition(self) && statement_inside(self) && if_else1(self) && statement_inside1(self));
        break;
        case T_RETURN:
        return_value = (expresion(self) &&  statement_inside1(self));
        break;
        case T_IDENTIFIER:
        return_value = (fce(self) && statement_inside1(self));
        break;
        default:
        return_value = 0;
        break;
    }
    return return_value;
}





int fce(ifjInter *self)
{
  /* TODO EDO   vyuzivane vyhradne vo vnutri funkcie*/
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active->type == T_LPAREN)
    {
        return_value = (function_parameters(self) && is_semicolon(self));
    }
    else if (active->type == T_ASSIGN)
    {
        return_value = (expresion(self) );
    }
    else if (active->type == T_DOT)
    {
        return_value =  (is_ID(self) && method_call(self));
    }
    else
    {
        return_value = 0;
    }
    return return_value;

}


int method_call(ifjInter *self)
{
  /* TODO EDO   vyuzivane vyhradne vo vnutri funkcie*/
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active->type == T_LPAREN)
    {
        return_value = (function_parameters(self)  && is_semicolon(self));
    }
    else if (active->type == T_ASSIGN)
    {
        return_value = (expresion(self) );
    }
    else
    {
        return_value = 0;
    }
    return return_value;

}


int function_parameters(ifjInter *self)
{
  /* TODO EDO   vyuzivane vyhradne vo vnutri funkcie*/
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active->type == T_RPAREN)
    {
        return_value = 1;
    }
    else if (active->type == T_IDENTIFIER ||
             active->type == T_INTEGER_C ||
             active->type == T_DOUBLE_C ||
             active->type == T_STRING_C)
    {
        return_value = next_function_parameters(self);
    }
    else
    {
        return_value = 0;
    }
    return return_value;
}


int next_function_parameters(ifjInter *self)
{
  /* TODO EDO   vyuzivane vyhradne vo vnutri funkcie*/
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active->type == T_RPAREN)
    {
        return_value = 1;
    }
    else if (active->type == T_COMMA)
    {
        return_value = (is_ID_number(self) && next_function_parameters(self));
    }
    else
    {
        return_value = 0;
    }
    return return_value;
}




int sth_next(ifjInter *self)
{
  /* TODO EDO   vyuzivane vyhradne vo vnutri funkcie*/
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active->type == T_SEMICOLON)
    {
        return_value = 1;
    }
    else if (active->type == T_ASSIGN)
    {
        return_value = ( expresion(self) );
    }
    else
    {
        return_value = 0;
    }
    return return_value;

}


int rel_operator(ifjInter *self)
{
  /* TODO EDO   vyuzivane vyhradne vo vnutri funkcie*/
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    switch (active->type)
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
    int return_value = start(self);

    if(self->debugMode)
    {
        fprintf(stderr, "Syna result: %d\n", return_value);
    }

    return return_value;
}

int function_parameters_for_exp(ifjInter *self)
{
  /* TODO EDO   vyuzivane vyhradne vo vnutri funkcie*/
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active->type == T_RPAREN)
    {
        return_value = is_semicolon(self);
    }
    else if (active->type == T_IDENTIFIER ||
             active->type == T_INTEGER_C ||
             active->type == T_DOUBLE_C ||
             active->type == T_STRING_C)
    {
        return_value = next_function_parameters_for_exp(self);
    }
    else
    {
        return_value = 0;
    }
    return return_value;
}

int next_function_parameters_for_exp(ifjInter *self)
{
  /* TODO EDO   vyuzivane vyhradne vo vnutri funkcie*/
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active->type == T_RPAREN)
    {
        return_value = is_semicolon(self);
    }
    else if (active->type == T_COMMA)
    {
        return_value = (is_ID_number(self) && next_function_parameters_for_exp(self));
    }
    else
    {
        return_value = 0;
    }
    return return_value;
}
