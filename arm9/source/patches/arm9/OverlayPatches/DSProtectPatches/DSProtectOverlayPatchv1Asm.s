.cpu arm946e-s
.syntax unified
.section "dsprotectpatchv1_part1", "ax"

.thumb

.global dsprotectpatchv1_entry1
.type dsprotectpatchv1_entry1, %function
dsprotectpatchv1_entry1:
    push {r4-r7,lr}
    adr r0, dsprotectpatchv1_stub_protectb1_offset
    ldrh r4, [r0, #(dsprotectpatchv1_overlay_id - dsprotectpatchv1_stub_protectb1_offset)]
    ldmia r5!, {r1, r2} // id, ram address
    cmp r1, r4
    bne continue_to_next

    ldr r4, dsprotectpatchv1_base_offset
    adds r2, r4 // add base offset to overlay address

    ldrh r4, [r0, #(dsprotectpatchv1_nitro_static_init_offset - dsprotectpatchv1_stub_protectb1_offset)]
    ldr r3,= 0xE12FFF1E // bx lr
    str r3, [r4, r2]

    adr r4, protectbX_patch
    ldmia r4!, {r5,r6}

    adds r3, r0, #(dsprotectpatchv1_nitro_static_init_offset - dsprotectpatchv1_stub_protectb1_offset)
1:
    ldmia r4!, {r7}
    ldrh r1, [r0]
    adds r1, r2
    stmia r1!, {r5,r6,r7}
    adds r0, #2
    cmp r0, r3
    bne 1b

    ldr r7, dsprotectpatchv1_part2_address
    bx r7

continue_to_next:
    ldr r7, dsprotectpatchv1_part2_address
    adds r7, #(dsprotectpatchv1_continue_to_next - dsprotectpatchv1_entry2)
    bx r7

.global dsprotectpatchv1_stub_protectb1_offset
dsprotectpatchv1_stub_protectb1_offset:
    .short 0

.global dsprotectpatchv1_stub_protectb2_offset
dsprotectpatchv1_stub_protectb2_offset:
    .short 0

.global dsprotectpatchv1_stub_protectb3_offset
dsprotectpatchv1_stub_protectb3_offset:
    .short 0

.global dsprotectpatchv1_nitro_static_init_offset
dsprotectpatchv1_nitro_static_init_offset:
    .short 0

.global dsprotectpatchv1_overlay_id
dsprotectpatchv1_overlay_id:
    .short 0

.balign 4

.global dsprotectpatchv1_base_offset
dsprotectpatchv1_base_offset:
    .word 0

.global dsprotectpatchv1_part2_address
dsprotectpatchv1_part2_address:
    .word 0

.pool

.arm
protectbX_patch:
    ldr r0, dsprotectpatchv1_protectb1_return_value
    bx lr
.global dsprotectpatchv1_protectb1_return_value
dsprotectpatchv1_protectb1_return_value:
    .word 1830601
.global dsprotectpatchv1_protectb2_return_value
dsprotectpatchv1_protectb2_return_value:
    .word 1830203
.global dsprotectpatchv1_protectb3_return_value
dsprotectpatchv1_protectb3_return_value:
    .word 1828014

.section "dsprotectpatchv1_part2", "ax"

.thumb

.global dsprotectpatchv1_entry2
.type dsprotectpatchv1_entry2, %function
dsprotectpatchv1_entry2:
    adr r0, dsprotectpatchv1_stub_notprotectb1_offset
    adds r1, r0, #(dsprotectpatchv1_stub_notprotectb3_offset - dsprotectpatchv1_stub_notprotectb1_offset + 2)
    mov lr, r1
2:
    ldrh r1, [r0]
    adds r1, r2
    adr r3, notprotectbX_patch
    ldmia r3!, {r4,r5,r6,r7}
    stmia r1!, {r4,r5,r6,r7}
    ldmia r3!, {r4,r5,r6}
    stmia r1!, {r4,r5,r6}
    adds r0, #2
    cmp r0, lr
    bne 2b

    ldmia r3!, {r6,r7}

    adds r2, #0x18
    adr r0, dsprotectpatchv1_stub_notprotectb1_offset
    ldrh r1, [r0, #(dsprotectpatchv1_stub_notprotectb2_offset - dsprotectpatchv1_stub_notprotectb1_offset)]
    str r6, [r1, r2]
    ldrh r1, [r0, #(dsprotectpatchv1_stub_notprotectb3_offset - dsprotectpatchv1_stub_notprotectb1_offset)]
    str r7, [r1, r2]

dsprotectpatchv1_continue_to_next:
    ldr r0, dsprotectpatchv1_nextAddress
    pop {r4-r7,pc}

.global dsprotectpatchv1_stub_notprotectb1_offset
dsprotectpatchv1_stub_notprotectb1_offset:
    .short 0

.global dsprotectpatchv1_stub_notprotectb2_offset
dsprotectpatchv1_stub_notprotectb2_offset:
    .short 0

.global dsprotectpatchv1_stub_notprotectb3_offset
dsprotectpatchv1_stub_notprotectb3_offset:
    .short 0

.balign 4

.global dsprotectpatchv1_nextAddress
dsprotectpatchv1_nextAddress:
    .word 0

.pool

.arm
notprotectbX_patch:
    push {lr}
.global dsprotectpatchv1_moveCallback
dsprotectpatchv1_moveCallback:
    movs lr, r0
    movne r0, r1
    blxne lr
.global dsprotectpatchv1_loadReturnValue
dsprotectpatchv1_loadReturnValue:
    ldr r0, dsprotectpatchv1_notprotectb1_return_value
    pop {pc}
.global dsprotectpatchv1_notprotectb1_return_value
dsprotectpatchv1_notprotectb1_return_value:
    .word 1831551
.global dsprotectpatchv1_notprotectb2_return_value
dsprotectpatchv1_notprotectb2_return_value:
    .word 1830859
.global dsprotectpatchv1_notprotectb3_return_value
dsprotectpatchv1_notprotectb3_return_value:
    .word 1829648

.end