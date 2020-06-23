[asm]
printf:
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
	push 64
	pop r0
	push r0
	push 64
	pop r0
	push r0
	push 64
	pop r0
	push r0
	lea r0 [fp+24]
	push r0
	lea r0 [fp+16]
	push r0
	pop r0
	pop r1
	add r0 r1
	push r0
	lea r0 [fp+8]
	push r0
	pop r0
	pop r1
	add r0 r1
	push r0
	lea r0 [fp+0]
	push r0
	pop r0
	pop r1
	add r0 r1
	push r0
	pop r0
	push r0
	mov sp fp
	pop fp
