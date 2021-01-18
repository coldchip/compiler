[asm]
procedure add:
	pushstack
	mov r0 @r5+0
	push r0
	mov r0 @r5+8
	push r0
	mov r0 @fp+0
	push r0
	mov r0 @fp+0
	push r0
	pop r0
	pop r1
	add r0 r1
	push r0
	pop r0
	push j0
	popstack
procedure main:
	pushstack
	push 6
	push 5
	pop r0
	pop r1
	add r0 r1
	push r0
	push 4
	pop r0
	pop r1
	add r0 r1
	push r0
	push 3
	pop r0
	pop r1
	add r0 r1
	push r0
	push 2
	pop r0
	pop r1
	add r0 r1
	push r0
	push 1
	pop r0
	pop r1
	add r0 r1
	push r0
	popstack
