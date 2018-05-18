/*
 * Copyright (C) Qu. All right reserved.
 */


#include <sys/types.h>

#include <splaytree.h>


inline void nix_splay_tree_init(nix_splay_tree_t *tree,
    int (*pred)(const nix_splay_tree_node_t *, intptr_t)) {
    tree->root = NULL;
    tree->pred = pred;
}

/*
 zig right rotate zag left rotate
      t   zig(x)   x
    x   C   =>   A   t
   A B    zag(y)    B C
 zig-zig zag-zag
         z    zig-zig(x)    x
      y     D     =>     A     y
    x   C     zag-zag(z)     B   z
   A B                          C D
 zig-zag zag-zig
      z       zig-zag(x)    x
   A     y       =>      z     y
       x   D            A B   C D
      B C
 */

/*
 * 伸展树的自顶向下伸展 (top-down splay)
 * 自顶向下伸展操作将伸展树分为三部分：
 * 左树：包含所有已经知道比待查节点 X小的节点。
 * 右树：包含所有已经知道比待查节点 X大的节点。
 * 中树：包含所有其它节点。
 * 在中树自根向下进行节点查找（每次向下比较两个节点），根据查找情况将中树中的节点移动
 * （此处的移动是指将节点和中树的连接断开，而将节点连接到左或右树的适当位置。）
 * 到左树或右树（如有必要则会先对中树进行旋转再进行节点移动）。
 * 初始状态时，左树和右树都为空，而中树为整个原伸展树。随着查找的进行，左树和右树会
 * 因节点的逐渐移入变大，中树会因节点的逐渐移出变小。最后查找结束（找到或遇到空节点）
 * 时组合左中右树是伸展树自顶向下伸展方法的最终结果。
*/
/*
 孩子即为要查找的点,只需要一次连接操作即可
  L          M           R       L           M           R
             a               =>              b       a
          b      c                        d     e      c
       d     e  f g                      h i   j k    f g
      h i   j k
 孙子为要查找的点,且左右孩子一致.需要首先旋转父亲和祖父节点,然后连接操作.
  L          M           R       L           M           R
             a               =>              d           b
         b       c                          h i            a
      d     e   f g                                     e     c
     h i   j k                                         j k   f g
 孙子为要查找的点,且左右孩子不一致.需要两次连接操作. 
  L          M           R       L           M           R
             a               =>     b        e        a
         b       c                d         j k         c
      d     e   f g              h i                   f g
     h i   j k
 合并
  L          M           R                   M
             x               =>              x
         A       B                       L        R
                                           A    B
*/
static nix_splay_tree_node_t *
splay_tree_splay(nix_splay_tree_node_t *t, intptr_t key,
    int (*pred)(const nix_splay_tree_node_t *, intptr_t)) {
    int                    n;
    nix_splay_tree_node_t  sentinel, *left, *right, *x;

    if (!t) return t; /* empty tree */

    sentinel.left = sentinel.right = NULL; /* sentinel */
    for (left = right = &sentinel; (n = pred(t, key));) {
        if (n < 0) {
            if (!t->left) break;
            if (pred(t->left, key) < 0) { /* grandson */
                /* zig rotate right x(right), t(left) */
                x = t->left; t->left = x->right; x->right = t; t = x;
                if (!t->left) break;
            }
            right->left = t; right = t; t = t->left; /* link to right */
        } else /*if (0 < n)*/ {
            if (!t->right) break;
            if (0 < pred(t->right, key)) { /* grandson */
                /* zag rotate left x(left), t(right) */
                x = t->right; t->right = x->left; x->left = t; t = x;
                if (!t->right) break;
            }
            left->right = t; left = t; t = t->right; /* link to left */
        }
    }
    /* assemble */
    left->right = t->left; right->left = t->right;
    t->left = sentinel.right; t->right = sentinel.left;
    return t;
}

int
nix_splay_tree_insert(nix_splay_tree_t *t, intptr_t key,
    nix_splay_tree_node_t *(*alloc)(intptr_t key)) {
    int                     n = 0;
    nix_splay_tree_node_t  *node;
    if (t->root) {
        t->root = splay_tree_splay(t->root, key, t->pred);
        /* it's already in the tree */
        if (!(n = t->pred(t->root, key))) return 0;
    }
    if (!(node = alloc(key))) return -1;
    if (n < 0) node->left = t->root->left, t->root->left = NULL,
        node->right = t->root;
    else if (0 < n) node->right = t->root->right, t->root->right = NULL,
        node->left = t->root;
    else node->left = node->right = NULL;
    return t->root = node, 0;
}

nix_splay_tree_node_t *
nix_splay_tree_erase(nix_splay_tree_t *t, intptr_t key) {
    nix_splay_tree_node_t  *node;
    if (t->root) {
        t->root = splay_tree_splay(t->root, key, t->pred);
        if (!t->pred(t->root, key)) { /* found it */
            node = t->root;
            if (node->left) {
                t->root = splay_tree_splay(node->left, key, t->pred);
                /* i is max */
                t->root->right = node->right;
            } else t->root = node->right;
            return node;
        }
    }
    return NULL; /* It wasn't there */
}

nix_splay_tree_node_t *
nix_splay_tree_find(nix_splay_tree_t *t, intptr_t key) {
    if (t->root) {
        t->root = splay_tree_splay(t->root, key, t->pred);
        if (t->pred(t->root, key)) return NULL;
    }
    return t->root;
}

void
nix_splay_tree_each(const nix_splay_tree_node_t *node,
    void (*cb)(const nix_splay_tree_node_t *node)) {
    if (node) {
        nix_splay_tree_each(node->left, cb);
        cb(node);
        nix_splay_tree_each(node->right, cb);
    }
}
