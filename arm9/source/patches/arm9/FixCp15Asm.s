.cpu arm946e-s
.section "fixcp15", "ax"
.syntax unified
.arm

// to fix access to gba slot memory
.global fix_cp15_asm
.type fix_cp15_asm, %function
fix_cp15_asm:
    mrc p15, 0, r0, c2, c0, 0
    bic r0, r0, #(1 << 3)
    mcr p15, 0, r0, c2, c0, 0
    mrc p15, 0, r0, c3, c0, 0
    bic r0, r0, #(1 << 3)
    mcr p15, 0, r0, c3, c0, 0
    mrc p15, 0, r0, c5, c0, 2
    bic r0, r0, #(0xF << 12)
    orr r0, r0, #(3 << 12)
    mcr p15, 0, r0, c5, c0, 2
    bx lr

.balign 4

.pool

.end