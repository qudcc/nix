/*
 * Copyright (C) Qu. All right reserved.
 */


#ifndef NIX_RBTREE_H
#define NIX_RBTREE_H


typedef struct nix_rbtree_node_t {
    intptr_t                   key : 63, color : 1;
    struct nix_rbtree_node_t  *parent, *left, *right;
} nix_rbtree_node_t;
typedef int (*nix_rbtree_predicate_pt)(nix_rbtree_node_t *node, intptr_t key);
typedef struct {
    nix_rbtree_node_t       *root;
    nix_rbtree_predicate_pt  pred; /* predicate */
} nix_rbtree_t;

nix_rbtree_node_t *nix_rbtree_insert(nix_rbtree_t *tree, intptr_t key,
    nix_rbtree_node_t *(*alloc)(intptr_t key));
void nix_rbtree_erase(nix_rbtree_t *tree, const nix_rbtree_node_t *node);
nix_rbtree_node_t *nix_rbtree_find(nix_rbtree_t *tree, intptr_t key);
nix_rbtree_node_t **nix_rbtree_upper(nix_rbtree_t *tree, intptr_t key,
    nix_rbtree_node_t **parent, nix_rbtree_node_t **node);
nix_rbtree_node_t **nix_rbtree_ceil(nix_rbtree_t *tree, intptr_t key,
    nix_rbtree_node_t **parent, nix_rbtree_node_t **node);

void nix_rbtree_init(nix_rbtree_t *tree, nix_rbtree_predicate_pt predicate);
int nix_rbtree_nil(const nix_rbtree_node_t *node);

nix_rbtree_node_t *nix_rbtree_first(nix_rbtree_node_t *node);
nix_rbtree_node_t *nix_rbtree_last(nix_rbtree_node_t *node);
nix_rbtree_node_t *nix_rbtree_next(nix_rbtree_node_t *node);
nix_rbtree_node_t *nix_rbtree_prev(nix_rbtree_node_t *node);
int nix_rbtree_height(const nix_rbtree_node_t *node);

/*
 * WARN
 * ? if much more each
 * call of cb is require more CPU time
 * so cb is embedded expression
 * can not be a function
#define nix_rbtree_each(tree, cb) ({ \
    nix_rbtree_node_t  *_next, *node = (tree)->root; \
    for (node = nix_rbtree_first(node); !nix_rbtree_nil(node);) { \
        _next = nix_rbtree_next(node); \
        if ((cb)(node) < 0) break; \
        node = _next; \
    } \
})
 */

#define nix_rbtree_each(tree, cb) ({ \
    nix_rbtree_node_t  *_next, *node = (tree)->root; \
    for (node = nix_rbtree_first(node); !nix_rbtree_nil(node); \
        _next = nix_rbtree_next(node), cb, node = _next); \
})


#endif /* NIX_RBTREE_H */
