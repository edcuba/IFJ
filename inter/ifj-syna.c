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

/* TODO EDO  na zaciatku by mala byt vytvorena niejaka tabulka  globalna kde bude vsetko + 1 tabulka pre classu Main sa urobi hned na zaciatku este pred pustenim  analyz*/
/*TODO EDO kazda funkcia obsahuje premennu active kde je ulozeny akutualny  token  2. parameter je vzdy self->table  nastaveny, len mi nieje jasne ako to urobit aby sa napriklad pri kazdom volani
funkcie ukladalo do inej tabulky kedze stale sa bude volat ten isty kod, dufam ze tebe je :D*/
/* TODO EDO  pri kazdom volani tokenu najdes popis o co sa jedna a  kde sa nachadza*/

int start(ifjInter *self)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active->type ==  T_CLASS)
    {
        /* TODO EDO  uplne prve slovo  v zdrojaku cize  token class k prvej classe Main*/
        return_value = (is_ID(self) &&
        /*TODO  JANY tu treba overit ci to is_ID je Main, cize bud  upravit funkciu  a spravit v nej 2 volania lexa_next_token, alebo si urobit   static premennu counter ktora to skontroluje ked sa
        1.x zavola is_ID --> prve ID musi byt main */
                        class_inside(self) &&
                        next_class(self));
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
    if ((active->type) == T_END)
    {
          /* TODO EDO   token je koncovy znak suboru  PS neviem do ktorej tabulky to ukadat*/
        return_value =  1;
    }
    else if (active->type == T_CLASS && is_ID(self))
    {
        /* TODO EDO   nasiel dafiniciu dalsej classy takze vytvara tabulku pre novu classu ktorej nazov je token ktory dostava is_ID, viem to prerobit aby
        ten token s id dostavala rovno aj tato funkcia  staci povedat, ale myslim ze je jednoduchsie vytvorit nieco na ukladanie viz presny popis nizsie */
        return_value = (class_inside(self) && next_class(self));
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
    if (active->type == T_LBLOCK)
    { /* TODO EDO  "{" ktora zacina vnutro classy*/
        return_value = class_inside1(self);
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
    if (active->type == T_RBLOCK)
    {
      /* TODO EDO  *"}" znaciaca koniec classy */
        return_value = 1;
    }
    else
    {
        if (active->type == T_STATIC)
        {
          /* TODO EDO   static cize zacina definicia bud premennej alebo  funkcie*/
            return_value = (tell_me_type_with_void(self) && is_ID(self) && class_inside2(self));
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
    if (active->type == T_IDENTIFIER)
    { /* TODO EDO  dostava ID o ake ide ide urcuju ostatne funkcie*/
        return_value = 1;
    }
    else
    {
        return_value = 0;
    }
    return return_value;
}



int is_ID_number(ifjInter *self)
{
  int return_value = 0;
  token * active = lexa_next_token(self->lexa_module,self->table);
  /* TODO EDO  pouziva sa iba pri volani funkcie, kontroluje ci je v argumentoch iba to co tam moze byt (syntakticky nie sematicky)
  , mozes dorobit aj sematicku kontrolu ci sedia typy tak ako je v deklaracii, funkcie s tym suvisiace next_function_parameters, function_parameters,
  a  function_parameters_for_exp, next_function_parameters_for_exp   presne to iste len to 2.  komunikuje  s exp.c, pre teba by to nemalo znamenat rozdiel*/
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


int class_inside2(ifjInter *self)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active->type == T_LPAREN)
    /* TODO EDO  vytvaras novu tabullku pre funkciu, nachadzas sa v deklaracii funkcie  prave si nasiel "(" ktora ti hovori ze predchadzajuce ID bolo id funkcie,
    asi treba dorobit niejaku strukturu, glob. premennu ktora si bude ukladat  posledne ID z lexikalnej  pomocou ktorej sa  k nemu budeme vediet dostat
     pre najdenie vsetkych tychto situacii CTRL + F T_IDENTIFIER, alebo mi daj vediet a urobim to ja */
    {
        return_value = (function_declar(self) && function_inside(self) && class_inside1(self));
    }
    else if (active->type == T_SEMICOLON)
    {
        return_value = class_inside1(self);
    }
    else if (active->type == T_COMMA)
    {
        return_value = id_declar(self) && class_inside1(self);
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
    /* TODO EDO  sme pri deklaracii a urcovani datoveho typu  s voidom cize stale v classe mimo akejkolvek funkcie */
    /* TODO JANNY treba skontrolovat ze ak ide o deklaraciu premennej ci nieje pouzity typ void a zistit ako tuto chybu specifikovat*/
    switch(active->type)
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
    switch(active->type)
    /* TODO EDO  deklaracia vo funkcii */
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
    /* TODO EDO  riesi  pripad viacnasobnej deklaracie mimo funkcie v classe priklad: int a, b, c;*/
    if (active->type == T_IDENTIFIER)
    {
        return_value = id_declar1(self);
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
    /* TODO EDO  to iste co id_declar, pracuju spolu a navzajom sa cyklia*/
    if (active->type == T_SEMICOLON)
    {
        return_value = 1;
    }
    else if (active->type == T_COMMA)
    {
        return_value = id_declar(self);
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
    switch(active->type)
    /* TODO EDO   deklaracie funkcie  v classe, konkretne riesi prvy paramenter*/
    {
        case T_INTEGER:

        case T_DOUBLE:

        case T_STRING:
        return_value = (is_ID(self) && next_function_param(self));
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
    /* TODO EDO  riesi 2. az n-ty parameter pri deklaracii funkcie */
    if (active->type == T_RPAREN)
    {
        return_value = 1;
    }
    else if (active->type == T_COMMA)
    {
        return_value = (tell_me_type_without_void(self) && is_ID(self) && next_function_param(self));
    }
    else
    {
        return_value = 0;
    }
    return return_value;
}


int function_inside(ifjInter *self)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active->type == T_LBLOCK)
    /* TODO EDO  "{" ktora znaci zaciatok funkcie  */
    {
        return_value = function_inside1(self);
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
    /* TODO EDO  sme vo vnutri funcie */
    switch (active->type)
    {
        case T_RBLOCK:
        return_value = 1;
        break;
        case T_WHILE:
        return_value = (
                        condition(self) &&
                        statement_inside(self) &&
                        function_inside1(self));
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
                        function_inside1(self));
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
        return_value = 0;
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
    /* TODO EDO   vyuzivane vyhradne vo vnutri funkcie, resp, momentalne nevyuzivane vobec pouzivam pri DO a FOR ktore zatial niesu plne funkcne*/
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
