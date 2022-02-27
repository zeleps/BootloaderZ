#ifndef __DFU_DESC_STRING_H
#define __DFU_DESC_STRING_H

#include "chaos/preprocessor/arithmetic.h"
#include "chaos/preprocessor/tuple/for_each.h"
#include "chaos/preprocessor/tuple/size.h"
#include "chaos/preprocessor/tuple/take.h"
#include "chaos/preprocessor/seq/to_tuple.h"
#include "chaos/preprocessor/comparison/min.h"
#include "chaos/preprocessor/slot/slot.h"
#include "chaos/preprocessor/repetition/repeat.h"
#include "chaos/preprocessor/facilities/indirect.h"
#include "chaos/preprocessor/control/branch.h"

#define PAGES_STM32F4XX \
    (4)(1),             \
        (1)(4),         \
        (7)(8),         \
        (4)(1),         \
        (1)(4),         \
        (7)(8)

// breakdown required to conform to the 2 digit sector count
#define PAGES_STM32F1XX \
    (99)(1),            \
        (99)(1),        \
        (99)(1),        \
        (99)(1),        \
        (99)(1),        \
        (17)(1)

#ifndef DFU_FLASH_BASE_STRING

    #if DFU_FLASH_BASE != 0x08000000
        #error "Flash base is not 0x08000000. Please define DFU_FLASH_BASE_STRING in Configuration.h."
    #endif
    
    #define DFU_FLASH_BASE_STRING "0x08000000"

#endif

#define CHAOS_PP_VALUE (DFU_FLASH_END + 1 - DFU_FLASH_BASE) / FLASH_SIZE_DIVISOR
#include CHAOS_PP_ASSIGN_SLOT(1)

#define CHAOS_PP_VALUE (DFU_USER_CODE_ADDRESS - DFU_FLASH_BASE) / FLASH_SIZE_DIVISOR
#include CHAOS_PP_ASSIGN_SLOT(2)

#define CHAOS_PP_VALUE FLASH_SIZE_DIVISOR / 1024
#include CHAOS_PP_ASSIGN_SLOT(3)

#define _PAD_1(x) 0##x
#define _PAD_2(x) 00##x

#define PAD_2(x) CHAOS_PP_IIF(CHAOS_PP_GREATER(x, 9))(x, CHAOS_PP_INDIRECT(x)(_PAD_1))
#define PAD_3(x) CHAOS_PP_BRANCH(0xIIF)(CHAOS_PP_GREATER(x, 99))(x)(0xELIIF)(CHAOS_PP_GREATER(x, 9))(CHAOS_PP_INDIRECT(x)(_PAD_1))(0xELSE)(CHAOS_PP_INDIRECT(x)(_PAD_2))(0xENDIF)

#define CREATE_DESCRIPTOR(com, cnt, siz, acc) CHAOS_PP_IF(cnt)(CHAOS_PP_WHEN(com)(",") CHAOS_PP_STRINGIZE(PAD_2(cnt)) "*" CHAOS_PP_STRINGIZE(PAD_3(CHAOS_PP_MUL(siz, CHAOS_PP_SLOT(3)))) acc, CHAOS_PP_EMPTY())

#define VCNTxSIZ(offs, siz, cnt) CHAOS_PP_MIN(CHAOS_PP_SUB(CHAOS_PP_SLOT(1), offs), CHAOS_PP_MUL(cnt, siz)) /* MIN(end - offs, cnt * siz) */

#define PROCESS_PAGE(n, offs, cnt, siz)                                                                                                                         \
    CREATE_DESCRIPTOR(n, CHAOS_PP_DIV(CHAOS_PP_SUB(CHAOS_PP_SLOT(2), offs), siz), siz, "Ka")                                         /* (uoffs - offs) / siz */ \
    CREATE_DESCRIPTOR(1, CHAOS_PP_DIV(CHAOS_PP_SUB(VCNTxSIZ(offs, siz, cnt), CHAOS_PP_SUB(CHAOS_PP_SLOT(2), offs)), siz), siz, "Kg") /* (vcnt*siz - (uoffs - offs)) / siz */

#define _CALC_OFFSET(x, y) CHAOS_PP_MUL(x, y), (0xchaos)(0xadd),                 /* (x * y + ) */
#define CALC_OFFSET(s, t) CHAOS_PP_EXPAND(_CALC_OFFSET CHAOS_PP_SEQ_TO_TUPLE(t)) /* (cnt) (siz) => (cnt, siz) */

#define MACRO(s, n, tup) CHAOS_PP_EXPAND(PROCESS_PAGE CHAOS_PP_SEQ_TO_TUPLE((n) (CHAOS_PP_MACHINE((, CHAOS_PP_EXPR(CHAOS_PP_TUPLE_FOR_EACH(CALC_OFFSET, CHAOS_PP_TUPLE_TAKE(n, tup))) 0, (0xchaos)(0xstop), ))) CHAOS_PP_TUPLE_ELEM(1, n, tup))) /* (for ((cnt,siz) in PAGES[i < n]) => SUM(cnt*siz)) (cnt) (siz) => PROCESS_PAGE(offs, cnt, siz) */

#define DFU_FLASH_DESCRIPTOR "@Internal Flash /" DFU_FLASH_BASE_STRING "/" CHAOS_PP_EXPR(CHAOS_PP_REPEAT(CHAOS_PP_TUPLE_QUICK_SIZE((FLASH_PAGE_MAP)), MACRO, (FLASH_PAGE_MAP))) /* foreach ((cnt, siz) in PAGES) => MACRO(n, (cnt, siz)) */

#endif // __DFU_DESC_STRING_H