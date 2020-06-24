[asm]
test:
	lea r0 @fp+0
	push r0
	lea r0 @fp+0
	push r0
	lea r0 @fp+0
	push r0
	lea r0 @fp+0
	push r0
	lea r0 @fp+0
	push r0
	lea r0 @fp+0
	push r0
	lea r0 @fp+0
	push r0
	lea r0 @fp+0
	push r0
	lea r0 @fp+0
	push r0
	lea r0 @fp+0
	push r0
	lea r0 @fp+0
	push r0
	lea r0 @fp+0
	push r0
	push fp
	mov fp sp
	mov sp fp
	pop fp
main:
	push fp
	mov fp sp
	push 64
	pop r0
	push r0
	push 128
	pop r0
	push r0
	push 128
	pop r0
	push r0
	push 64
	pop r0
	push r0
	push 128
	pop r0
	push r0
	push 128
	pop r0
	push r0
	push 64
	pop r0
	push r0
	push 128
	pop r0
	push r0
	push 128
	pop r0
	push r0
	push 64
	pop r0
	push r0
	push 128
	pop r0
	push r0
	push 128
	pop r0
	push r0
	mov r5 sp
	lea r0 @fp+0
	push r0
	lea r0 @fp+8
	push r0
	lea r0 @fp+16
	push r0
	lea r0 @fp+24
	push r0
	lea r0 @fp+32
	push r0
	lea r0 @fp+40
	push r0
	lea r0 @fp+48
	push r0
	lea r0 @fp+56
	push r0
	lea r0 @fp+64
	push r0
	lea r0 @fp+72
	push r0
	lea r0 @fp+80
	push r0
	lea r0 @fp+88
	push r0
	call test
	mov sp fp
	pop fp
