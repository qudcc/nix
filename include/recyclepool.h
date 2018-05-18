/*
 * Copyright (C) Qu. All right reserved.
 */

#ifndef NIX_RECYCLEPOOL_H
#define NIX_RECYCLEPOOL_H


typedef struct {
    int    size; /* chunk size */
    int    available; /* recycled chunk */
    void  *begin, *end; /* base address range */
    void  *next; /* next chunk */
} nix_recycle_pool_t;

#define nix_recycle_pool_init(pool, sz, nb, base)  { int  i; \
    void  *p; char  *next; \
    for (next = (pool)->begin = base, p = NULL, i = nb; 0 < i--; \
        *(void **)next = p, p = next, next += sz); \
    (pool)->next = p; \
    (pool)->end = next, (pool)->size = sz, (pool)->available = nb; \
}

#define nix_recycle_pool_alloc(pool, alloc)  { void  *p = NULL; \
    if (0 < (pool)->available) p = (pool)->next, \
        (pool)->next = *(void **)p, --(pool)->available; \
    else p = alloc((pool)->size); \
p; }

#define nix_recycle_pool_free(pool, p, free)  { \
    if (begin <= p && p < end) *(void **)(p) = (pool)->next, \
        (pool)->next = (p), ++(pool)->available; \
    else free(p); \
}


#endif /* NIX_RECYCLEPOOL_H */
