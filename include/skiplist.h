/*
 * Copyright (C) Qu. All right reserved.
 */


#ifndef NIX_SKIPLIST_H
#define NIX_SKIPLIST_H


#ifndef NIX_SKIP_LIST_SIZE
#define NIX_SKIP_LIST_SIZE 16
#endif

#define skip_list_rand_level(max) ({ int  _; \
    for (_ = 0; ++_ < (max) && (rand() & 1);); \
_; })

typedef struct nix_skip_list_node_t {
    struct nix_skip_list_node_t  *next[0];
} nix_skip_list_node_t;

typedef struct {
    int                   size; /* capacity deep */
    int                   len;  /* level */
    nix_skip_list_node_t  head;
} nix_skip_list_t;

#define nix_skip_list_init(deap) ({ nix_skip_list_t  *_p; int  _; \
    if ((_p = (nix_skip_list_t *)malloc(sizeof(nix_skip_list_t) \
        + (deap) * sizeof(nix_skip_list_node_t *)))) { \
        nix_skip_list_node_t  **_node; \
        for (_node = _p->head.next, _p->size = _ = (deap), _p->len = 0; \
            0 < _--; *_node++ = NULL); \
    } \
_p; })

#define nix_skip_list_each(list, level, cb) ({ \
    nix_skip_list_node_t  *node, *_next; \
    for (node = (list)->head.next[level]; node; node = _next) \
        _next = node->next[level], (cb); \
node; })

#define nix_skip_list_find(list, pred) ({ int  _; \
    nix_skip_list_node_t  *_node, *_next; \
    for (_node = NULL, _next = &(list)->head, _ = (list)->len; 0 < _--;) \
        for (; (_node = _next->next[_]) && (pred)(_node) < 0; _next = _node); \
    if (_node && 0 < (pred)(_node)) _node = NULL; /* not found */ \
_node; })

#define nix_skip_list_insert(list, pred, alloc) ({ int  _; \
    nix_skip_list_node_t  *_node = NULL, *_next, *_prev[(list)->size]; \
    for (_next = &(list)->head, _ = (list)->size; 0 < _--; _prev[_] = _next) \
        for (; (_node = _next->next[_]) && (pred)(_node) < 0; _next = _node); \
    if (!_node || 0 < (pred)(_node)) { /* not exists */ \
        if ((list)->len < (_ = skip_list_rand_level((list)->size))) \
            (list)->len = _; \
        if ((_node = (alloc)(_))) { \
            for (; 0 < _--; _node->next[_] = _prev[_]->next[_], \
                _prev[_]->next[_] = _node); \
        } \
    } \
_node=_node; })

#define nix_skip_list_delete(list, pred) ({ int  _; \
    nix_skip_list_node_t  *_node = NULL, *_next, *_prev[(list)->size]; \
    for (_next = &(list)->head, _ = (list)->len; 0 < _--; _prev[_] = _next) \
        for (; (_node = _next->next[_]) && (pred)(_node) < 0; _next = _node); \
    if (_node && 0 < (pred)(_node)) _node = NULL; /* not found */ \
    for (_ = 0; _ < (list)->len && _prev[_]->next[_] == _node; \
        _prev[_]->next[_] = _node->next[_], ++_); \
    for (_next = &(list)->head, _ = (list)->len; \
        0 < _ && !_next->next[--_];); \
    (list)->len = ++_; \
_node; })


#endif /* NIX_SKIPLIST_H */
