@ print 5 aseterixes
.global main
main:
    @ Stack the return address (lr) in addition to a dummy register (ip) to
    @ keep the stack 8-byte aligned.
    push    {ip, lr}

    mov     r0, #42 @ 42 = asterisk
    bl      putchar
    bl      putchar
    bl      putchar
    bl      putchar
    bl      putchar

    mov	r0, #10 @ newline
    bl	putchar

    @ Exit from 'main'. This is like 'return 0' in C.
    mov     r0, #0    @ Return 0.

    @ Pop the dummy ip to reverse our alignment fix, and pop the original lr
    @ value directly into pc — the Program Counter — to return.
    pop     {ip, pc}
