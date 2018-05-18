/*
 * Copyright (C) Qu. All right reserved.
 */


#ifndef NIX_SPLAYTREE_H
#define NIX_SPLAYTREE_H


typedef struct nix_splay_tree_node_t {
    struct nix_splay_tree_node_t  *left, *right;
} nix_splay_tree_node_t;
typedef struct {
    nix_splay_tree_node_t  *root;
    int (*pred)(const nix_splay_tree_node_t *, intptr_t);
} nix_splay_tree_t;

void nix_splay_tree_init(nix_splay_tree_t *tree,
        int (*pred)(const nix_splay_tree_node_t *, intptr_t));
int nix_splay_tree_insert(nix_splay_tree_t *t, intptr_t key,
        nix_splay_tree_node_t *(*alloc)(intptr_t key));
nix_splay_tree_node_t *nix_splay_tree_erase(nix_splay_tree_t *t,
        intptr_t key);
nix_splay_tree_node_t *nix_splay_tree_find(nix_splay_tree_t *t,
        intptr_t key);
void nix_splay_tree_each(const nix_splay_tree_node_t *node,
        void (*cb)(const nix_splay_tree_node_t *node));


#endif /* NIX_SPLAYTREE_H */
