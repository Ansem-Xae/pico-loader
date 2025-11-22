.cpu arm7tdmi
.section "patch_pokemondownloader7", "ax"
.syntax unified

.thumb
.global patch_pokemondownloader7_entry
.type patch_pokemondownloader7_entry, %function
patch_pokemondownloader7_entry:
    ldr r0,= 0x04000180 // REG_IPC_SYNC
    movs r1, #1
    strb r1, [r0, #1]

1:
    ldrb r7, [r0] // ipc sync
    cmp r7, #1
    bne 1b // while ipc sync from arm9 is not 1

    ldr r0,= 0x06000000
    ldr r0, [r0]
    bx r0

.balign 4

.pool
.end
