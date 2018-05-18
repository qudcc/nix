/*
 * Copyright (C) Qu. All right reserved.
 */


#ifndef NIX_ALGORITHM_H
#define NIX_ALGORITHM_H


#define int32_cmp(lhs, rhs)  ((lhs) - (rhs))
#define int64_cmp(lhs, rhs)  ((rhs) < (lhs)) - ((lhs) < (rhs))

#define nix_for_each(len, ptr, cb) ({ int _; \
    for (_ = 0; _ < (len); ++_) cb; \
_; })

#define nix_for_each_rev(len, ptr, cb) ({ int _; \
    for (_ = (len); 0 < _--;) cb; \
_; })

#define nix_find_first_of(len, ptr, pred) ({ int _;\
    for (_ = 0; _ < (len) && !(pred); ++_); \
_ == (len) ? -1 : _; })

#define nix_find_last_of(len, ptr, pred) ({ int _;\
    for (_ = (len); 0 < _-- && !(pred);); \
_; })

#define nix_count(len, ptr, pred) ({ int _, _0 = 0; \
    for (_ = (len); 0 < _--;) if (!(pred)) ++_0; \
_0; })

#define nix_sum(len, ptr, pred) ({ __typeof(*ptr) _0 = {}; int _;\
    for (_ = (len); 0 < _--;) if (!(pred)) _0 += ptr[_]; \
_0; })

#define nix_avg(len, ptr, pred) (nix_sum(len, ptr, pred) / (len))

#define nix_quick_sort(len, ptr, pred) ({ \
    int  _i, _j, _n, _top, _size[len]; __typeof(*ptr) _t, *_p, *_stack[len]; \
    /* first one, _i is 0 */ \
    for (_top = 0, _size[_top] = (len), _stack[_top++] = (ptr); 0 < _top;) { \
        for (_j = _n = _size[--_top], _p = _stack[_top], _t = *_p, _i = 0; \
            _i < _j;) { \
            /* right to left, find one less than _t */ \
            for (;_i < --_j && pred(_t, _p[_j]) < 0;); \
            if (_i < _j) { /* find */ \
                for (_p[_i] = _p[_j]; ++_i < _j && pred(_p[_i], _t) < 0;); \
                if (_i < _j) _p[_j] = _p[_i]; /* find, so _j is _i */ \
            } \
        } \
        _p[_i] = _t; /* inplace _i=_j */ \
        if (0 < _i) _size[_top] = _i, _stack[_top++] = _p; \
        /* left less than _p */ \
        if (0 < (_size[_top] = _n - ++_i)) \
            _stack[_top++] = _p + _i; /* right greater than _p */ \
    } \
})

#define nix_heap_sort(len, ptr, pred) ({ __typeof(*ptr)  _; \
    int  _i, _j, _k, _m, _n; \
    /* make heap, all sub binary tree is satisfied */ \
    for (_n = (len), _m = _n >> 1; 0 < _m--; /* not leaf, sub tree */ \
        _ = (ptr)[_k=_m], heap_adjust(ptr, pred)); \
    /* adjust */ \
    for (; 0 < --_n; _ = (ptr)[_n], /* save the last to adjust */ \
        (ptr)[_n] = (ptr)[_k=0], /* first one to sort list */ \
        heap_adjust(ptr, pred)); /* from the first */ \
})
#define heap_adjust(ptr, pred) ({ \
    for (; _j = (_k+1) << 1, (_i = _j - 1) < _n; \
        (ptr)[_k] = (ptr)[_i], _k = _i) { \
        /* greater of the two children _i and _j */ \
        if (_j < _n && pred((ptr)[_i], (ptr)[_j]) < 0) _i = _j; \
        /* greater than sentinel ? */ \
        if (pred((ptr)[_i], _) < 0) break; /* satisfy */ \
    } \
    ptr[_k] = _; /* inplace */ \
})

#define nix_binary_search(len, ptr, pred) ({ int  _, _i, _j, _k, _n; \
    __typeof(*ptr)  *_p = ptr; \
    for (_j = 0, _k = (len); 0 < _k;) { \
        _i = _k >> 1; _ = _i + _j; \
        if ((_n = (pred)) < 0) _k = _i; \
        else if (0 < _n) _p += ++_i, _k -= _i, _j += _i; \
        else break; \
    } \
0 < _k ? _ : -1; })

#define nix_lower_bound(len, ptr, pred) ({ int  _, _i, _j, _k; \
    __typeof(*ptr)  *_p = ptr; \
    for (_j = 0, _k = (len); 0 < _k;) { \
        _i = _k >> 1; _ = _i + _j; \
        if (0 < (pred)) _p += ++_i, _k -= _i, _j += _i; \
        else _k = _i; \
    } \
_j; })
#define nix_upper_bound(len, ptr, pred) ({ int  _, _i, _j, _k; \
    __typeof(*ptr)  *_p = ptr; \
    for (_j = 0, _k = (len); 0 < _k;) { \
        _i = _k >> 1; _ = _i + _j; \
        if ((pred) < 0) _k = _i; \
        else _p += ++_i, _k -= _i, _j += _i; \
    } \
_j; })


#endif /* NIX_ALGORITHM_H */
