#ifndef  __LN_COMPILER_H__
#define  __LN_COMPILER_H__

/*------------------ RealView Compiler -----------------*/
#if defined(__CC_ARM)

#define ARMCC_V5

#define __ASM__             __asm
#define __VOLATILE__        volatile

#define __INLINE__          inline
#define __STATIC__          static

#if (__ARMCC_VERSION < 5060750)
  #ifndef __STATIC_INLINE__
  #define __STATIC_INLINE__   static
  #endif
#else
  #ifndef __STATIC_INLINE__
  #define __STATIC_INLINE__   static inline
  #endif
#endif

#define likely(x)           __builtin_expect(!!(x), 1)
#define unlikely(x)         __builtin_expect(!!(x), 0)
#define __UNUSED__          __attribute__((__unused__))
#define __USED__            __attribute__((__used__))
#define __PACKED__          __attribute__((packed))
#define __ALIGNED__(x)      __attribute__((aligned(x)))
#define __PURE__            __attribute__((__pure__))
#define __CONST__           __attribute__((__const__))
#define __NO_RETURN__       __attribute__((__noreturn__))
#define __WEAK__            __attribute__((weak))

/*------------------ ARM Compiler V6 -------------------*/
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)

#define ARMCC_V6

#define __ASM__             __asm
#define __VOLATILE__        volatile

#define __INLINE__          inline
#define __STATIC__          static
#ifndef __STATIC_INLINE__
  #define __STATIC_INLINE__ static inline
#endif

#define likely(x)           __builtin_expect(!!(x), 1)
#define unlikely(x)         __builtin_expect(!!(x), 0)
#define __UNUSED__          __attribute__((__unused__))
#define __USED__            __attribute__((__used__))
#define __PACKED__          __attribute__((packed))
#define __ALIGNED__(x)      __attribute__((aligned(x)))
#define __PURE__            __attribute__((__pure__))
#define __CONST__           __attribute__((__const__))
#define __NO_RETURN__       __attribute__((__noreturn__))
#define __NAKED__           __attribute__((naked))
#define __WEAK__            __attribute__((weak))

/*------------------ ICC Compiler ----------------------*/
#elif defined(__ICCARM__)  || defined(__ICC430__) // __IAR_SYSTEMS_ICC__

#if (__VER__ >= 8000000)
  #define __ICCARM_V8 1
#else
  #define __ICCARM_V8 0
#endif

#define __ASM__             __asm
#define __VOLATILE__        volatile
#define __INLINE__          inline
#define __STATIC__          static

#ifndef __STATIC_INLINE__
  #define __STATIC_INLINE__ static inline
#endif

#define likely(x)           (x)
#define unlikely(x)         (x)
#define __UNUSED__
#define __USED__

#if __ICCARM_V8
  #define __PACKED__        __attribute__((packed, aligned(1)))
#else
  /* Needs IAR language extensions */
  #define __PACKED__        __packed
#endif

#if __ICCARM_V8
  #define __ALIGNED__(x)    __attribute__((aligned(x)))
#elif (__VER__ >= 7080000)
  /* Needs IAR language extensions */
  #define __ALIGNED__(x)    __attribute__((aligned(x)))
#else
  #warning No compiler specific solution for __ALIGNED.__ALIGNED is ignored.
  #define __ALIGNED__(x)
#endif

#define __ALIGNED__(x)
#define __PURE__
#define __CONST__
#define __NO_RETURN__
#define __NAKED__
#define __WEAK__            __weak

/*------------------ GNU Compiler ----------------------*/
#elif defined(__GNUC__)

#define __ASM__             __asm
#define __VOLATILE__        volatile

#define __INLINE__          inline
#define __STATIC__          static
#ifndef __STATIC_INLINE__
  #define __STATIC_INLINE__ static inline
#endif

#define likely(x)           __builtin_expect(!!(x), 1)
#define unlikely(x)         __builtin_expect(!!(x), 0)
#define __UNUSED__          __attribute__((__unused__))
#define __USED__            __attribute__((__used__))
#define __PACKED__          __attribute__((packed))
#define __ALIGNED__(x)      __attribute__((aligned(x)))
#define __PURE__            __attribute__((__pure__))
#define __CONST__           __attribute__((__const__))
#define __NO_RETURN__       __attribute__((__noreturn__))
#define __NAKED__           __attribute__((naked))
#define __WEAK__            __attribute__((weak))

#endif

#endif /* __LN_COMPILER_H__ */

