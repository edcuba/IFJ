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
                           is_ID(self, table, &active) &&
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
    self->returnCode = return_value;
    return return_value;
}

/**
 * Resolving args for called functions. Fetch next token, expects "arg" or ")"
 * @param self global structure
 * @param table symbol table for current function (not called one!)
 * @param expected prototype of expected argument for type check
 * @return 0 if successfull
 **/
int next_param(ifjInter *self, symbolTable *table, token *expected)
{
    token * active = lexa_next_token(self->lexa_module, table);
    if (active->type == T_IDENTIFIER)
    {
        int rc = resolve_identifier(self, table, &active, 0);
        if(rc)
        {
            self->returnCode = rc;
            return rc;
        }
        if(active->dataType == expected->dataType)
        {
            return 0;
        }
    }
    else if(active->type == T_INTEGER_C ||
            active->type == T_DOUBLE_C ||
            active->type == T_STRING_C)
    {
        if(active->dataType == expected->dataType)
        {
            return 0;
        }
    }
    print_unexpected(active);
    return 4;
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
 * FIXME: I dont think, that we can accept "break" or "continue" here
 * @param self global structure
 * @param item current function token
 * @return 0 if successfull
 **/
int function_inside1(ifjInter *self, token *item)
{
    int return_value = 0;
    token * active = NULL;
    if(self->pushBack)
    {
        active = self->pushBack;
        self->pushBack = NULL;
    }
    else
    {
        active = lexa_next_token(self->lexa_module, item->childTable);
    }
    switch (active->type)
    {
        case T_RBLOCK:
            return_value = 0;
            break;
        case T_WHILE:
            return_value = (
                !condition(self, item->childTable) &&
                !statement_inside1(self, item->childTable) &&
                !function_inside1(self, item)
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
            return_value = !is_semicolon(self) &&
                           !function_inside1(self, item);
            break;
        case T_CONTINUE:
            return_value = !is_semicolon(self) &&
                           !function_inside1(self);
            break;
        case T_IF:
            return_value = (
                !condition(self, item->childTable) &&
                !statement_inside1(self, item->childTable) &&
                !if_else1(self, table) &&
                !function_inside1(self, item)
            );
            break;
        case T_RETURN:
            return_value = !expresion(self, item->childTable) &&
                           !statement_inside1(self, item->childTable);
            break;
        case T_INTEGER:
        case T_STRING:
        case T_DOUBLE:
            return_value = !is_ID(self, item->childTable, &active) &&
                           !sth_next(self, item->childTable, active) &&
                           !function_inside1(self, item);
            break;
        case T_IDENTIFIER:
            return_value = !fce(self) &&
                           !function_inside1(self, item);
            break;
        default:
            return_value = 2;
            print_unexpected(active);
            break;
    }
    return return_value;
}

/**
 * Fetch next token, expects semicolon
 * @param self global structure
 * @return 0 if successfull
 **/
int is_semicolon(ifjInter *self)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module, self->table);
    if (active->type != T_SEMICOLON)
    {
        print_unexpected(active);
        return_value = 2;
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

/**
 * Fetch next token. Expect "else", if unexpected, perform token push bezparametricka
 * @param self global structure
 * @param table table for current function
 * @return 0 if else, 1 if pushBack
 **/
int if_else1(ifjInter *self, symbolTable *table)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module, table);
    if (active->type == T_ELSE)
    {
        return_value = is_LBLOCK(self) && statement_inside1(self);
    }
    else
    {
      self->pushBack = active;
      return_value = 1;
    }
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

/**
 * Inside statement (while, if, for ...)
 * - no variable declaration here!
 * - Expects "}", "while", "for", "if", "break", "continue", "return", "id"
 * @param self global structure
 * @param table symbol table for current function
 * @return 0 if successful
 **/
int statement_inside1(ifjInter *self, symbolTable *table)
{
    int return_value = 0;
    token * active = NULL;
    if(self->pushBack)
    {
        active = self->pushBack;
        self->pushBack = NULL;
    }
    else
    {
        active = lexa_next_token(self->lexa_module, table);
    }
    switch (active->type)
    {
        //END
        case T_RBLOCK:
          return_value = 0;
          break;

        case T_WHILE:
            return_value = !condition(self, table) &&
                           !statement_inside1(self, table) &&
                           !statement_inside1(self, table);
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
            return_value = !is_semicolon(self) &&
                           !statement_inside1(self, table);
            break;
        case T_CONTINUE:
            return_value = !is_semicolon(self) &&
                           !statement_inside1(self, table);
            break;
        case T_IF:
            return_value = !condition(self, table) &&
                           !statement_inside1(self, table) &&
                           !if_else1(self, table) &&
                           !statement_inside1(self, table);
            break;
        case T_RETURN:
            return_value = !expresion(self, table) &&
                           !statement_inside1(self, table);
            break;
        case T_IDENTIFIER:
            return_value = resolve_identifier(self, table, &active, 0);
            if(!return_value)
            {
                return_value = !fce(self, table, active) &&
                               !statement_inside1(self, table);
            }
            break;
        default:
            print_unexpected(active);
            return_value = 2;
            break;
    }
    return return_value;
}

/**
 * After identifier. Expects "(" - func or "=" for expresion
 * @param self global structure
 * @param table symbol table for current function_parameters
 * @param last token/identifier
 * @return 0 if successful
 **/
int fce(ifjInter *self, symbolTable *table, token *item)
{
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active->type == T_LPAREN)
    {
        return function_parameters(self) && !is_semicolon(self);
    }
    else if (active->type == T_ASSIGN)
    {
        //TODO typing + instruction
        return expresion(self);
    }
    print_unexpected(active);
    return 2;
}

/**
 * Resolving functon call with return
 * fetch next token. Expect ")" or next function param
 * TODO We should check argument count and type here
 * @param self global structure
 * @param table symbol table for current function (not called one!)
 * @param item called function structure
 * @return 0 if successful
 **/
int function_parameters(ifjInter *self, symbolTable *table, token *item)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module, table);
    if (active->type == T_IDENTIFIER ||
        active->type == T_INTEGER_C ||
        active->type == T_DOUBLE_C ||
        active->type == T_STRING_C)
    {
        return_value = next_function_parameters_for_exp(self, table, item);
    }
    else if (active->type != T_RPAREN)
    {
        print_unexpected(active);
        return_value = 2;
    }
    return return_value;
}

/**
 * Resolving function call, fetch next token. Expect ");" or next func param
 * TODO We should check argument count and type here
 * @param self global structure
 * @param table symbol table for current function (not called one!)
 * @param item called function structure
 * @return 0 if successful
 **/
int next_function_parameters(ifjInter *self, symbolTable *table, token *item)
{
    int return_value = 0;
    token *active = lexa_next_token(self->lexa_module,self->table);
    token *expected = NULL; //TODO
    if (active->type == T_RPAREN)
    {
        return_value = !is_semicolon(self);
    }
    else if (active->type == T_COMMA)
    {
        return_value = next_param(self, table, expected) &&
                       next_function_parameters_for_exp(self);
    }
    else
    {
        print_unexpected(active);
        return_value = 2;
    }
    return return_value;
}

/**
 * Fetch next token. Expect "=" or ";".
 * @param self global structure
 * @param table symbol table for current function
 * @param item last token/identifier for instruction generation/type control
 * @return 0 if successful
 **/
int sth_next(ifjInter *self, symbolTable *table, token *item)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module, table);
    if (active->type == T_ASSIGN)
    {
        //TODO check typing
        return_value = expresion(self);
        // TODO generate instruction
    }
    else if (active->type != T_SEMICOLON)
    {
        print_unexpected(active);
        return_value = 2;
    }
    return return_value;
}

//TODO XXX JANY načo je toto?
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

/**
 * Resolving functon call with return
 * fetch next token. Expect ")" or next function param
 * TODO We should check argument count and type here
 * @param self global structure
 * @param table symbol table for current function (not called one!)
 * @param item called function structure
 * @return 0 if successful
 * TODO resolve return
 **/
int function_parameters_for_exp(ifjInter *self,
                                symbolTable *table,
                                token *item)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module, table);
    if (active->type == T_IDENTIFIER ||
        active->type == T_INTEGER_C ||
        active->type == T_DOUBLE_C ||
        active->type == T_STRING_C)
    {
        return_value = next_function_parameters_for_exp(self, table, item);
    }
    else if (active->type != T_RPAREN)
    {
        print_unexpected(active);
        return_value = 2;
    }
    return return_value;
}

/**
 * Resolving function call with return
 * fetch next token. Expect ");" or next func param
 * TODO We should check argument count and type here
 * @param self global structure
 * @param table symbol table for current function (not called one!)
 * @param item called function structure
 * @return 0 if successful
 **/
int next_function_parameters_for_exp(ifjInter *self,
                                     symbolTable *table,
                                     token *item)
{
    int return_value = 0;
    token *active = lexa_next_token(self->lexa_module,self->table);
    token *expected = NULL; //TODO
    if (active->type == T_RPAREN)
    {
        return_value = !is_semicolon(self);
    }
    else if (active->type == T_COMMA)
    {
        return_value = next_param(self, table, expected) &&
                       next_function_parameters_for_exp(self);
    }
    else
    {
        print_unexpected(active);
        return_value = 2;
    }
    return return_value;
}
