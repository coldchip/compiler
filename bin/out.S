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
	push 256
	push 256
	mov r0 @fp+0
	push r0
	mov r0 @fp+0
	push r0
	pop r0
	pop r1
	add r0 r1
	push r0
	popstack
