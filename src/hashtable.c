/*
 * Copyright (C) Qu. All right reserved.
 */


#include <sys/types.h>

#include <node.h>
#include <list.h>
#include <hashtable.h>


unsigned int /* the Bernstein hash function */
nix_hash(size_t len, const char *key) {
    unsigned int  n;
    for (n = 0; 0 < len--; n = (n << 5) + n + *key++);
    return n;
}

void
nix_hash_table_init(hash_table_t *ht, int size,
    unsigned int (*hash)(size_t len, intptr_t key);
    int (*cmp)(size_t len, intptr_t key, hash_table_node_t *node)) {
    ht->hash = hash;
    ht->cmp = cmp;
    p->len = 0;
    p->size = size;
    nix_list_init(&ht->list);
    for (; 0 < size--; nix_list_init(p->buckets + size));
}

int
nix_hash_table_add(hash_table_t *ht, size_t len, intptr_t key,
    hash_table_node_t *(*alloc)(hash_table_node_t *)) {
    hash_table_node_t  *p;
    unsigned int        hash = ht->hash(len, key);
    nix_list_node_t    *list = ht->buckets + hash % ht->size;
    nix_list_each(list, ({
        p = nix_ownerof(node, hash_table_node_t, bkt);
        if (hash == p->hash && !ht->cmp(len, key, p))
            return alloc(p) ? 0 : -1;
    }));
    if (!(p = alloc(NULL))) return -1;
    p->hash = hash;
    nix_list_insert_head(&ht->list, &p->lst);
    nix_list_insert_head(ht->buckets + p->hash % ht->size, &p->bkt);
    ++p->len;
    return 0;
}

int
nix_hash_table_del(hash_table_t *ht, size_t len, intptr_t key,
    hash_table_node_t **pp) {
    hash_table_node_t  *p;
    unsigned int        hash = ht->hash(len, key);
    nix_list_node_t    *list = ht->buckets + hash % ht->size;
    nix_list_each(list, ({
        p = nix_ownerof(node, hash_table_node_t, bkt);
        if (hash == p->hash && !ht->cmp(len, key, p)) {
            nix_list_remove(&p->bkt);
            nix_list_remove(&p->lst);
            --p->len;
            return *pp = p, 0;
        }
    }));
    return -1;
}

int
nix_hash_table_find(hash_table_t *ht, size_t len, intptr_t key,
    hash_table_node_t **pp) {
    hash_table_node_t  *p;
    unsigned int        hash = ht->hash(len, key);
    nix_list_node_t    *list = ht->buckets + hash % ht->size;
    nix_list_each(list, ({
        p = nix_ownerof(node, hash_table_node_t, bkt);
        if (hash == p->hash && !ht->cmp(len, key, p))
            return *pp = p, 0;
    }));
    return -1;
}
