/**
 * Useful macros
 * 
 * Based on Jhnet C preprocessor magic, copied from Marlin 3D Printer Firmware macros.h
 */

#ifndef __MACROS_H
#define __MACROS_H

// Macros to make a string from a macro
#define STRINGIFY_(M) #M
#define STRINGIFY(M) STRINGIFY_(M)

#define STRCAT(a, b) #a #b

// Macros for bit masks
#undef _BV
#define _BV(n) (1 << (n))
#define TEST(n, b) (!!((n)&_BV(b)))
#define SET_BIT_TO(N, B, TF) \
    do                       \
    {                        \
        if (TF)              \
            SBI(N, B);       \
        else                 \
            CBI(N, B);       \
    } while (0)
#ifndef SBI
#define SBI(A, B) (A |= _BV(B))
#endif
#ifndef CBI
#define CBI(A, B) (A &= ~_BV(B))
#endif
#define TBI(N, B) (N ^= _BV(B))
#define _BV32(b) (1UL << (b))
#define TEST32(n, b) !!((n)&_BV32(b))
#define SBI32(n, b) (n |= _BV32(b))
#define CBI32(n, b) (n &= ~_BV32(b))
#define TBI32(N, B) (N ^= _BV32(B))

#define SIGN(a) ({__typeof__(a) _a = (a); (_a>0)-(_a<0); })

// Use NUM_ARGS(__VA_ARGS__) to get the number of variadic arguments
#define _NUM_ARGS(_, n, m, l, k, j, i, h, g, f, e, d, c, b, a, Z, Y, X, W, V, U, T, S, R, Q, P, O, N, M, L, K, J, I, H, G, F, E, D, C, B, A, OUT, ...) OUT
#define NUM_ARGS(V...) _NUM_ARGS(0, V, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

// Macros to chain up to 40 conditions
#define _DO_1(W, C, A) (_##W##_1(A))
#define _DO_2(W, C, A, B) (_##W##_1(A) C _##W##_1(B))
#define _DO_3(W, C, A, V...) (_##W##_1(A) C _DO_2(W, C, V))
#define _DO_4(W, C, A, V...) (_##W##_1(A) C _DO_3(W, C, V))
#define _DO_5(W, C, A, V...) (_##W##_1(A) C _DO_4(W, C, V))
#define _DO_6(W, C, A, V...) (_##W##_1(A) C _DO_5(W, C, V))
#define _DO_7(W, C, A, V...) (_##W##_1(A) C _DO_6(W, C, V))
#define _DO_8(W, C, A, V...) (_##W##_1(A) C _DO_7(W, C, V))
#define _DO_9(W, C, A, V...) (_##W##_1(A) C _DO_8(W, C, V))
#define _DO_10(W, C, A, V...) (_##W##_1(A) C _DO_9(W, C, V))
#define _DO_11(W, C, A, V...) (_##W##_1(A) C _DO_10(W, C, V))
#define _DO_12(W, C, A, V...) (_##W##_1(A) C _DO_11(W, C, V))
#define _DO_13(W, C, A, V...) (_##W##_1(A) C _DO_12(W, C, V))
#define _DO_14(W, C, A, V...) (_##W##_1(A) C _DO_13(W, C, V))
#define _DO_15(W, C, A, V...) (_##W##_1(A) C _DO_14(W, C, V))
#define _DO_16(W, C, A, V...) (_##W##_1(A) C _DO_15(W, C, V))
#define _DO_17(W, C, A, V...) (_##W##_1(A) C _DO_16(W, C, V))
#define _DO_18(W, C, A, V...) (_##W##_1(A) C _DO_17(W, C, V))
#define _DO_19(W, C, A, V...) (_##W##_1(A) C _DO_18(W, C, V))
#define _DO_20(W, C, A, V...) (_##W##_1(A) C _DO_19(W, C, V))
#define _DO_21(W, C, A, V...) (_##W##_1(A) C _DO_20(W, C, V))
#define _DO_22(W, C, A, V...) (_##W##_1(A) C _DO_21(W, C, V))
#define _DO_23(W, C, A, V...) (_##W##_1(A) C _DO_22(W, C, V))
#define _DO_24(W, C, A, V...) (_##W##_1(A) C _DO_23(W, C, V))
#define _DO_25(W, C, A, V...) (_##W##_1(A) C _DO_24(W, C, V))
#define _DO_26(W, C, A, V...) (_##W##_1(A) C _DO_25(W, C, V))
#define _DO_27(W, C, A, V...) (_##W##_1(A) C _DO_26(W, C, V))
#define _DO_28(W, C, A, V...) (_##W##_1(A) C _DO_27(W, C, V))
#define _DO_29(W, C, A, V...) (_##W##_1(A) C _DO_28(W, C, V))
#define _DO_30(W, C, A, V...) (_##W##_1(A) C _DO_29(W, C, V))
#define _DO_31(W, C, A, V...) (_##W##_1(A) C _DO_30(W, C, V))
#define _DO_32(W, C, A, V...) (_##W##_1(A) C _DO_31(W, C, V))
#define _DO_33(W, C, A, V...) (_##W##_1(A) C _DO_32(W, C, V))
#define _DO_34(W, C, A, V...) (_##W##_1(A) C _DO_33(W, C, V))
#define _DO_35(W, C, A, V...) (_##W##_1(A) C _DO_34(W, C, V))
#define _DO_36(W, C, A, V...) (_##W##_1(A) C _DO_35(W, C, V))
#define _DO_37(W, C, A, V...) (_##W##_1(A) C _DO_36(W, C, V))
#define _DO_38(W, C, A, V...) (_##W##_1(A) C _DO_37(W, C, V))
#define _DO_39(W, C, A, V...) (_##W##_1(A) C _DO_38(W, C, V))
#define _DO_40(W, C, A, V...) (_##W##_1(A) C _DO_39(W, C, V))
#define __DO_N(W, C, N, V...) _DO_##N(W, C, V)
#define _DO_N(W, C, N, V...) __DO_N(W, C, N, V)
#define DO(W, C, V...) (_DO_N(W, C, NUM_ARGS(V), V))

// Macros to support option testing
#define _CAT(a, V...) a##V
#define CAT(a, V...) _CAT(a, V)

#define _ISENA_ ~, 1
#define _ISENA_1 ~, 1
#define _ISENA_0x1 ~, 1
#define _ISENA_true ~, 1
#define _ISENA(V...) IS_PROBE(V)

#define IS_PROBE(V...) SECOND(V, 0) // Get the second item passed, or 0
#define PROBE() ~, 1                // Second item will be 1 if this is passed
#define _NOT_0 PROBE()
#define NOT(x) IS_PROBE(_CAT(_NOT_, x)) // NOT('0') gets '1'. Anything else gets '0'.
#define _BOOL(x) NOT(NOT(x))            // NOT('0') gets '0'. Anything else gets '1'.

#define _ENA_1(O) _ISENA(CAT(_IS, CAT(ENA_, O)))
#define _DIS_1(O) NOT(_ENA_1(O))
#define ENABLED(V...) DO(ENA, &&, V)
#define DISABLED(V...) DO(DIS, &&, V)
#define COUNT_ENABLED(V...) DO(ENA, +, V)

#define TERN(O, A, B) _TERN(_ENA_1(O), B, A) // OPTION ? 'A' : 'B'
#define TERN0(O, A) _TERN(_ENA_1(O), 0, A)   // OPTION ? 'A' : '0'
#define TERN1(O, A) _TERN(_ENA_1(O), 1, A)   // OPTION ? 'A' : '1'
#define TERN_(O, A) _TERN(_ENA_1(O), , A)    // OPTION ? 'A' : '<nul>'
#define _TERN(E, V...) __TERN(_CAT(T_, E), V)    // Prepend 'T_' to get 'T_0' or 'T_1'
#define __TERN(T, V...) ___TERN(_CAT(_NO, T), V) // Prepend '_NO' to get '_NOT_0' or '_NOT_1'
#define ___TERN(P, V...) THIRD(P, V)             // If first argument has a comma, A. Else B.

#define ANY(V...) !DISABLED(V)
#define NONE(V...) DISABLED(V)
#define ALL(V...) ENABLED(V)
#define BOTH(V1, V2) ALL(V1, V2)
#define EITHER(V1, V2) ANY(V1, V2)
#define MANY(V...) (COUNT_ENABLED(V) > 1)

//
// Primitives supporting precompiler REPEAT
//
#define FIRST(a, ...) a
#define SECOND(a, b, ...) b
#define THIRD(a, b, c, ...) c

// Defer expansion
#define EMPTY()
#define DEFER(M) M EMPTY()
#define DEFER2(M) M EMPTY EMPTY()()
#define DEFER3(M) M EMPTY EMPTY EMPTY()()()
#define DEFER4(M) M EMPTY EMPTY EMPTY EMPTY()()()()

// Force define expansion
#define EVAL(V...) EVAL16(V)
#define EVAL1024(V...) EVAL512(EVAL512(V))
#define EVAL512(V...) EVAL256(EVAL256(V))
#define EVAL256(V...) EVAL128(EVAL128(V))
#define EVAL128(V...) EVAL64(EVAL64(V))
#define EVAL64(V...) EVAL32(EVAL32(V))
#define EVAL32(V...) EVAL16(EVAL16(V))
#define EVAL16(V...) EVAL8(EVAL8(V))
#define EVAL8(V...) EVAL4(EVAL4(V))
#define EVAL4(V...) EVAL2(EVAL2(V))
#define EVAL2(V...) EVAL1(EVAL1(V))
#define EVAL1(V...) V

// MIN/MAX macros

#define __MIN_N(N, V...) MIN_##N(V)
#define _MIN_N(N, V...) __MIN_N(N, V)
#define _MIN_N_REF() _MIN_N
#define _MIN(V...) EVAL(_MIN_N(TWO_ARGS(V), V))
#define MIN_2(a, b) ((a) < (b) ? (a) : (b))
#define MIN_3(a, V...) MIN_2(a, DEFER2(_MIN_N_REF)()(TWO_ARGS(V), V))

#define __MAX_N(N, V...) MAX_##N(V)
#define _MAX_N(N, V...) __MAX_N(N, V)
#define _MAX_N_REF() _MAX_N
#define _MAX(V...) EVAL(_MAX_N(TWO_ARGS(V), V))
#define MAX_2(a, b) ((a) > (b) ? (a) : (b))
#define MAX_3(a, V...) MAX_2(a, DEFER2(_MAX_N_REF)()(TWO_ARGS(V), V))

#endif // __MACROS_H