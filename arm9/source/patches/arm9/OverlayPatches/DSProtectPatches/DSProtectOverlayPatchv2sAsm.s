.cpu arm946e-s
.section "dsprotectpatchv2s", "ax"
.syntax unified
.thumb

.global dsprotectpatchv2s_entry
.type dsprotectpatchv2s_entry, %function
dsprotectpatchv2s_entry:
    push {r4-r7,lr}

    ldmia r5!, {r0,r1,r2,r3,r4}

    ldr r7, dsprotectpatchv2s_overlay_id
    cmp r0, r7
    bne continue_to_next

    movs r3, #0
    movs r7, #0
    stmia r4!, {r3,r7}
    stmia r4!, {r3,r7}
    str r3, [r4]

    adr r0, stub_patch_code
    ldmia r0!, {r3,r4,r5,r6,r7}

    ldr r0, dsprotectpatchv2s_stub_instantdetect_offset
    adds r0, r1
    stmia r0!, {r3,r4,r5,r6} // code
.global dsprotectpatchv2s_store_checksum_fix
dsprotectpatchv2s_store_checksum_fix:
    str r7, [r0, #0] // checksum fix

continue_to_next:
    ldr r0, dsprotectpatchv2s_nextAddress
    pop {r4-r7,pc}
    
.balign 4

.global dsprotectpatchv2s_stub_instantdetect_offset
dsprotectpatchv2s_stub_instantdetect_offset:
    .word 0

.global dsprotectpatchv2s_overlay_id
dsprotectpatchv2s_overlay_id:
    .word 0

.global dsprotectpatchv2s_nextAddress
dsprotectpatchv2s_nextAddress:
    .word 0

.pool

.arm
stub_patch_code:
    movs r3, r0
    pusheq {r0,r1,lr}
    pushne {r1,r2,r3}
    pop {r0,r1,pc}

.global dsprotectpatchv2s_checksum_fix
dsprotectpatchv2s_checksum_fix:
    .word 0

.end