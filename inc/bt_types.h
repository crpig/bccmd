#ifndef __BT_TYPES_H__
#define __BT_TYPES_H__

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdarg.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#undef  FALSE
#define FALSE (0)

#undef  TRUE
#define TRUE (1)


/* Unsigned fixed width types */
#ifndef __cplusplus
typedef uint8_t bool;
#endif
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint32_t uint24;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;

//typedef unsigned int size_t;

typedef struct
{
    uint24 lap;   /*!< Lower Address Part 00..23 */
    uint8  uap;   /*!< upper Address Part 24..31 */
    uint16 nap;   /*!< Non-significant    32..47 */
} bdaddr;

#ifndef __cplusplus
typedef bool    BOOL;
#endif

#ifdef __cplusplus
}
#endif

#endif /* __BT_TYPES_H__ */