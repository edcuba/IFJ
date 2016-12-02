#ifndef IFJ_TABLES_H
#define IFJ_TABLES_H


#define T_L '<'
#define T_G '>'
#define T_E 501
#define T_Q 603
#define T_C ','
#define T_R '?' // END of  conditon

int predictCondition[17][17] =
{
    {T_G, T_G, T_L, T_L, T_L, T_G, T_L, T_E, T_L, T_L, T_L, T_G, T_G, T_G, T_G, T_G, T_G},
    {T_G, T_G, T_L, T_L, T_L, T_G, T_L, T_E, T_L, T_L, T_L, T_G, T_G, T_G, T_G, T_G, T_G},
    {T_G, T_G, T_G, T_G, T_L, T_G, T_L, T_E, T_L, T_L, T_L, T_G, T_G, T_G, T_G, T_G, T_G},
    {T_G, T_G, T_G, T_G, T_L, T_G, T_L, T_E, T_L, T_L, T_L, T_G, T_G, T_G, T_G, T_G, T_G},
    {T_L, T_L, T_L, T_L, T_L, T_Q, T_L, T_E, T_L, T_L, T_L, T_L, T_L, T_L, T_L, T_L, T_L},
    {T_G, T_G, T_G, T_G, T_E, T_G, T_E, T_E, T_E, T_E, T_E, T_G, T_G, T_G, T_G, T_G, T_G},
    {T_G, T_G, T_G, T_G, T_E, T_G, T_E, T_E, T_E, T_E, T_E, T_G, T_G, T_G, T_G, T_G, T_G},
    {T_L, T_L, T_L, T_L, T_L, T_R, T_L, T_E, T_L, T_L, T_L, T_L, T_L, T_L, T_L, T_L, T_L},
    {T_G, T_G, T_G, T_G, T_E, T_G, T_E, T_E, T_E, T_E, T_E, T_G, T_G, T_G, T_G, T_G, T_G},
    {T_G, T_G, T_G, T_G, T_E, T_G, T_E, T_E, T_E, T_E, T_E, T_G, T_G, T_G, T_G, T_G, T_G},
    {T_G, T_G, T_G, T_G, T_E, T_G, T_E, T_E, T_E, T_E, T_E, T_G, T_G, T_G, T_G, T_G, T_G},
    {T_L, T_L, T_L, T_L, T_L, T_G, T_L, T_E, T_L, T_L, T_L, T_G, T_G, T_G, T_G, T_G, T_G},
    {T_L, T_L, T_L, T_L, T_L, T_G, T_L, T_E, T_L, T_L, T_L, T_G, T_G, T_G, T_G, T_G, T_G},
    {T_L, T_L, T_L, T_L, T_L, T_G, T_L, T_E, T_L, T_L, T_L, T_G, T_G, T_G, T_G, T_G, T_G},
    {T_L, T_L, T_L, T_L, T_L, T_G, T_L, T_E, T_L, T_L, T_L, T_G, T_G, T_G, T_G, T_G, T_G},
    {T_L, T_L, T_L, T_L, T_L, T_G, T_L, T_E, T_L, T_L, T_L, T_G, T_G, T_G, T_G, T_G, T_G},
    {T_L, T_L, T_L, T_L, T_L, T_G, T_L, T_E, T_L, T_L, T_L, T_G, T_G, T_G, T_G, T_G, T_G}
};

int predictExpresion [11][11] = {
    {T_G, T_G, T_L, T_L, T_L, T_G, T_L, T_G, T_L, T_L, T_L},
    {T_G, T_G, T_L, T_L, T_L, T_G, T_L, T_G, T_L, T_L, T_L},
    {T_G, T_G, T_G, T_G, T_L, T_G, T_L, T_G, T_L, T_L, T_L},
    {T_G, T_G, T_G, T_G, T_L, T_G, T_L, T_G, T_L, T_L, T_L},
    {T_L, T_L, T_L, T_L, T_L, T_Q, T_L, T_E, T_L, T_L, T_L},
    {T_G, T_G, T_G, T_G, T_E, T_G, T_E, T_G, T_E, T_E, T_E},
    {T_G, T_G, T_G, T_G, T_C, T_G, T_E, T_G, T_E, T_E, T_E},
    {T_L, T_L, T_L, T_L, T_L, T_E, T_L, T_E, T_L, T_L, T_L},
    {T_G, T_G, T_G, T_G, T_E, T_G, T_E, T_G, T_E, T_E, T_E},
    {T_G, T_G, T_G, T_G, T_E, T_G, T_E, T_G, T_E, T_E, T_E},
    {T_G, T_G, T_G, T_G, T_E, T_G, T_E, T_G, T_E, T_E, T_E}
};

#endif
