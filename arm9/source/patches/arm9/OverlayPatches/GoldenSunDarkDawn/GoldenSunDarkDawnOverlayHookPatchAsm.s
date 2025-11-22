.cpu arm946e-s
.section "gsddoverlayhookpatch", "ax"
.syntax unified
.thumb

.global gsddoverlayhookpatch_entry
.type gsddoverlayhookpatch_entry, %function
gsddoverlayhookpatch_entry:
    ldr r0, gsddoverlayhookpatch_hookFuncAddress
1:
    blx r0
    cmp r0, #0
    bne 1b

    ldr r3, gsddoverlayhookpatch_returnAddress
    ldr r1, [r5, #8]
    movs r0, #1
    lsls r0, r0, #14 // r0 = 0x4000
    cmp r1, r0
    bx r3

.balign 4

.global gsddoverlayhookpatch_hookFuncAddress
gsddoverlayhookpatch_hookFuncAddress:
    .word 0

.global gsddoverlayhookpatch_returnAddress
gsddoverlayhookpatch_returnAddress:
    .word 0

.pool

.end