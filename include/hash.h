/*
* Copyright (C) Qu. All right reserved.
*/


#ifndef NIX_HASH_H
#define NIX_HASH_H


unsigned int nix_rs_hash(int len, const char *str);
unsigned int nix_js_hash(int len, const char *str);
unsigned int nix_pjw_hash(int len, const char *str);
unsigned int nix_elf_hash(int len, const char *str);
unsigned int nix_bkdr_hash(int len, const char *str);
unsigned int nix_sdbm_hash(int len, const char *str);
unsigned int nix_djb_hash(int len, const char *str);
unsigned int nix_dek_hash(int len, const char *str);
unsigned int nix_bp_hash(int len, const char *str);
unsigned int nix_fnv_hash(int len, const char *str);


#endif /* NIX_HASH_H */
