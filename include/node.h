/*
 * Copyright (C) Qu. All right reserved.
 */


#ifndef NIX_NODE_H
#define NIX_NODE_H


#define nix_ownerof(node, type, field) \
        ((type *)((char *)(node) - offsetof(type, field)))

#define nix_countof(a)  (sizeof(a) / sizeof(*a))
#define nix_typeof      __typeof
#define nix_offsetof    offsetof


#endif /* NIX_NODE_H */
