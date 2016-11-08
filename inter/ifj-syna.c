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
* @return 0 when successful
*/
int start(ifjInter *self)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active->type ==  T_CLASS)
    {
        return_value = (is_ID(self) &&
                        class_inside(self) &&
                        next_class(self));
        //take 2. token , this mean ID and save it to the class table like class ID
        // make table for new class

        /*symbolTable * class_table = ial_symbol_table_new(self);
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
    if ((active->type) == T_END)
    {
        return_value =  1;
    }
    else if (active->type == T_CLASS && is_ID(self))
    {
        return_value = (class_inside(self) && next_class(self));
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
    if (active->type == T_LBLOCK)
    {
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
        return_value = 1;
    }
    else
    {
        if (active->type == T_STATIC)
        {
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
    switch (active->type)
    {
        case T_RBLOCK:
        return_value = 1;
        break;
        case T_WHILE:
        return_value = (is_LPAREN(self) &&
                        condition(self) &&
                        is_RPAREN(self) &&
                        statement_inside(self) &&
                        function_inside1(self));
        break;
        case T_FOR:
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
        break;
        case T_DO:
        return_value = (statement_inside(self) &&
                        is_while(self) &&
                        is_LPAREN(self) &&
                        condition(self) &&
                        is_RPAREN(self) &&
                        is_semicolon(self) &&
                        function_inside1(self));
        break;
        case T_BREAK:
        return_value = (is_semicolon(self) && function_inside1(self));
        break;
        case T_CONTINUE:
        return_value = (is_semicolon(self) && function_inside1(self));
        break;
        case T_IF:
        return_value = (is_LPAREN(self) &&
                        condition(self) &&
                        is_RPAREN(self) &&
                        statement_inside(self) &&
                        if_else1(self) &&
                        function_inside1(self));
        break;
        case T_RETURN:
        return_value = (expresion(self) && is_semicolon(self) && statement_inside1(self));
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
    if (active->type == T_ELSE)
    {
        return_value = statement_inside(self);
    }
    // tu nastava problem treba dat else akurat co moze byt v tokene  ak tam nebude else?
    return return_value;
}

int statement_inside(ifjInter *self)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active->type == T_LBLOCK)
    {
        return_value = statement_inside1(self);
    }
    else
    {
        return_value = 0;
    }
    return return_value;
}

int statement_inside1(ifjInter *self)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    switch (active->type)
    {
        case T_RBLOCK:
        return_value = 1;
        break;
        case T_WHILE:
        return_value = (is_LPAREN(self) && condition(self) && is_RPAREN(self) && statement_inside(self) && statement_inside1(self));
        break;
        case T_FOR:
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
        break;
        case T_BREAK:
        return_value = (is_semicolon(self) && statement_inside1(self));
        break;
        case T_CONTINUE:
        return_value = (is_semicolon(self) && statement_inside1(self));
        break;
        case T_IF:
        return_value = (is_LPAREN(self) && condition(self) && is_RPAREN(self) && statement_inside(self) && if_else1(self) && statement_inside1(self));
        break;
        case T_RETURN:
        return_value = (expresion(self) && is_semicolon(self) && statement_inside1(self));
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


int condition(ifjInter *self)
{
    int return_value = 0;
    return_value = (expresion(self) && rel_operator(self) && expresion(self));
    return return_value;

}


int fce(ifjInter *self)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active->type == T_LPAREN)
    {
        return_value = (function_parameters(self) && is_semicolon(self));
    }
    else if (active->type == T_ASSIGN)
    {
        return_value = (expresion(self) && is_semicolon(self));
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
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active->type == T_LPAREN)
    {
        return_value = (function_parameters(self)  && is_semicolon(self));
    }
    else if (active->type == T_ASSIGN)
    {
        return_value = (expresion(self) && is_semicolon(self));
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
    if (active->type == T_RPAREN)
    {
        return_value = 1;
    }
    else if (active->type == T_IDENTIFIER)
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
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active->type == T_RPAREN)
    {
        return_value = 1;
    }
    else if (active->type == T_COMMA)
    {
        return_value = (is_ID(self) && next_function_parameters(self));
    }
    else
    {
        return_value = 0;
    }
    return return_value;
}




int sth_next(ifjInter *self)
{
    int return_value = 0;
    token * active = lexa_next_token(self->lexa_module,self->table);
    if (active->type == T_SEMICOLON)
    {
        return_value = 1;
    }
    else if (active->type == T_ASSIGN)
    {
        return_value = ( expresion(self) && is_semicolon(self));
    }
    else
    {
        return_value = 0;
    }
    return return_value;

}


int rel_operator(ifjInter *self)
{
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

int expresion(ifjInter *self) //TODO
{
    return 1;
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
