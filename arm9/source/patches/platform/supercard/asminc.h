#pragma once

.macro BEGIN_ARM_FUNCTION name
    .global \name
    .arm
    .type \name, %function
    .balign 4
\name:
.endm

.macro BEGIN_THUMB_FUNCTION name
    .global \name
    .thumb
    .type \name, %function
    .balign 2
\name:
.endm

.macro INTERWORK name
 \name:
     bx r4
.balign 4
.pool
.endm

#ifdef LITE

.macro CALL func, interworkLabel
    ldr r4, \func\()_\interworkLabel\()Lite_address
    bl \interworkLabel
.endm

.macro INTERWORK_FUNCTION func, interworkLabel
.global \func\()_\interworkLabel\()Lite_address
\func\()_\interworkLabel\()Lite_address:
    .word 0
.endm

#else

.macro CALL func, interworkLabel
    ldr r4, \func\()_\interworkLabel\()_address
    bl \interworkLabel
.endm

.macro INTERWORK_FUNCTION func, interworkLabel
.global \func\()_\interworkLabel\()_address
\func\()_\interworkLabel\()_address:
    .word 0
.endm

#endif
