/* htable.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Jakub Kulich <xkulic03@stud.fit.vutbr.cz>
 */

#ifndef IFJ_HTABLE_H
#define IFJ_HTABLE_H

/**
 * List of items
 *
 * *key - pointer to dynamically allocated array of characters where is key
 * data - number of item occurrence
 * *next - pointer to next item in list
 */
struct htab_listitem {

    unsigned id;
    char *key;
    unsigned data;
    struct htab_listitem *next;

};

/**
 * Hash table
 *
 * htab_size - size of hash table
 * *hash_fun_ptr - pointer to hash function
 * n - number of items in table
 * ptr - list of pointers to items list
 */
struct htab_t {

    unsigned htab_size;
    unsigned (*hash_fun_ptr)(const char *str, unsigned htab_size);
    unsigned n;
    struct htab_listitem ptr[];

};

/**
 * Function initialize hash table of size "size"
 * @param size Size of hash table
 * @return Pointer to hash table
 */
struct htab_t *htab_init(unsigned size);

/**
 * Function initialize hash table of size "size" with custom hash function "hashfn"
 * @param size Size of hash table
 * @param hashfn Pointer to hash function
 * @return Pointer to hash table
 */
struct htab_t *htab_init2(unsigned size, unsigned (*hashfn)(const char *, unsigned));

/**
 * Find item with key "key" in hash table "t".
 * If item is found: increase number "data" in item and return pointer to this item.
 * If item is not found: create new item and return pointer to this item.
 * @param t Pointer to hash table to lookup
 * @param key Key of searched item
 * @return Pointer to found item. NULL if allocation of memory for new item
 * wasn't successful
 */
struct htab_listitem *htab_lookup_add(struct htab_t *t,
                                      char *key, unsigned id);
/**
 * Find item with key "key" in hash table "t".
 *
 * @param t Pointer to hash table to lookup
 * @param key Key of searched item
 * @return Pointer to found item. NULL if item not found
 */
struct htab_listitem *htab_lookup(struct htab_t *t, char *key);

/**
 * Function calls function "func" for every item in hash table
 * @param t Pointer to hash table with items
 * @param func Pointer to function that is called
 */
void htab_foreach(struct htab_t *t, void (*func)(char *, int));

/**
 * Function finds item with key "key" in hash table "t" and removes it.
 * @param t Pointer to hash table with items
 * @param key Key of searched item
 */
void htab_remove(struct htab_t *t, char *key);

/**
 * Function removes all items in hash table "t"
 * @param t Pointer to hash table where all items are removed
 */
void htab_clear(struct htab_t *t);

/**
 * Function removes all items in hash table "t" and then removes whole hash table
 * Function frees allocated memory for hash table
 * @param t Pointer to hash table which is removed
 */
void htab_free(struct htab_t *t);

/**
 * Default hash function
 * @param str Key that is hashed
 * @param htab_size Size of hash table
 * @return Index of item in hash table
 */
unsigned int hash_function(const char *str, unsigned htab_size);

#endif //IFJ_HTABLE_H
