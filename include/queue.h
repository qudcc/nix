/*
 * Copyright (C) Qu. All right reserved.
 */


#ifndef NIX_QUEUE_H
#define NIX_QUEUE_H


typedef struct nix_queue_node_t {
    struct nix_queue_node_t *next; /* last is null */
} nix_queue_node_t;
typedef struct {
    nix_queue_node_t head; /* sentinel also */
    nix_queue_node_t *tail;
} nix_queue_t;

#define nix_queue_head(q) (q)->head.next
#define nix_queue_tail(q) (q)->tail
#define nix_queue_next(node) (node)->next

#define nix_queue_empty(q) !nix_queue_head(q)
#define nix_queue_init(q) nix_queue_head(q) = NULL, \
    nix_queue_tail(q) = &(q)->head

#define nix_queue_enqueue(q, n) \
    (q)->tail->next = (n), (q)->tail = (n), (n)->next = NULL

#define nix_queue_dequeue(q) ({ nix_queue_node_t *_ = nix_queue_head(q); \
    if (!(nix_queue_head(q) = nix_queue_next(_))) \
        nix_queue_tail(q) = &(q)->head; \
_; })

#define nix_queue_each(q, cb) ({ nix_queue_node_t *node, *_next; \
    for (node = nix_queue_head(q); node; \
        _next = nix_queue_next(node), cb, node = _next); \
node; })

#define nix_queue_dequeue_until(q, cb) ({ nix_queue_node_t *node; \
    node = nix_queue_each(l, cb) ? nix_queue_head(q) = node \
        : nix_queue_tail(q) = &(q)->head; \
})


#endif /* NIX_QUEUE_H */
