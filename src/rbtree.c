/*
 * Copyright (C) Qu. All right reserved.
 */


#include <sys/types.h>

#include <rbtree.h>


#define RBTREE_BLACK  0
#define RBTREE_RED   -1

static const intptr_t rbtree_nil_placehold = RBTREE_BLACK;
static nix_rbtree_node_t *const rbtree_nil
    = (nix_rbtree_node_t *const)&rbtree_nil_placehold;

#define RBTREE_NIL(node)  ((node) == rbtree_nil)

inline void
nix_rbtree_init(nix_rbtree_t *tree, nix_rbtree_predicate_pt predicate) {
    tree->root = rbtree_nil;
    tree->pred = predicate;
}

inline int
nix_rbtree_nil(const nix_rbtree_node_t *node) {
    return RBTREE_NIL(node);
}

nix_rbtree_node_t *
nix_rbtree_first(nix_rbtree_node_t *node) {
    if (!RBTREE_NIL(node))
        for (; !RBTREE_NIL(node->left); node = node->left);
    return node;
}

nix_rbtree_node_t *
nix_rbtree_last(nix_rbtree_node_t *node) {
    if (!RBTREE_NIL(node))
        for (; !RBTREE_NIL(node->right); node = node->right);
    return node;
}

nix_rbtree_node_t *
nix_rbtree_next(nix_rbtree_node_t *node) {
    /* If we have a right-hand child,
       go down and then left as far as we can. */
    if (!RBTREE_NIL(node->right)) return nix_rbtree_first(node->right);

    /* No right-hand children. Everything down and left is smaller than us,
       so any 'next' node must be in the general direction of our parent.
       Go up the tree; any time the ancestor is a right-hand child of its parent,
       keep going up. First time it's a left-hand child of its parent,
       said parent is our 'next' node. */
    for (; !RBTREE_NIL(node->parent) && node == node->parent->right;
        node = node->parent);
    return node->parent;
}

nix_rbtree_node_t *
nix_rbtree_prev(nix_rbtree_node_t *node) {
    /* If we have a left-hand child,
       go down and then right as far as we can. */
    if (!RBTREE_NIL(node->left)) return nix_rbtree_last(node->left);

    /* No left-hand children. Go up till we find an ancestor which
       is a right-hand child of its parent */
    for (; !RBTREE_NIL(node->parent) && node == node->parent->left;
        node = node->parent);
    return node->parent;
}

int
nix_rbtree_height(const nix_rbtree_node_t *node) {
    int  height;
    for (height = 1; !RBTREE_NIL(node->left); node = node->left)
        height += node->left->color;
    return height;
}

/*
 *     |               |
 *     x               y
 *    / \             / \
 *   a   y    -->    x   c
 *      / \         / \
 *     b   c       a   b
 */
static void
rbtree_left_rotate(nix_rbtree_node_t **root, nix_rbtree_node_t *node) {
    nix_rbtree_node_t  *right = node->right; /* y */
    nix_rbtree_node_t  *parent = node->parent;

    if (!RBTREE_NIL(node->right = right->left))
        right->left->parent = node; /* parent */
    right->left = node;

    if (!RBTREE_NIL(right->parent = parent)) {
        if (node == parent->left) parent->left = right;
        else parent->right = right;
    } else *root = right; /* root */
    node->parent = right;
}

/*
 *     |               |
 *     x               y
 *    / \             / \
 *   a   y    <--    x   c
 *      / \         / \
 *     b   c       a   b
 */
static void
rbtree_right_rotate(nix_rbtree_node_t **root, nix_rbtree_node_t *node) {
    nix_rbtree_node_t  *left = node->left; /* x */
    nix_rbtree_node_t  *parent = node->parent;

    if (!RBTREE_NIL(node->left = left->right))
        left->right->parent = node; /* parent */
    left->right = node;

    if (!RBTREE_NIL(left->parent = parent)) {
        if (node == parent->right) parent->right = left;
        else parent->left = left;
    } else *root = left; /* root */
    node->parent = left;
}

static void
rbtree_insert_color(nix_rbtree_node_t **root, nix_rbtree_node_t *node) {
    nix_rbtree_node_t  *parent, *grandpa;

    while (!RBTREE_NIL(parent = node->parent)
        && RBTREE_RED == parent->color) {
        grandpa = parent->parent;

        if (parent == grandpa->left) {
            nix_rbtree_node_t *uncle = grandpa->right;
            if (RBTREE_RED == uncle->color) { /* case 1 */
                uncle->color = RBTREE_BLACK;
                parent->color = RBTREE_BLACK;
                grandpa->color = RBTREE_RED;
                node = grandpa;
            } else {
                if (node == parent->right) { /* case 2 */
                    rbtree_left_rotate(root, parent);
                    parent = node;
                    node = parent->left;
                }
                /* case 3 */
                parent->color = RBTREE_BLACK; /* terminate */
                grandpa->color = RBTREE_RED;
                rbtree_right_rotate(root, grandpa);
            }
        } else {
            nix_rbtree_node_t *uncle = grandpa->left;
            if (RBTREE_RED == uncle->color) {
                uncle->color = RBTREE_BLACK;
                parent->color = RBTREE_BLACK;
                grandpa->color = RBTREE_RED;
                node = grandpa;
                continue;
            } else {
                if (node == parent->left) {
                    rbtree_right_rotate(root, parent);
                    parent = node;
                    node = parent->right;
                }
                parent->color = RBTREE_BLACK; /* terminate */
                grandpa->color = RBTREE_RED;
                rbtree_left_rotate(root, grandpa);
            }
        }
    }
    (*root)->color = RBTREE_BLACK;
}

static void
rbtree_erase_color(nix_rbtree_node_t **root,
    nix_rbtree_node_t *node, nix_rbtree_node_t *parent) {
    nix_rbtree_node_t *other;

    while (RBTREE_BLACK == node->color && node != *root) {
        if (node == parent->left) {
            other = parent->right;
            if (RBTREE_RED == other->color) {
                other->color = RBTREE_BLACK;
                parent->color = RBTREE_RED;
                rbtree_left_rotate(root, parent);
                other = parent->right; /* other->left */
            }
            if (RBTREE_BLACK == other->left->color
                && RBTREE_BLACK == other->right->color) {
                other->color = RBTREE_RED;
                node = parent;
                parent = node->parent;
            } else {
                if (RBTREE_BLACK == other->right->color) {
                    other->color = RBTREE_RED;
                    other->left->color = RBTREE_BLACK;
                    rbtree_right_rotate(root, other);
                    other = parent->right;
                }
                other->color = parent->color;
                parent->color = RBTREE_BLACK;
                other->right->color = RBTREE_BLACK;
                rbtree_left_rotate(root, parent);
                node = *root;
                break;
            }
        } else {
            other = parent->left;
            if (RBTREE_RED == other->color) {
                other->color = RBTREE_BLACK;
                parent->color = RBTREE_RED;
                rbtree_right_rotate(root, parent);
                other = parent->left;
            }
            if (RBTREE_BLACK == other->left->color
                && RBTREE_BLACK == other->right->color) {
                other->color = RBTREE_RED;
                node = parent;
                parent = node->parent;
            } else {
                if (RBTREE_BLACK == other->left->color) {
                    other->color = RBTREE_RED;
                    other->right->color = RBTREE_BLACK;
                    rbtree_left_rotate(root, other);
                    other = parent->left;
                }
                other->color = parent->color;
                parent->color = RBTREE_BLACK;
                other->left->color = RBTREE_BLACK;
                rbtree_right_rotate(root, parent);
                node = *root;
                break;
            }
        }
    }
    if (!RBTREE_NIL(node)) node->color = RBTREE_BLACK;
}

nix_rbtree_node_t *
nix_rbtree_insert(nix_rbtree_t *tree, intptr_t key,
    nix_rbtree_node_t *(*alloc)(intptr_t key)) {
    int                 n;
    nix_rbtree_node_t  *parent, **pp, *node;
    for (parent = rbtree_nil, pp = &tree->root; !RBTREE_NIL(*pp);) {
        parent = *pp;
        if ((n = tree->pred(parent, key)) < 0) pp = &parent->left;
        else if (0 < n) pp = &parent->right;
        else return *pp;
    }
    if (!(*pp = node = alloc(key))) return rbtree_nil;
    node->color = RBTREE_RED;
    node->parent = parent;
    node->left = node->right = rbtree_nil;
    rbtree_insert_color(&tree->root, node);
    return node;
}

nix_rbtree_node_t *
nix_rbtree_find(nix_rbtree_t *tree, intptr_t key) {
    int                 n;
    nix_rbtree_node_t  *parent, **node;
    for (parent = rbtree_nil, node = &tree->root; !RBTREE_NIL(*node);) {
        parent = *node;
        if ((n = tree->pred(parent, key)) < 0) node = &parent->left;
        else if (0 < n) node = &parent->right;
        else break;
    }
    return *node;
}

nix_rbtree_node_t ** /* upper_bound > for insert duplicate-value */
nix_rbtree_upper(nix_rbtree_t *tree, intptr_t key,
    nix_rbtree_node_t **parent, nix_rbtree_node_t **node) {
    nix_rbtree_node_t  *p, **pp;
    for (p = rbtree_nil, pp = &tree->root; !RBTREE_NIL(*pp);) {
        p = *pp;
        if (tree->pred(p, key) < 0) pp = &p->left;
        else pp = &p->right;
    }
    if (parent) *parent = p;
    if (node) {
        if (!RBTREE_NIL(p) && pp == &p->right)
            if (RBTREE_NIL(p = nix_rbtree_next(p)))
                p = nix_rbtree_first(tree->root);
        *node = p;
    }
    return pp;
}

nix_rbtree_node_t ** /* lower_bound >= */
nix_rbtree_ceil(nix_rbtree_t *tree, intptr_t key,
    nix_rbtree_node_t **parent, nix_rbtree_node_t **node) {
    nix_rbtree_node_t  *p, **pp;
    for (p = rbtree_nil, pp = &tree->root; !RBTREE_NIL(*pp);) {
        p = *pp;
        if (0 < tree->pred(p, key)) pp = &p->right;
        else pp = &p->left;
    }
    if (parent) *parent = p;
    if (node) {
        if (!RBTREE_NIL(p) && pp == &p->right)
            if (RBTREE_NIL(p = nix_rbtree_next(p)))
                p = nix_rbtree_first(tree->root);
        *node = p;
    }
    return pp;
}

void
nix_rbtree_erase(nix_rbtree_t *tree, const nix_rbtree_node_t *node) {
    nix_rbtree_node_t  *child, *parent;
    nix_rbtree_node_t  *left, *candidate;
    int                 color;

    if (RBTREE_NIL(node->left)) child = node->right;
    else if (RBTREE_NIL(node->right)) child = node->left;
    else {
        for (candidate = node->right; !RBTREE_NIL(left = candidate->left);
            candidate = left);

        color = candidate->color;
        child = candidate->right;

        if (RBTREE_NIL(node->parent)) tree->root = candidate;
        else { /* '|'- */
            if (node == node->parent->left) node->parent->left = candidate;
            else node->parent->right = candidate;
        }

        node->left->parent = candidate; /* '/'- */

        if (node == candidate->parent) parent = candidate;
        else {
            node->right->parent = candidate; /* '\'- */
            candidate->right = node->right; /* '\'+ */

            parent = candidate->parent;
            /* downline candidate */
            if (!RBTREE_NIL(parent->left = child)) /* delete */
                child->parent = parent;
        }

        /* replace node with candidate */
        candidate->parent = node->parent; /* '|'+ */
        candidate->color = node->color;
        candidate->left = node->left; /* '/'+ */

        goto color;
    }

    parent = node->parent;
    color = node->color;

    if (!RBTREE_NIL(child)) child->parent = parent;
    if (RBTREE_NIL(parent)) tree->root = child;
    else {
        if (node == parent->left) parent->left = child;
        else parent->right = child;
    }

color:
    if (RBTREE_BLACK == color)
        rbtree_erase_color(&tree->root, child, parent);
}
