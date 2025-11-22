.cpu arm946e-s
.section "dsprotectpatchv2", "ax"
.syntax unified
.thumb

.global dsprotectpatchv2_entry
.type dsprotectpatchv2_entry, %function
dsprotectpatchv2_entry:
    push {r4-r7,lr}

    ldmia r5!, {r0,r1,r2}
    adds r3, r2, r1 // bss address

    adr r2, dsprotectpatchv2_stub_protectb1_offset
    ldrh r7, [r2, #(dsprotectpatchv2_overlay_id - dsprotectpatchv2_stub_protectb1_offset)]
    cmp r0, r7
    bne continue_to_next

    ldr r7, [r2, #(dsprotectpatchv2_base_offset - dsprotectpatchv2_stub_protectb1_offset)]
    adds r1, r7 // add base offset to overlay address

    ldrh r0, [r2, #(dsprotectpatchv2_am_init_offset - dsprotectpatchv2_stub_protectb1_offset)]
    ldr r7,= 0xE12FFF1E // bx lr
    str r7, [r1, r0]

    adr r0, stub_patch_code
    ldmia r0!, {r4,r5,r6,r7}

    subs r1, #4 // write from 4 bytes before the actual function address
    adds r0, r2, #(dsprotectpatchv2_stub_protectb4_offset - dsprotectpatchv2_stub_protectb1_offset)
    mov lr, r0
patch_loop:
    ldrh r0, [r2]
    adds r2, #2
    adds r0, r1
    stmia r0!, {r3,r4,r5,r6} // bss address + code
.global dsprotectpatchv2_store_checksum_fix
dsprotectpatchv2_store_checksum_fix:
    str r7, [r0, #0] // checksum fix
    cmp r2, lr
    ble patch_loop

continue_to_next:
    ldr r0, dsprotectpatchv2_nextAddress
    pop {r4-r7,pc}
    
.balign 4

.global dsprotectpatchv2_stub_protectb1_offset
dsprotectpatchv2_stub_protectb1_offset:
    .short 0

.global dsprotectpatchv2_stub_protectb2_offset
dsprotectpatchv2_stub_protectb2_offset:
    .short 0

.global dsprotectpatchv2_stub_protectb3_offset
dsprotectpatchv2_stub_protectb3_offset:
    .short 0

.global dsprotectpatchv2_stub_protectb4_offset
dsprotectpatchv2_stub_protectb4_offset:
    .short 0

.global dsprotectpatchv2_am_init_offset
dsprotectpatchv2_am_init_offset:
    .short 0

.global dsprotectpatchv2_overlay_id
dsprotectpatchv2_overlay_id:
    .short 0

.global dsprotectpatchv2_base_offset
dsprotectpatchv2_base_offset:
    .word 0

.global dsprotectpatchv2_nextAddress
dsprotectpatchv2_nextAddress:
    .word 0

.pool

.arm
stub_patch_code:
    ldr r12, (. - 4)
    ldr r2, [r12], #4 
    ldr pc, [r12, r2, lsl #2]

.global dsprotectpatchv2_checksum_fix
dsprotectpatchv2_checksum_fix:
    .word 0

.end