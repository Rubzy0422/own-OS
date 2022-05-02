

.set IRQ_BASE, 0x20

.section .text

.extern _ZN8kernelos21hardwarecommunication16InterruptManager15HandleInterruptEhj


.macro HandleException num

.global _ZN8kernelos21hardwarecommunication16InterruptManager19HandleException\num\()Ev
_ZN8kernelos21hardwarecommunication16InterruptManager19HandleException\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm


.macro HandleInterruptRequest num
.global _ZN8kernelos21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN8kernelos21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    jmp int_bottom
.endm

HandleInterruptRequest 0x00 # ignore
HandleInterruptRequest 0x01 # keyboard
HandleInterruptRequest 0x0C # mouse
int_bottom:

    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    push (interruptnumber)
    call _ZN8kernelos21hardwarecommunication16InterruptManager15HandleInterruptEhj
    # add %esp, 6
    mov %eax, %esp 

    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

.global _ZN8kernelos21hardwarecommunication16InterruptManager15InterruptIgnoreEv
_ZN8kernelos21hardwarecommunication16InterruptManager15InterruptIgnoreEv:

    iret


.data
    interruptnumber: .byte 0
