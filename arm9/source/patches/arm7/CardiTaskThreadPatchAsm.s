.cpu arm7tdmi
.section "patch_carditaskthread", "ax"
.syntax unified
.thumb

.global __patch_carditaskthread_entry_sdk4
.type __patch_carditaskthread_entry_sdk4, %function
__patch_carditaskthread_entry_sdk4:
    push {r4,lr}
    beq end_success
    b __patch_carditaskthread_mov_cardicommon_to_r4

.global __patch_carditaskthread_entry
.type __patch_carditaskthread_entry, %function
__patch_carditaskthread_entry:
    push {r4,lr}
.global __patch_carditaskthread_mov_cardicommon_to_r4
__patch_carditaskthread_mov_cardicommon_to_r4:
    mov r4, r9
.global __patch_carditaskthread_mov_command_to_r0
__patch_carditaskthread_mov_command_to_r0:
    ldr r0, [r4, #4] // r0 = command

    cmp r0, #2
    beq identify_backup

    cmp r0, #6
    beq read_backup

    cmp r0, #7
    beq write_backup

    cmp r0, #8
    beq write_backup

    cmp r0, #9
    beq verify_backup

    // erase
    cmp r0, #10
    beq end_success
    cmp r0, #11
    beq end_success
    cmp r0, #12
    beq end_success
    cmp r0, #15
    beq end_success

end_fail:
    ldr r0, __patch_carditaskthread_failoffset
    b end

end_success:
    ldr r0, __patch_carditaskthread_successoffset
end:
    pop {r4}
    pop {r3}
    adds r3, r0
    bx r3

identify_backup:
    b end_success

read_backup:
    ldr r0, [r4]
    movs r1, #0
    str r1, [r0] // result
    ldr r1, [r0, #0xC] // src
    ldr r2, [r0, #0x10] // dst
    ldr r3, [r0, #0x14] // len
    cmp r3, #0
        beq end_success

    ldr r0, __patch_carditaskthread_readsave_asm_address
    bl blx_r0

    b end_success

write_backup:
    ldr r0, [r4]
    movs r1, #0
    str r1, [r0] // result
    ldr r1, [r0, #0xC] // src
    ldr r2, [r0, #0x10] // dst
    ldr r3, [r0, #0x14] // len
    cmp r3, #0
        beq end_success

    ldr r0, __patch_carditaskthread_writesave_asm_address
    bl blx_r0

    b end_success

verify_backup:
    ldr r0, [r4]
    movs r1, #0
    str r1, [r0] // result
    b end_success

blx_r0:
    bx r0

.balign 4

.global __patch_carditaskthread_failoffset
__patch_carditaskthread_failoffset:
    .word 0

.global __patch_carditaskthread_successoffset
__patch_carditaskthread_successoffset:
    .word 4

.global __patch_carditaskthread_readsave_asm_address
__patch_carditaskthread_readsave_asm_address:
    .word 0

.global __patch_carditaskthread_writesave_asm_address
__patch_carditaskthread_writesave_asm_address:
    .word 0

.pool

.end