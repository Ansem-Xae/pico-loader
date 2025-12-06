.cpu arm946e-s
.syntax unified
.section "pokemonirappatch", "ax"
.thumb

.global pokemonirappatch_entry
.type pokemonirappatch_entry, %function
pokemonirappatch_entry:
    push {r5,lr}

    ldmia r5!, {r1, r2} // overlay id, ram address
    ldr r0, pokemonirappatch_overlayId
    cmp r0, r1
    bne continue_to_next

    ldr r1, pokemonirappatch_offset
    movs r0, #1 // force return value of function to 1 instead of 0
    strb r0, [r2, r1]

continue_to_next:
    ldr r0, pokemonirappatch_nextAddress
    pop {r5,pc}

.balign 4

.global pokemonirappatch_overlayId
pokemonirappatch_overlayId:
    .word 0

.global pokemonirappatch_offset
pokemonirappatch_offset:
    .word 0

.global pokemonirappatch_nextAddress
pokemonirappatch_nextAddress:
    .word 0

.pool

.end
