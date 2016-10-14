#include "../ifj-inter.h"
#include <stdlib.h>
#include <string.h>

#define test(asdf) if (asdf == 1) return 1;
static int check_type(int type, token *token1) {
    if (token1->type != type) {
        printf("Expected token of type %d and got token of type %d\n", type,
               token1->type);
        return 1;
    }
    return 0;
}

static int check_and_destroy_string(int type, char *value, token *t) {
    if (check_type(type, t)) {
        return 1;
    }

    if (strcmp(value, t->value)) {
        printf("Expected token value: %s\n", value);
        printf("Got token value: %s\n", t->value);
    }
    free(t);
}

static int check_and_destroy_double(int type, double value, token *t) {
    if (check_type(type, t)) {
        return 1;
    }

    if (value != t->value) {
        printf("Expected token value: %f\n", value);
        printf("Got token value: %f\n", t->value);
    }

    free(t);
}

static int check_and_destroy_integer(int type, int value, token *t) {
    if (check_type(type, t)) {
        return 1;
    }

    if (value != t->value) {
        printf("Expected token value: %d\n", value);
        printf("Got token value: %d\n", t->value);
    }

    free(t);
}

static int check_lexical_analysis(ifjInter *interpret) {

    interpret->inputFile = fopen("inter/tests/lexa1.ifj16", 'r');
    if (interpret->inputFile == NULL) {
        printf("Opening of input file failed");
    }

    ifj_lexa *lex_anal = ifj_lexa_init();

    if (lex_anal == NULL) {
        printf("Initialization of lexical analysis module failed");
        return 1;
    }

    test(check_type(T_CLASS, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_string(T_IDENTIFIER, "Main", lexa_next_token(lex_anal, interpret)));
    test(check_type(T_LBLOCK, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_VOID, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_string(T_IDENTIFIER, "run", lexa_next_token(lex_anal, interpret)));
    test(check_type(T_LPAREN, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_RPAREN, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_LBLOCK, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_INTEGER, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_string(T_IDENTIFIER, "a", lexa_next_token(lex_anal, interpret)));
    test(check_type(T_SEMICOLON, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_string(T_IDENTIFIER, "ifj16", lexa_next_token(lex_anal, interpret)));
    test(check_type(T_DOT, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_string(T_IDENTIFIER, "print", lexa_next_token(lex_anal, interpret)));
    test(check_type(T_LPAREN, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_string(T_STRING_C, "Zadejte cislo pro vypocet faktorialu: ", lexa_next_token(lex_anal, interpret)));
    test(check_type(T_RPAREN, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_SEMICOLON, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_string(T_IDENTIFIER, "a", lexa_next_token(lex_anal, interpret)));
    test(check_type(T_ASSIGN, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_string(T_IDENTIFIER, "ifj16", lexa_next_token(lex_anal, interpret)));
    test(check_type(T_DOT, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_string(T_IDENTIFIER, "readInt", lexa_next_token(lex_anal, interpret)));
    test(check_type(T_LPAREN, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_RPAREN, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_SEMICOLON, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_IF, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_LPAREN, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_string(T_IDENTIFIER, "a", lexa_next_token(lex_anal, interpret)));
    test(check_type(T_LESS, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_integer(T_INTEGER_C, 0, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_RPAREN, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_LBLOCK, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_string(T_IDENTIFIER, "ifj16", lexa_next_token(lex_anal, interpret)));
    test(check_type(T_DOT, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_string(T_IDENTIFIER, "print", lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_string(T_STRING_C, "Faktorial nelze spocitat!\\n", lexa_next_token(lex_anal, interpret)));
    test(check_type(T_RPAREN, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_SEMICOLON, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_RBLOCK, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_ELSE, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_LBLOCK, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_string(T_IDENTIFIER, "vysl", lexa_next_token(lex_anal, interpret)));
    test(check_type(T_ASSIGN, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_double(T_DOUBLE_C, 3.14, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_ASSIGN, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_double(T_DOUBLE_C, 3.14e-13, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_SEMICOLON, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_WHILE, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_LPAREN, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_string(T_IDENTIFIER, "a", lexa_next_token(lex_anal, interpret)));
    test(check_type(T_GREATER, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_integer(T_INTEGER_C, 0, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_RPAREN, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_LBLOCK, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_string(T_IDENTIFIER, "vysl", lexa_next_token(lex_anal, interpret)));
    test(check_type(T_ASSIGN, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_string(T_IDENTIFIER, "vysl", lexa_next_token(lex_anal, interpret)));
    test(check_type(T_MULTIPLY, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_string(T_IDENTIFIER, "a", lexa_next_token(lex_anal, interpret)));
    test(check_type(T_SEMICOLON, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_string(T_IDENTIFIER, "a", lexa_next_token(lex_anal, interpret)));
    test(check_type(T_ASSIGN, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_string(T_IDENTIFIER, "a", lexa_next_token(lex_anal, interpret)));
    test(check_type(T_SUBTRACT, lexa_next_token(lex_anal, interpret)));
    test(check_and_destroy_integer(T_INTEGER_C, 1, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_SEMICOLON, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_RBLOCK, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_RBLOCK, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_RBLOCK, lexa_next_token(lex_anal, interpret)));
    test(check_type(T_RBLOCK, lexa_next_token(lex_anal, interpret)));

    fclose(interpret->inputFile);
    ifj_lexa_free(lex_anal);

}