.cpu arm946e-s
.syntax unified
.section "pokemonbw1irappatch", "ax"
.thumb

.global pokemonbw1irappatch_entry
.type pokemonbw1irappatch_entry, %function
pokemonbw1irappatch_entry:
    push {r5,lr}

    ldmia r5!, {r1, r2} // id, ram address
    ldr r0,= 231 // target overlay id
    cmp r0, r1
    bne continue_to_next
    ldr r1,= 0x1CA // patch offset in overlay
    movs r0, #1 // force return value of function to 1 instead of 0
    adds r2, r1
    strb r0, [r2]

continue_to_next:
    ldr r0, pokemonbw1irappatch_nextAddress
    pop {r5,pc}

.balign 4

.global pokemonbw1irappatch_nextAddress
pokemonbw1irappatch_nextAddress:
    .word 0

.pool

.end
