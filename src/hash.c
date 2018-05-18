/*
 * Copyright (C) Qu. All right reserved.
 */


#include <stdio.h>


unsigned int
nix_rs_hash(int len, const char *str) {
    unsigned int  b = 378551;
    unsigned int  a = 63689;
    unsigned int  hash;
    for (hash = 0; 0 < len--; hash *= a, hash += *str++, a *= b);
    return hash;
}

unsigned int
nix_js_hash(int len, const char *str) {
    unsigned int  hash;
    for (hash = 1315423911; 0 < len--;)
        hash ^= ((hash << 5) + *str++ + (hash >> 2));
    return hash;
}

unsigned int
nix_pjw_hash(int len, const char *str) {
    const unsigned int  bits_in_unsigned_int = (unsigned int)
        (sizeof(unsigned int) * 8);
    const unsigned int  three_quarters = (unsigned int)
        ((bits_in_unsigned_int  * 3) / 4);
    const unsigned int  one_eighth = (unsigned int)
        (bits_in_unsigned_int / 8);
    const unsigned int  high_bits = (unsigned int)
        (0xFFFFFFFF) << (bits_in_unsigned_int - one_eighth);
    unsigned int        hash, test;

    for (hash = 0; 0 < len--;) {
        hash = (hash << one_eighth) + *str++;
        if ((test = hash & high_bits))
            hash = (hash ^ (test >> three_quarters)) & ~high_bits;
    }

    return hash;
}

unsigned int
nix_elf_hash(int len, const char *str) {
    unsigned int  hash, test;

    for (hash = 0; 0 < len--;) {
        hash <<= 4;
        hash += *str++;
        if ((test = hash & 0xF0000000L)) hash ^= (test >> 24);
        hash &= ~test;
    }

    return hash;
}

unsigned int
nix_bkdr_hash(int len, const char *str) {
    const unsigned int  seed = 131; /* 31 131 1313 13131 131313 etc.. */
    unsigned int        hash;
    for (hash = 0; 0 < len--; hash *= seed, hash += *str++);
    return hash;
}

unsigned int
nix_sdbm_hash(int len, const char *str) {
    unsigned int  hash;
    for (hash = 0; 0 < len--;)
        hash = *str++ + (hash << 6) + (hash << 16) - hash;
    return hash;
}


unsigned int
nix_djb_hash(int len, const char *str) {
    unsigned int  hash;
    for (hash = 5381; 0 < len--; hash = ((hash << 5) + hash) + *str++);
    return hash;
}


unsigned int
nix_dek_hash(int len, const char *str) {
    unsigned int  hash;
    for (hash = len; 0 < len--; hash = ((hash << 5) ^ (hash >> 27)) ^ *str++);
    return hash;
}


unsigned int
nix_bp_hash(int len, const char *str) {
    unsigned int  hash;
    for (hash = 0; 0 < len--; hash = hash << 7 ^ *str++);
    return hash;
}


unsigned int
nix_fnv_hash(int len, const char *str) {
    const unsigned int  fnv_prime = 0x811C9DC5;
    unsigned int        hash;
    for (hash = 0; 0 < len--; hash *= fnv_prime, hash ^= *str++);
    return hash;
}
