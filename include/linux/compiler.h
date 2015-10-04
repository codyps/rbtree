#ifndef RBTREE_COMPILER_H_
#define RBTREE_COMPILER_H_

#include <stdint.h>

#ifndef __always_inline
# define __always_inline inline __attribute__((always_inline))
#endif

#ifndef __force
# define __force
#endif

/* nop because I'm not bothering to properly check it */
#define compiletime_assert_atomic_type(t)

#define barrier() __asm__ __volatile__("" : : : "memory")

static __always_inline void __write_once_size(volatile void *p, void *res, int size)
{
	switch (size) {
	case 1: *(volatile uint8_t *)p = *(uint8_t *)res; break;
	case 2: *(volatile uint16_t *)p = *(uint16_t *)res; break;
	case 4: *(volatile uint32_t *)p = *(uint32_t *)res; break;
	case 8: *(volatile uint64_t *)p = *(uint64_t *)res; break;
	default:
		barrier();
		__builtin_memcpy((void *)p, (const void *)res, size);
		barrier();
	}
}

#define WRITE_ONCE(x, val) \
({							\
	union { typeof(x) __val; char __c[1]; } __u =	\
		{ .__val = (__force typeof(x)) (val) }; \
	__write_once_size(&(x), __u.__c, sizeof(x));	\
	__u.__val;					\
})

#endif
