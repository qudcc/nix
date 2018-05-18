/*
 * Copyright (C) Qu. All right reserved.
 */


#ifndef NIX_ATOMIC_H
#define NIX_ATOMIC_H


#if (NIX_SMP) /* Symmetrical Multi-Processing */
#define NIX_SMP_LOCK "lock;"
#else
#define NIX_SMP_LOCK
#endif

typedef volatile intptr_t nix_atomic_t;

#if (__i386__ || __i386)
#define cmpxchg "cmpxchgl"
#define xadd "xaddl"
/* old as does not support "pause" opcode */
#define nix_cpu_pause() __asm__("pause")
#elif (__amd64__ || __amd64)
#define cmpxchg "cmpxchgq"
#define xadd "xaddq"
#define nix_cpu_pause() __asm__(".byte 0xf3, 0x90")
#else
#error system are not defined!
#endif

/*
 * "cmpxchgl r, [m]":
 *
 *     if (eax == [m]) {
 *         zf = 1;
 *         [m] = r;
 *     } else {
 *         zf = 0;
 *         eax = [m];
 *     }
 *
 * The "r" means the general register.
 * The "=a" and "a" are the %eax register.
 * Although we can return result in any register, we use "a" because it is
 * used in cmpxchgl anyway. The result is actually in %al but not in %eax,
 * however, as the code is inlined gcc can test %al as well as %eax,
 * and icc adds "movzbl %al, %eax" by itself.
 *
 * The "cc" means that flags were changed.
 */

static inline intptr_t
nix_atomic_cmp_set(nix_atomic_t *lock, intptr_t old, intptr_t set) {
    char res;

    __asm__ volatile (
        NIX_SMP_LOCK
        cmpxchg " %3, %1;"
        "sete %0;"
    : "=a"(res) : "m"(*lock), "a"(old), "r"(set) : "cc", "memory");

    return res;
}

/*
 * "xaddl r, [m]":
 *
 *     temp = [m];
 *     [m] += r;
 *     r = temp;
 *
 * The "+r" means the general register.
 * The "cc" means that flags were changed.
 */

static inline intptr_t
nix_atomic_fetch_add(nix_atomic_t *value, intptr_t add) {
#if !((__GNUC__ == 2 && __GNUC_MINOR__ <= 7) || (__INTEL_COMPILER >= 800))
/*
 * icc 8.1 and 9.0 compile broken code with -march=pentium4 option:
 * nix_atomic_fetch_add() always return the input "add" value,
 * so we use the gcc 2.7 version.
 *
 * icc 8.1 and 9.0 with -march=pentiumpro option or icc 7.1 compile
 * correct code.
 */
    __asm__ volatile (
        NIX_SMP_LOCK
        xadd " %0, %1;"
    : "+r"(add) : "m"(*value) : "cc", "memory");

    return add;
#else
/*
 * gcc 2.7 does not support "+r", so we have to use the fixed
 * %eax ("=a" and "a") and this adds two superfluous instructions in the end
 * of code, something like this: "mov %eax, %edx / mov %edx, %eax".
 */
    intptr_t old;

    __asm__ volatile (
        NIX_SMP_LOCK
        xadd " %2, %1;"
    : "=a"(old) : "m"(*value), "a"(add) : "cc", "memory");

    return old;
#endif
}

/*
 * on x86 the write operations go in a program order, so we need only
 * to disable the gcc reorder optimizations
 */

#define nix_memory_barrier() __asm__ volatile ("" ::: "memory")

#define nix_lock_try(lock) (!*(lock) && nix_atomic_cmp_set(lock, 0, -1))
#define nix_unlock(lock) *(lock) = 0

#define nix_spinlock(lock, value, spin, ncpu)  { \
    intptr_t  i, n; \
    for (; *lock || !nix_atomic_cmp_set(lock, 0, value);) { \
        if (1 < ncpu) { \
            /* spin=1024 */ \
            for (n = 1; n < spin; n <<= 1) { \
                /* n=1,2,4,8,16,32,64,128,256,512 */ \
                for (i = n; 0 < i--; nix_cpu_pause()); \
                /* try again */ \
                if (!*lock && nix_atomic_cmp_set(lock, 0, value)) goto _locked; \
            } \
        } \
        sched_yield(); /* usleep(1) */ \
    } \
} _locked:

typedef nix_atomic_t nix_mutex_t;
#define nix_shmtx_init(mtx) (*(mtx) = 0)
#define nix_shmtx_lock_try(mtx) (!*(mtx) && nix_atomic_cmp_set(mtx, 0, nix_pid))
#define nix_shmtx_lock(mtx) nix_spinlock(mtx, nix_pid, 1024)
#define nix_shmtx_unlock(mtx) nix_atomic_cmp_set(mtx, nix_pid, 0)


#endif /* NIX_ATOMIC_H */
