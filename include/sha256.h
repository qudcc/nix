/*
 * Copyright (C) Qu. All right reserved.
 */


#ifndef NIX_SHA256_H
#define NIX_SHA256_H


#define NIX_SHA256_DIGEST_LEN  32 /* 256/8 */

typedef char nix_sha256_digest_t[NIX_SHA256_DIGEST_LEN];

typedef struct {
    unsigned char  data[64];
    unsigned int   data_len;
    unsigned int   bit_len[2];
    unsigned int   state[8];
} nix_sha256_ctx_t;

void nix_sha256_init(nix_sha256_ctx_t *ctx);
void nix_sha256_update(nix_sha256_ctx_t *ctx,
    size_t len, unsigned char data[]);
void nix_sha256_final(nix_sha256_ctx_t *ctx, nix_sha256_digest_t digest);

#define nix_sha256_hash(len, buf, digest) ({ \
    nix_sha256_ctx_t  ctx; \
    nix_sha256_init(&ctx); \
    nix_sha256_update(&ctx, len, buf); \
    nix_sha256_final(&ctx, digest); \
})


#endif /* NIX_SHA256_H */
