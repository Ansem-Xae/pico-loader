.section ".itcm", "ax"
.arm

// r0: iteration count
.global waitByLoop
.type waitByLoop, %function
waitByLoop:
    subs r0, r0, #1
    bgt waitByLoop
    bx lr
