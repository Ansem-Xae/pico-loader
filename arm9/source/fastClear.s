.section ".itcm", "ax"
.arm

// r0: dst
// r1: size (multiple of 32)
.global fastClear
.type fastClear, %function
fastClear:
    push {r4-r8,lr}
    mov r2, #0
    mov r3, #0
    mov r4, #0
    mov r5, #0
    mov r6, #0
    mov r7, #0
    mov r8, #0
    mov lr, #0
1:
    subs r1, r1, #32
    stmgeia r0!, {r2-r8,lr}
    bgt 1b
    pop {r4-r8,pc}
