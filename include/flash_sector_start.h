#ifndef __FLASH_SECTOR_START_H
#define __FLASH_SECTOR_START_H

#include "chaos/preprocessor/arithmetic.h"
#include "chaos/preprocessor/tuple/for_each.h"
#include "chaos/preprocessor/tuple/size.h"
#include "chaos/preprocessor/tuple/take.h"
#include "chaos/preprocessor/seq/to_tuple.h"
#include "chaos/preprocessor/comparison/greater.h"
#include "chaos/preprocessor/slot/slot.h"
#include "chaos/preprocessor/repetition/repeat.h"

#define PAGES_STM32F4XX \
    (4)(1),             \
        (1)(4),         \
        (7)(8),         \
        (4)(1),         \
        (1)(4),         \
        (7)(8)

#define PAGES_STM32F1XX (512)(1)

#define CHAOS_PP_VALUE (DFU_FLASH_END + 1 - DFU_FLASH_BASE) / FLASH_SIZE_DIVISOR
#include CHAOS_PP_ASSIGN_SLOT(1)

// IS_SECTOR_START_ADDRESS macros

#define SECTOR_START_COND(arg, n, offs, cnt, siz)           \
    CHAOS_PP_WHEN(CHAOS_PP_GREATER(CHAOS_PP_SLOT(1), offs)) \
    (CHAOS_PP_WHEN(n)(|| arg >= (DFU_FLASH_BASE + offs * FLASH_SIZE_DIVISOR) &&) arg <= (DFU_FLASH_BASE + (offs + cnt * siz) * FLASH_SIZE_DIVISOR) CHAOS_PP_WHEN(CHAOS_PP_DEC(siz))( && ((arg - (DFU_FLASH_BASE + (offs * FLASH_SIZE_DIVISOR))) & (siz * FLASH_SIZE_DIVISOR - 1)) == 0))

#define _CALC_OFFSET(x, y) CHAOS_PP_MUL(x, y), (0xchaos)(0xadd),                 /* (x * y + ) */
#define CALC_OFFSET(s, t) CHAOS_PP_EXPAND(_CALC_OFFSET CHAOS_PP_SEQ_TO_TUPLE(t)) /* (cnt) (siz) => (cnt, siz) */

#define ITERATE(s, n, tup, arg) CHAOS_PP_EXPAND(SECTOR_START_COND CHAOS_PP_SEQ_TO_TUPLE((arg)(n)(CHAOS_PP_MACHINE((, CHAOS_PP_EXPR(CHAOS_PP_TUPLE_FOR_EACH(CALC_OFFSET, CHAOS_PP_TUPLE_TAKE(n, tup))) 0, (0xchaos)(0xstop), )))CHAOS_PP_TUPLE_ELEM(1, n, tup)))

#define IS_SECTOR_START_ADDRESS(arg) ((arg & (FLASH_SIZE_DIVISOR - 1)) == 0 && (CHAOS_PP_EXPR(CHAOS_PP_REPEAT(CHAOS_PP_TUPLE_QUICK_SIZE((FLASH_PAGE_MAP)), ITERATE, (FLASH_PAGE_MAP), arg))))

// ADDRESS_TO_SECTOR macros

#define SECTOR_CALC(arg, n, offs, tcnt, siz) CHAOS_PP_WHEN(CHAOS_PP_GREATER(CHAOS_PP_SLOT(1), offs))(CHAOS_PP_WHEN(n)(arg >= offs ?) (arg - offs) / siz + tcnt CHAOS_PP_WHEN(n)(:))

#define _ADD_COUNT(x) x, (0xchaos)(0xadd),
#define ADD_COUNT(s, t) _ADD_COUNT(CHAOS_PP_SEQ_HEAD(t))

#define ITERATE_2(s, n, tup, arg, max) CHAOS_PP_EXPAND(SECTOR_CALC CHAOS_PP_SEQ_TO_TUPLE((arg)(CHAOS_PP_SUB(max, n))(CHAOS_PP_MACHINE((, CHAOS_PP_EXPR(CHAOS_PP_TUPLE_FOR_EACH(CALC_OFFSET, CHAOS_PP_TUPLE_TAKE(CHAOS_PP_SUB(max, n), tup))) 0, (0xchaos)(0xstop), )))(CHAOS_PP_MACHINE((, CHAOS_PP_EXPR(CHAOS_PP_TUPLE_FOR_EACH(ADD_COUNT, CHAOS_PP_TUPLE_TAKE(CHAOS_PP_SUB(max, n), tup))) 0, (0xchaos)(0xstop), )))CHAOS_PP_BINARY(1) CHAOS_PP_TUPLE_ELEM(1, CHAOS_PP_SUB(max, n), tup)))

#define ADDRESS_TO_SECTOR(arg) CHAOS_PP_EXPR(CHAOS_PP_REPEAT(CHAOS_PP_TUPLE_QUICK_SIZE((FLASH_PAGE_MAP)), ITERATE_2, (FLASH_PAGE_MAP), arg, CHAOS_PP_DEC(CHAOS_PP_TUPLE_QUICK_SIZE((FLASH_PAGE_MAP)))))

#endif // __FLASH_SECTOR_START_H