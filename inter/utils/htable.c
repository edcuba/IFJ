/* htable.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Jakub Kulich <xkulic03@stud.fit.vutbr.cz>
 */

#include <stdlib.h>
#include <string.h>
#include "htable.h"

struct htab_t *htab_init(unsigned size) {

    struct htab_t *hash_table;
    hash_table = malloc(
            sizeof(struct htab_t) + size * sizeof(struct htab_listitem));

    if (hash_table == NULL) {

        return NULL;
    }

    hash_table->htab_size = size;
    hash_table->n = 0;
    hash_table->hash_fun_ptr = hash_function;

    for (unsigned i = 0; i < hash_table->htab_size; i++) {

        hash_table->ptr[i].key = NULL;
        hash_table->ptr[i].next = NULL;
        hash_table->ptr[i].data = 0;
        hash_table->ptr[i].id = 0;
    }

    return hash_table;
}

struct htab_t *htab_init2(unsigned size,
                          unsigned (*hashfn)(const char *, unsigned)) {

    struct htab_t *hash_table;
    hash_table = htab_init(size);

    if (hash_table == NULL) {

        return NULL;
    }

    hash_table->hash_fun_ptr = hashfn;

    return hash_table;
}

struct htab_listitem *htab_lookup_add(struct htab_t *t,
                                      char *key, unsigned id) {

    /// Calculate hash for item
    unsigned hash = t->hash_fun_ptr(key, t->htab_size);

    /// Create pointer to first item in list
    struct htab_listitem *item = &t->ptr[hash];

    /// Cycle through whole list
    while (item->key != NULL) {

        /// If key is that we search, increase data and n in table, return pointer
        if (strcmp(item->key, key) == 0) {

            item->data++;
            t->n++;

            return item;
        }

        /**
         * If key is not that we are searching and there is no item in list
         * create new item, if allocation wasn't successful, return NULL
         */
        if (item->next == NULL) {

            item->next = malloc(sizeof(struct htab_listitem));
            if (item->next == NULL) {

                return NULL;
            }

            item->next->key = NULL;
            item->next->next = NULL;
        }

        /// If new item was added to end of list, while cond. won't be met
        item = item->next;
    }

    /**
     * Alloc. place for new item key, set key to our value, set data to 1
     * and return pointer
     */

    item->key = malloc(strlen(key) + 1);
    if (item->key == NULL) {

        return NULL;
    }

    strcpy(item->key, key);
    item->data = 1;
    item->id = id;
    t->n++;

    return item;
}

struct htab_listitem *htab_lookup(struct htab_t *t, char *key) {

    /// Calculate hash for item
    unsigned hash = t->hash_fun_ptr(key, t->htab_size);

    /// Create pointer to first item in list
    struct htab_listitem *item = &t->ptr[hash];

    /// Cycle through whole list
    while (item->key != NULL) {

        /// If key is that we search, increase data and n in table, return pointer
        if (strcmp(item->key, key) == 0) {
            return item;
        }
        item = item->next;
    }

    return NULL;
}

void htab_foreach(struct htab_t *t, void (*func)(char *, int)) {

    struct htab_listitem *item = NULL;

    /// Iterate through all lists and then iterate through each one list and call func.
    for (unsigned i = 0; i < t->htab_size; i++) {

        item = &t->ptr[i];

        while (item != NULL && item->key != NULL) {

            func(item->key, item->data);
            item = item->next;
        }
    }
}

void htab_remove(struct htab_t *t, char *key) {

    unsigned hash = t->hash_fun_ptr(key, t->htab_size);

    struct htab_listitem *item = &t->ptr[hash];
    struct htab_listitem *item2 = &t->ptr[hash];

    /**
     * We can't free memory of first item in list, so if searched item is first
     * in list we copy data from last item and remove last item.
     *
     * If searched item isn't first in list, we copy pointer of next item to
     * previous item and then we remove item.
     *
     * Removing item consists from freeing memory of key and then freeing
     * memory of item.
     */
    if (item->key != NULL && strcmp(item->key, key) == 0) {

        while (item2->next->next == NULL) {

            item2 = item2->next;
        }
        free(item->key);
        item->key = malloc(strlen(item2->next->key));
        strcpy(item->key, item2->next->key);
        item->data = item2->next->data;
        free(item2->next->key);
        free(item2->next);
        item2->next = NULL;

    } else if (item->next != NULL) {

        do {

            if (item->next->key != NULL && strcmp(item->next->key, key) == 0) {

                item2 = item->next->next;
                free(item->next->key);
                free(item->next);
                item->next = item2;
            }
            item = item->next;

        } while (item->next->next != NULL);
    }

    /// Decrease number of entries in hash table
    t->n--;
}

void htab_clear(struct htab_t *t) {

    struct htab_listitem *item = NULL;
    struct htab_listitem *itemNext = NULL;

    /**
     * Removing items works as in htab_remove, in cycle remove always second
     * item from list while list consists only from 1 item.
     * Then free key in first item, if was allocated, pointers to NULL, data to 0
     * and set hash table number of entries to 0
     */

    for (unsigned i = 0; i < t->htab_size; i++) {

        item = &t->ptr[i];

        while (item->next != NULL) {

            if (item->next->key != NULL) {

                free(item->next->key);
            }

            itemNext = item->next->next;

            free(item->next);
            item->next = itemNext;
        }

        if (item->key != NULL) {

            free(item->key);
        }

        item->next = NULL;
        item->key = NULL;
        item->data = 0;

    }

    t->n = 0;
}

void htab_free(struct htab_t *t) {

    /// Same as htab_clear but hash table is also freed
    htab_clear(t);
    free(t);
}

unsigned int hash_function(const char *str, unsigned htab_size) {

    unsigned int h=0;
    const unsigned char *p;

    for(p=(const unsigned char*)str; *p!='\0'; p++) {

        h = 65599*h + *p;
    }

    return h % htab_size;
}