    .section .rst, "ax"
    .global rst00, rst08, rst10, rst18, rst20, rst28, rst30, rst38
    .type rst00, @function
    .type rst08, @function
    .type rst10, @function
    .type rst18, @function
    .type rst20, @function
    .type rst28, @function
    .type rst30, @function
    .type rst38, @function

    .balign 8
    .global _start
rst00:
    jp _start
    ret

.rept 7
    .balign 8
    rst 00h ; Stub
    ret
.endr
