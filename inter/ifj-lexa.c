/* ifj-lexa.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Jakub Kulich <xkulic03@stud.fit.vutbr.cz>
 */

#include <stdlib.h>
#include "utils/htable.h"
#include "ifj-lexa.h"
#include "ifj-inter.h"

ifj_lexa *ifj_lexa_init() {
    ifj_lexa *l = malloc(sizeof(ifj_lexa));
    if (l == NULL) {
        return NULL;
    }

    // 29 for minimalising colision in hash table
    l->reserved_words = htab_init(29);

    return l;
}

void ifj_lexa_free(ifj_lexa *l) {
    htab_free(l->reserved_words);
    free(l);
}

void ifj_lexa_add_reserved(ifj_lexa *l, char *word, int token_type) {
    htab_lookup_add(l->reserved_words, word, (unsigned) token_type);
}

int ifj_lexa_is_reserved(ifj_lexa *l, char *word) {
    struct htab_listitem *item = htab_lookup(l->reserved_words, word);
    if (item == NULL) {
        return -1;
    } else {
        return item->id;
    }
}

token *lexa_next_token(ifj_lexa *l, ifjInter *self) {

//    token *newToken = malloc(sizeof(token));
//    if (newToken == NULL) {
//        return NULL;
//    }

    return 0;
}

