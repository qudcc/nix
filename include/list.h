/*
 * Copyright (C) Qu. All right reserved.
 */


#ifndef NIX_LIST_H
#define NIX_LIST_H


typedef struct nix_list_node_t {
    struct nix_list_node_t *prev;
    struct nix_list_node_t *next;
} nix_list_node_t;

#define nix_list_tail(l) (l)->prev
#define nix_list_head(l) (l)->next
#define nix_list_prev(node) (node)->prev
#define nix_list_next(node) (node)->next

#define nix_list_init(l) nix_list_tail(l) = nix_list_head(l) = (l)
#define nix_list_nil(node) nix_list_prev(node) = nix_list_next(node) = NULL

#define nix_list_is_empty(l) ((l) == nix_list_tail(l))
#define nix_list_is_head(l, node) ((node) == nix_list_head(l))
#define nix_list_is_tail(l, node) ((node) == nix_list_tail(l))
#define nix_list_is_exist(node) (nix_list_next(node))

#define nix_list_remove(node) \
    nix_list_prev(nix_list_next(node)) = nix_list_prev(node), \
    nix_list_next(nix_list_prev(node)) = nix_list_next(node)
#define nix_list_remove_nil(node) nix_list_remove(node), nix_list_nil(node)
#define nix_list_remove_if(node) if (nix_list_is_exist(node)) \
    nix_list_remove_nil(node)

#define nix_list_remove_head(l) ({ nix_list_node_t *_ = nix_list_head(l); \
    nix_list_head(l) = nix_list_next(_), \
    nix_list_prev(nix_list_next(_)) = (l), \
_;})

#define nix_list_remove_tail(l) ({ nix_list_node_t *_ = nix_list_tail(l); \
    nix_list_tail(l) = nix_list_prev(_), \
    nix_list_next(nix_list_prev(_)) = (l), \
_;})

#define nix_list_insert_head(l, node) \
    nix_list_prev(node) = (l), nix_list_next(node) = nix_list_head(l), \
    nix_list_prev(nix_list_head(l)) = (node), nix_list_head(l) = (node)

#define nix_list_insert_tail(l, node) \
    nix_list_prev(node) = nix_list_tail(l), nix_list_next(node) = (l), \
    nix_list_next(nix_list_tail(l)) = (node), nix_list_tail(l) = (node)

#define nix_list_move_head(l, node) \
    nix_list_remove(node), nix_list_insert_head(l, node)

#define nix_list_move_tail(l, node) \
    nix_list_remove(node), nix_list_insert_tail(l, node)

#define nix_list_each(l, cb) ({ nix_list_node_t *node, *_next; \
    for (node = nix_list_head(l); node != (l); node = _next) \
        _next = node->next, cb; \
node; })

#define nix_list_remove_until(l, cb) ({ nix_list_node_t *node; \
    node = nix_list_each(l, cb); \
    nix_list_head(l) = node, nix_list_prev(node) = (l); \
})


#endif /* NIX_LIST_H */
