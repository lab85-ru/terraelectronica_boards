.set FIQ_STACK_SIZE, 128
.set IRQ_STACK_SIZE, 512
.set SVR_STACK_SIZE, 128
.set ABT_STACK_SIZE, 128
.set UND_STACK_SIZE, 128
.set USR_STACK_SIZE, 4096


.set MODE_USER,         0x10
.set MODE_FIQ,          0x11
.set MODE_IRQ,          0x12
.set MODE_SUPERVISOR,   0x13
.set MODE_ABORT,        0x17
.set MODE_UNDEFINED,    0x1B
.set MODE_SYSTEM,       0x1F

.equ PSR_I_BIT, 0x80
.equ PSR_F_BIT, 0x40

.section .stack, "w"
.code 32
.align 4

.space  (USR_STACK_SIZE + 3) & ~3
.space  (FIQ_STACK_SIZE + 3) & ~3
.space  (IRQ_STACK_SIZE + 3) & ~3
.space  (SVR_STACK_SIZE + 3) & ~3
.space  (ABT_STACK_SIZE + 3) & ~3
.space  (UND_STACK_SIZE + 3) & ~3
_stack_end:

.text
.section .vect, "ax"
.code 32
.align 4

.global _vectors
.func _vectors
_vectors:
ldr     pc, _rst
ldr     pc, _und
ldr     pc, _swi
ldr     pc, _pabt
ldr     pc, _dabt
.word   0x00000000
ldr     pc, [pc, #-0x0120]
ldr     pc, _fiq

_rst:
.word   _start
_und:
.word   UndHandler
_swi:
.word   _swi_prehandler
_pabt:
.word   PAbtHandler
_dabt:
.word   DAbtHandler
_fiq:
.word   FIQHandler

.size _vectors, . - _vectors
.endfunc @ _vectors

.weak UndHandler
.global UndHandler
.func UndHandler
UndHandler:
b       .
.size UndHandler, . - UndHandler
.endfunc @ UndHandler

.global _swi_prehandler
.func   _swi_prehandler
_swi_prehandler:
stmfd   sp!, {r0-r12, lr}
ldr     r0, [lr, #-4]
bic     r0, r0, #0xFF000000
bl      SWIHandler
ldmfd   sp!, {r0-r12, lr}
movs    pc, lr
.size   _swi_prehandler, . - _swi_prehandler
.endfunc @ _swi_prehandler

.weak SWIHandler
.global SWIHandler
.func SWIHandler
SWIHandler:
b       .
.size SWIHandler, . - SWIHandler
.endfunc @ SWIHandler

.weak PAbtHandler
.global PAbtHandler
.func PAbtHandler
PAbtHandler:
subs    pc, lr, #4
b       .
.size PAbtHandler, . - PAbtHandler
.endfunc @ PAbtHandler

.weak DAbtHandler
.global DAbtHandler
.func DAbtHandler
DAbtHandler:
subs    pc, lr, #8
@ b       .
.size DAbtHandler, . - DAbtHandler
.endfunc @ DAbtHandler

.weak FIQHandler
.global FIQHandler
.func FIQHandler
FIQHandler:
b       .
.size FIQHandler, . - FIQHandler
.endfunc @ FIQHandler

.section .init, "ax"
.code 32
.align 4

.global _start
.func   _start
_start:

ldr     R0, =_stack_end
@ Undefined Instruction Mode
msr     CPSR_c, #MODE_UNDEFINED | PSR_I_BIT | PSR_F_BIT
mov     SP, R0
sub     R0, R0, #UND_STACK_SIZE
@ Supervisor Mode
msr     CPSR_c, #MODE_SUPERVISOR | PSR_I_BIT | PSR_F_BIT
mov     SP, R0
sub     R0, R0, #SVR_STACK_SIZE
@ Abort Mode
msr     CPSR_c, #MODE_ABORT | PSR_I_BIT | PSR_F_BIT
mov     SP, R0
sub     R0, R0, #ABT_STACK_SIZE
@ IRQ Mode
msr     CPSR_c, #MODE_IRQ | PSR_I_BIT | PSR_F_BIT
mov     SP, R0
sub     R0, R0, #IRQ_STACK_SIZE
@ FIQ Mode
msr     CPSR_c, #MODE_FIQ | PSR_I_BIT | PSR_F_BIT
mov     SP, R0
sub     R0, R0, #FIQ_STACK_SIZE
@ System | User Mode
msr     CPSR_c, #MODE_USER | PSR_I_BIT | PSR_F_BIT
mov     SP, R0

ldr     R1, =_text_end
ldr     R2, =_data_start
ldr     R3, =_data_end
_data_cpy:
cmp     R2, R3
ldrlo   R0, [ R1 ], #4
strlo   R0, [ R2 ], #4
blo     _data_cpy

mov   R0, #0
ldr   R1, =_bss_start
ldr   R2, =_bss_end
_bss_clr:
cmp   R1, R2
strlo R0, [ R1 ], #4
blo   _bss_clr

ldr   R0, =_ctors_start
ldr   R1, =_ctors_end
_ctors_loop_start:
cmp   R0, R1
beq   _ctors_loop_end
ldr   R2, [ R0 ], #4
stmfd SP!, { R0 - R1 }
mov   LR, PC
mov   PC, R2
ldmfd SP!, { R0 - R1 }
b     _ctors_loop_start
_ctors_loop_end:

bl      __init

mov   R0, #0
mov   R1, R0
mov   R2, R0
mov   FP, R0
mov   R7, R0
ldr   R10, =main
mov   LR, PC
bx    R10
b     .

.size   _start, . - _start
.endfunc @ _start

.weak __init
.global __init
.func __init
__init:
bx    lr
.size __init, . - __init
.endfunc @ __init

.end

