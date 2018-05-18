/*
 * Copyright (C) Qu. All right reserved.
 */


#ifndef NIX_BITWISE_H
#define NIX_BITWISE_H


/* Bitwise Tricks */

/* Most Right Bit */
#define NIX_MRB_RESET(x)  ((x) & ((x) - 1))
/* 0101 1110 => 0101 1100 */
/* is 2 ^ , 2,4,8... */
#define NIX_MRB_SET(x)    ((x) | ((x) + 1))
/* 0101 0111 => 0101 1111 */

/* Most Right Bit */
#define NIX_MRB_REV_SET(x)    (-(x) & ((x) + 1))
/* 0101 0111 => 0000 1000 */
#define NIX_MRB_REV_RESET(x)  (-(x) | ((x) - 1))
/* 0101 1000 => 1111 0111 */

/* Most Right Bit */
#define NIX_MRB_REV_GET(x)    ((x) & -(x))
/* 0101 1000 => 0000 1000 */
#define NIX_MRB_REV_GET_1(x)  ((x) ^ ((x) - 1))
/* 0101 1000 => 0000 1111 */
#define NIX_MRB_REV_GET_0(x)  ((x) ^ ((x) + 1))
/* 0101 0111 => 0000 1111 */
#define NIX_MRB_RESET_EX(x)   ((((x) & -(x)) + (x)) & (x))
/* 0101 1000 => 0100 0000 */
/* is 2 ^ i - 2 ^ j */

/* Most Right Tail */
#define NIX_MRT_RESET(x)  ((x) & ((x) + 1))
/* 0101 0111 => 0101 0000 */
/* is 2 ^ -1, 1,3,7... */
#define NIX_MRT_SET(x)    ((x) | ((x) - 1))
/* 0101 1000 => 0101 1111 */

/* Most Right Tail */
#define NIX_MRT_REV_SET(x)    (-(x) & ((x) - 1))
/* 0101 1000 => 0000 0111 */
#define NIX_MRT_REV_RESET(x)  (-(x) | ((x) + 1))
/* 0101 0111 => 1111 1000 */


#define NIX_BIT_INT_MAX ((unsigned int)-1 >> 1)
#define NIX_BIT_INT_MIN (BO_INT_MAX + 1)
#define NIX_BIT_LONG_MAX ((unsigned long)-1 >> 1)
#define NIX_BIT_LONG_MIN (BO_LONG_MAX + 1)
#define NIX_BIT_SHORT_MAX ((unsigned short)-1 >> 1)
#define NIX_BIT_SHORT_MIN (-1 << 15)
#define NIX_BIT_BYTE_MAX ((unsigned char)-1 >> 1)
#define NIX_BIT_BYTE_MIN (-1 << 7)

#define NIX_BIT_GET(m, n) (((m) >> (n - 1)) & 1)
#define NIX_BIT_SET(m, n) ((m) | (1 << ((n) - 1)))
#define NIX_BIT_RESET(m, n) ((m) & ~(1 << ((n) - 1)))

#define NIX_BIT_MUL2(n) ((n) << 1)
#define NIX_BIT_DIV2(n) ((n) >> 1)
#define NIX_BIT_MUL2_N(m, n) ((m) << (n))
#define NIX_BIT_DIV2_N(m, n) ((m) >> (n))

#define NIX_BIT_IS_ODD(n) ((n) & 1)
#define NIX_BIT_IS_SIGN_SAME(m, n) (((m) ^ (n)) > 0)
#define NIX_BIT_IS_SIGN_DIFF(m, n) (((m) ^ (n)) < 0)
#define NIX_BIT_IS_FACTORIAL2(n) ((n) & ((n) - 1))
#define NIX_BIT_SIGN(x) (((x) >> 31) | (-(x) >> 31))
/* #define NIX_BIT_SIGN(x) ((0 < x) - (x < 0)) */
#define NIX_BIT_XCHG(x, a, b)  ((x) ^ (a) ^ (b))
/* x = x = a ? b : a; */
#define NIX_BIT_SWAP_FRAG(x, k, m) ((x))
/* */

#define NIX_BIT_SWAP(m, n) (m) ^= (n) ^= (m) ^= (n)

#define NIX_BIT_MAX(m, n) ((m) ^ (((m) ^ (n)) & -((m) < (n))))
#define NIX_BIT_MIN(m, n) ((n) ^ (((m) ^ (n)) & -((m) < (n))))
#define NIX_BIT_AVG(m, n) (((m) & (n)) + (((m) ^ (n)) >> 1))
#define NIX_BIT_ABS(m) (((m) ^ -((m) < 0)) + ((m) < 0))
/* difference or zero */
#define NIX_BIT_DOZ(m) ((m) & -(0 < (m)))
#define NIX_BIT_DOZ2(m,n) ((m - n) & -(n < m)))

#define NIX_BIT_FIRST_SET(n) ((n) & -(n))
#define NIX_BIT_FIRST_UNSET(n) (~(n) & ((n) + 1))
#define NIX_BIT_POP_COUNT(m, n) n = (m), \
	n -= (n >> 1) & 0x55555555, \
	n = (n & 0x33333333) + ((n >> 2) & 0x33333333), \
	n = (n + (n >> 4)) & 0x0F0F0F0F, \
	n += n >> 8, \
	n += n >> 16, \
	n &= 0x3f
#define NIX_BIT_POP_PARITY(m, n) n = (m), /* 奇偶性传递 */ \
	n ^= n >> 1, /* parity of every bit pair now in bits 0, 2, 4, ... */ \
	n ^= n >> 2, /* parity of every bit pair now in bits 0, 4, 8, ... */ \
	n ^= n >> 4, /* parity of every bit pair now in bits 0, 8, 16, ... */ \
	n ^= n >> 8, /* parity of every bit pair now in bits 0, 16 */ \
	n ^= n >> 16, /* parity of every bit pair now in bits 0 */ \
	n &= 1
#define NIX_BIT_CEIL_FACTORIAL2(m, n) n = (m) - 1, \
	n |= n >> 1, \
	n |= n >> 2, \
	n |= n >> 4, \
	n |= n >> 8, \
	n |= n >> 16, \
	++n
#define NIX_BIT_FLOOR_FACTORIAL2(m, n) n = (m), \
	n |= n >> 1, \
	n |= n >> 2, \
	n |= n >> 4, \
	n |= n >> 8, \
	n |= n >> 16, \
	++n, n >>= 1
#define NIX_BIT_SWAP_BITS(m, n) \
	n = ((m) & 0x55555555) << 1 | ((m) & 0xAAAAAAAA) >> 1, \
	n = (n & 0x33333333) << 2 | (n & 0xCCCCCCCC) >> 2, \
	n = (n & 0x0F0F0F0F) << 4 | (n & 0xF0F0F0F0) >> 4, \
	n = (n & 0x00FF00FF) << 8 | (n & 0xFF00FF00) >> 8, \
	n = (n & 0x0000FFFF) << 16 | (n & 0xFFFF0000) >> 16
/*
 * 二进制码->格雷码（编码）
 * 从最右边一位起，依次将每一位与左边一位异或(XOR)，
 * 作为对应格雷码该位的值，最左边一位不变(相当于左边是0)
 */
#define NIX_BIT_BINARY_GRAY(n) ((n) ^ ((n) >> 1))
/*
 * 格雷码-〉二进制码（解码）
 * 从左边第二位起，将每位与左边一位解码后的值异或，
 * 作为该位解码后的值（最左边一位依然不变）.
 * abcdefgh -> a(a^b)(a^b^c)(a^b^c^d)...(a^b^c^d^e^f^g^h)
 * a(a^b)(b^c)(c^d)...(f^g)(g^h)
 * a(a^b)(a^b^c)(a^b^c^d)(b^c^d^e)...(e^f^g^h)
 * a(a^b)(a^b^c)(a^b^c^d)(a^b^c^d^e)...(a^b^c^d^e^f^g^h)
 */
#define NIX_BIT_GRAY_BINARY(m, n) n = (m),\
	n ^= n >> 1, \
	n ^= n >> 2, \
	n ^= n >> 4, \
	n ^= n >> 8, \
	n ^= n >> 16
/*
 * 九连环问题就是格雷码的应用
 * 九连环的操作步数就是格雷码1...1到0...0的过程
 * 也就是格雷码对应的二进制值
 */

#define NIX_BIT_NEXT_COMBINATION(m, n, t) \
	t = m & -m, /* first 1bit, eg. 10110 -> t=00010 */ \
	n = m + t, /* eg. 10110 -> n=11000 */ \
	n |= ((n ^ m) >> 2) / t /* n is next */

int nix_bit_ctz8(unsigned char n);
int nix_bit_ctz32(unsigned int n);
int nix_bit_ctz64(unsigned long n);


#endif /* NIX_BITWISE_H */
