/*
 * Copyright (C) Qu. All right reserved.
 */


#ifndef NIX_FLATBUF_H
#define NIX_FLATBUF_H


typedef struct {
    int size, read, write;
    char buf[];
} nix_flat_buf_t;

#define nix_flat_buf_init(b, sz) \
	(b)->read = (b)->write = 0, (b)->size = (sz)
#define nix_flat_buf_empty(b) (!(b)->len)
#define nix_flat_buf_full(b) ((b)->len == (b)->size)
#define nix_flat_buf_length(b) ((b)->len)
#define nix_flat_buf_move(b, off) \
	bcopy((b)->buf + off, (b)->buf, (b)->len - (off))
#define nix_flat_buf_read(b, len) (b)->read += len, (b)->read %= (b)->size
#define nix_flat_buf_write(b, len) (b)->write += len, (b)->write %= (b)->size


#endif /* NIX_FLATBUF_H */
