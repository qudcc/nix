/*
 * Copyright (C) Qu. All right reserved.
 */

#ifndef NIXBIN_HEAP_H
#define NIXBIN_HEAP_H


typedef struct {
    int     (*pred)(intptr_t lhs, intptr_t rhs);
    int       size;
    int       len;
    intptr_t  buf[];
} nix_bin_heap_t;

#define nix_bin_heap_init(bh, sz, cmp) \
    (bh)->size = sz, (bh)->len = 0, (bh)->pred = cmp

/* binary heap float */
#define nix_bin_heap_float(bh, p, idx)  { int  i, n; \
    /* compare with parent */ \
    for (i = (idx); 0 < i && 0 < pred(p, (bh)->buf[n = (i+1) / 2 - 1]); \
        (bh)->buf[i] = (bh)->buf[n], i = n); \
    (bh)->buf[i] = p; \
}

/* binary heap sink */
#define nix_bin_heap_sink(bh, p, idx)  { int  i, n; \
    /* compare with smaller child */ \
    for (i = idx; (n = i * 2 + 1) < (bh)->len; \
        (bh)->buf[i] = (bh)->buf[n], i = n) { \
        if (n + 1 < (bh)->len && pred((bh)->buf[n + 1], (bh)->buf[n]) < 0) \
            ++n; \
        if (pred(p, (bh)->buf[n]) < 0) break; \
    } \
    (bh)->buf[i] = p; \
}

#define nix_bin_heap_enqueue(bh, p)  nix_bin_heap_float(bh, p, bh->len++)
#define nix_bin_heap_dequeue(bh, p)  \
    p = *(bh)->buf, nix_bin_heap_sink(bh, (bh)->buf[--bh->len], 0)


#endif /* NIX_BINHEAP_H */
