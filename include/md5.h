/*
 * Copyright (C) Qu. All right reserved.
 */


#ifndef NIX_MD5_H
#define NIX_MD5_H


#define NIX_MD5_DIGEST_LEN 16

typedef char nix_md5_digest_t[NIX_MD5_DIGEST_LEN];

typedef struct {
    unsigned int   state[4]; /* state (ABCD) */
    unsigned long  count; /* number of bits, modulo 2^64 (lsb first) */
    unsigned int   buffer[NIX_MD5_DIGEST_LEN]; /* input buffer */
} nix_md5_t;

/* MD5 initialization. Begins an MD5 operation, initialize the context. */
void nix_md5_init(nix_md5_t *md5);

/* MD5 block update operation. Continues an MD5 message-digest operation,
  processing another message block, and updating the context. */
void nix_md5_update(nix_md5_t *md5, size_t len, const char *buf);

/* MD5 finalization. Ends an MD5 message-digest operation, writing the
  the message digest and zeroizing the context. */
void nix_md5_final(nix_md5_t *md5, nix_md5_digest_t digest);

void nix_md5_hash(size_t len, const char *buf, nix_md5_digest_t digest);


#endif /* NIX_MD5_H */
