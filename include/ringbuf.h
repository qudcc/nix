/*
 * Copyright (C) Qu. All right reserved.
 */


#ifndef NIX_RINGBUF_H
#define NIX_RINGBUF_H


typedef struct {
    int size, read, write;
    char buf[];
} nix_ring_buf_t;

#define nix_ring_buf_init(b, sz) \
	(b)->size = sz, (b)->read = (b)->write = 0
#define nix_ring_buf_empty(b) ((b)->read == (b)->write)
#define nix_ring_buf_full(b) ((b)->read == (((b)->write + 1) % (b)->size))
#define nix_ring_buf_length(b) \
	(((b)->size + (b)->write - (b)->read) % (b)->size)
#define nix_ring_buf_read(b, len) (b)->read += len, (b)->read %= (b)->size
#define nix_ring_buf_write(b, len) (b)->write += len, (b)->write %= (b)->size


#endif /* NIX_RINGBUF_H */
