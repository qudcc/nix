/*
 * Copyright (C) Qu. All right reserved.
 */


#ifndef NIX_STACK_H
#define NIX_STACK_H


typedef struct nix_stack_node_s nix_stack_node_t;
struct nix_stack_node_s {
    nix_stack_node_t *next;
};

#define nix_stack_top(s) (s)
#define nix_stack_next(node) (node)->next

#define nix_stack_init(s) nix_stack_top(s) = NULL
#define nix_stack_is_empty(s) (nix_stack_top(s) == NULL)

#define nix_stack_push(s, node) \
    nix_stack_next(node) = nix_stack_top(s), nix_stack_top(s) = (node)
#define nix_stack_pop(s) ({ nix_stack_node_t *_ = nix_stack_top(s); \
    nix_stack_top(s) = nix_stack_next(_), \
_; })

#define nix_stack_each(s, cb) ({ nix_stack_node_t *node, *_next; \
    for (node = nix_stack_top(s); node; \
        _next = node->next, cb, node = _next); \
node; })

#define nix_stack_pop_until(s, cb) nix_stack_top(s) = nix_stack_each(s, cb)


#endif /* NIX_STACK_H */
