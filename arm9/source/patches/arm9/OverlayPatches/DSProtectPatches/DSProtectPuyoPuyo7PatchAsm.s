.cpu arm946e-s
.syntax unified
.section "puyopuyo7patch", "ax"
.thumb

// Puyo Puyo 7 includes DS Protect 1.08 *five times*:
//
// overlay id | function mask | overlay offset | A1 offset | NotA1 offset
//     9      |    100110     |     0x21AC     |     -     |    0x2AD8
//    12      |    100101     |     0xC568     |   0xCE94  |      -
//    14      |    010101     |    0x13AB8     |  0x143E4  |      -
//    15      |    010110     |    0x16DF0     |     -     |   0x1771C
//    19      |    011010     |     0x17F8     |     -     |    0x2124
//
// Note that each overlay only contains either A1 or NotA1, and never both
// This means we only need to patch one offset at each load

.global puyopuyo7patch_entry
.type puyopuyo7patch_entry, %function
puyopuyo7patch_entry:
    push {r5, lr}

    ldm r5!, {r1, r2} // overlay_id, ram_start

    cmp r1, #9
    ldr r3, =0x2AD8
    beq do_patch

    cmp r1, #12
    ldr r3, =0xCE94
    beq do_patch

    cmp r1, #14
    ldr r3, =0x143E4
    beq do_patch

    cmp r1, #15
    ldr r3, =0x1771C
    beq do_patch

    cmp r1, #19
    bne continue_to_next
    ldr r3, =0x2124

do_patch:
    ldr r1, =0xFFFFFCE0 // magic number to deactivate 1.08 function queue
    str r1, [r2, r3]

continue_to_next:
    ldr r0, puyopuyo7patch_nextAddress
    pop {r5, pc}

.balign 4

.global puyopuyo7patch_nextAddress
puyopuyo7patch_nextAddress:
    .word 0

.pool

.end
