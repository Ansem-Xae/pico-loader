.cpu arm946e-s
.syntax unified
.section "pokemonbw2irappatch", "ax"
.thumb

.global pokemonbw2irappatch_entry
.type pokemonbw2irappatch_entry, %function
pokemonbw2irappatch_entry:
    push {r5,lr}

    ldmia r5!, {r1, r2} // id, ram address
    ldr r0,= 338 // target overlay id
    cmp r0, r1
    bne continue_to_next
    movs r0, #0
    adds r2, #0xEC
    strh r0, [r2]
    adds r2, #(0x1B8 - 0xEC)
    ldr r0,= 0x47702001
    str r0, [r2]
    movs r0, #1
    strb r0, [r2, #(0x1CA - 0x1B8)]

continue_to_next:
    ldr r0, pokemonbw2irappatch_nextAddress
    pop {r5,pc}

.balign 4

.global pokemonbw2irappatch_nextAddress
pokemonbw2irappatch_nextAddress:
    .word 0

.pool

.end
