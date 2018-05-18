/*
 * Copyright (C) Qu. All right reserved.
 */


#ifndef NIX_HASHTABLE_H
#define NIX_HASHTABLE_H


typedef struct {
    unsigned         hash;
    nix_list_node_t  lst;
    nix_list_node_t  bkt;
} hash_table_node_t;

typedef struct {
    unsigned int (*hash)(size_t len, intptr_t key);
    int (*cmp)(size_t len, intptr_t key, hash_table_node_t *node);
    int              len;
    int              size;
    nix_list_node_t  list; /* for traversal */
    nix_list_node_t  buckets[];
} hash_table_t;

void nix_ht_init(hash_table_t *ht, int size,
    unsigned int (*hash)(size_t len, intptr_t key);
    int (*cmp)(size_t len, intptr_t key, hash_table_node_t *node));
int nix_ht_add(hash_table_t *ht, size_t len, intptr_t key,
    hash_table_node_t *(*alloc)(hash_table_node_t *));
int nix_ht_del(hash_table_t *ht, size_t len, intptr_t key,
    hash_table_node_t **pp);
int nix_ht_find(hash_table_t *ht, size_t len, intptr_t key,
    hash_table_node_t **pp);

#define nix_ht_remove(ht, node) (ht)->len, \
    nix_list_remove(&node->bkt), nix_list_remove(&node->lst)

#define nix_ht_each(ht, cb) ({ hash_table_node_t *node; \
    nix_list_node_t *_node, *_next; \
    for (_node = nix_list_head(&(ht)->list); _node != &(ht)->list; \
        _next = _node->next, \
        node = nix_ownerof(_node, hash_table_node_t, lst), \
        cb, _node = _next; \
node; })

unsigned int nix_hash(size_t len, const char *key);


#endif /* NIX_HASHTABLE_H */
