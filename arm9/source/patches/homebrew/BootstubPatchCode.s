.syntax unified
.section "patch_bootstub", "ax"
.cpu arm946e-s
.arm

.global patch_bootstub_arm9reboot
.type patch_bootstub_arm9reboot, %function
patch_bootstub_arm9reboot:
    // disable irqs
    ldr r0,= 0x04000208
    strb r0, [r0]

    // sync with arm7
    // make arm7 jump to arm7_after_arm9_sync
    ldr r0,= 0x02FFFE34
    adr r1, arm7_after_arm9_sync
    str r1, [r0]

    ldr r0,= 0x04000180
    ldr r1,= 0x0C04000C // LIBNDS_RESET_CODE
    str r1, [r0, #8]

    // wait for 1 from arm7
1:
    ldrb r1, [r0]
    cmp r1, #1
    bne 1b

    // send 1 to arm7
    ldr r1,= 0x100
    strh r1, [r0]

    // wait for 0 from arm7
1:
    ldrb r1, [r0]
    cmp r1, #0
    bne 1b

    // send 0 to arm7
    strh r1, [r0]

.type arm9_after_arm7_sync, %function
arm9_after_arm7_sync:
    // disable irqs
    ldr r0,= 0x04000208
    strb r0, [r0]

    // map vram ABCD to lcdc
    mov r0, #0x04000000
    ldr r1,= 0x80808080
    str r1, [r0, #0x240]

    ldr r0,= 0x04000204
    ldrh r1, [r0]
    bic r1, r1, #0x880 // map slot 1 and 2 to arm9
    strh r1, [r0]

    ldr r9, patch_bootstub_arm9reboot_readSdSectors_address
    ldr sp,= 0x02000000 + (16 * 1024) // the platform code needs a valid stack pointer

    // load pico loader arm9
    adr r5, patch_bootstub_arm9reboot_loader_info
    ldmia r5!, {r4,r6,r7} // clusterShift, database, clusterMap[0]
    mov r7, #0x06800000
    mov r11, pc
    b loadData

    // load pico loader arm7
    adr r5, patch_bootstub_arm9reboot_loader_info + 52
    ldr r7,= 0x06840000
    mov r11, pc
    b loadData

    ldr r7,= 0x06840000
    adr r5, patch_bootstub_arm9reboot_loader_info
    ldrh r0, [r5, #2] // loader_info_t::picoLoaderBootDrive
    strh r0, [r7, #8] // pload_header7_t::bootDrive

    ldr r0, patch_bootstub_arm9reboot_dldi_address
    str r0, [r7, #4]

    // set loadParams->romPath
    ldr r0, patch_bootstub_arm9reboot_launcher_path
    mov r1, #256
    add r3, r7, #0xC
1:
    ldr r2, [r0], #4
    str r2, [r3], #4
    subs r1, r1, #4
    bne 1b

    // map vram CD to arm7
    ldr r0,= 0x04000240
    ldr r1,= 0x8A82
    strh r1, [r0, #2]

    // start arm7 by sending 3
    ldr r0,= 0x04000180
    mov r1, #0x300
    strh r1, [r0]

    // invalidate icache
    mov r0, #0
    mcr p15, 0, r0, c7, c5, 0

    mov pc, #0x06800000

loadData_loop:
    sub r3, r3, #2
    add r0, r6, r3, lsl r4 // start sector
    mov r1, r7 // dst
    mov r2, r2, lsl r4 // sector count
    add r7, r7, r2, lsl #9

    blx r9 // read sectors

loadData:
    ldmia r5!, {r2, r3} // ncl, startSector
    cmp r2, #0
    bne loadData_loop
    bx r11

.balign 4

.pool

.global patch_bootstub_arm9reboot_readSdSectors_address
patch_bootstub_arm9reboot_readSdSectors_address:
    .word 0

.global patch_bootstub_arm9reboot_dldi_address
patch_bootstub_arm9reboot_dldi_address:
    .word 0

.global patch_bootstub_arm9reboot_launcher_path
patch_bootstub_arm9reboot_launcher_path:
    .word 0

.global patch_bootstub_arm9reboot_loader_info
patch_bootstub_arm9reboot_loader_info:
    .space 0x70 // sizeof(loader_info_t)

.balign 4

.cpu arm7tdmi

.global patch_bootstub_arm7reboot
.type patch_bootstub_arm7reboot, %function
patch_bootstub_arm7reboot:
    // disable irqs
    mov r0, #0x04000000
    strb r0, [r0, #0x208]

    bl copy_arm7_code

    // sync with arm9
    // make arm9 jump to arm9_after_arm7_sync
    ldr r0,= 0x02FFFE24
    adr r1, arm9_after_arm7_sync
    str r1, [r0]

    ldr r0,= 0x04000180
    ldr r1,= 0x0C04000C // LIBNDS_RESET_CODE
    str r1, [r0, #8]

    mov pc, #0x03800000 // arm7 private ram

.type arm7_after_arm9_sync, %function
arm7_after_arm9_sync:
    // disable irqs
    mov r0, #0x04000000
    strb r0, [r0, #0x208]

    bl copy_arm7_code
    mov r0, #0x03800000
    add r0, r0, #(arm7_iwram_region_after_arm9_sync - arm7_iwram_region)
    bx r0

copy_arm7_code:
    adr r0, arm7_iwram_region
    adr r1, arm7_iwram_region_end
    mov r2, #0x03800000 // arm7 private ram
1:
    cmp r0, r1
    ldrne r3, [r0], #4
    strne r3, [r2], #4
    bne 1b
2:
    bx lr

.balign 4
.pool

// Code loaded to arm7 private wram to avoid main memory contention
arm7_iwram_region:
    // wait for 1 from arm9
1:
    ldrb r1, [r0]
    cmp r1, #1
    bne 1b

    // send 1 to arm9
    ldr r1,= 0x100
    strh r1, [r0]

    // wait for 0 from arm9
1:
    ldrb r1, [r0]
    cmp r1, #0
    bne 1b

    // send 0 to arm9
    strh r1, [r0]

arm7_iwram_region_after_arm9_sync:
    ldr r0,= 0x04000180

    // wait for 3 from arm9
1:
    ldrb r1, [r0]
    cmp r1, #3
    bne 1b

    // boot pico loader arm7
    mov r0, #0x06000000
    ldr r0, [r0]
    bx r0

.balign 4
.pool
.balign 4

arm7_iwram_region_end:

.balign 4

.end