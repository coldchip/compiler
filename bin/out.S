[asm]
test:
	push fp
	mov fp sp
	mov r0 @r5+0
	push r0
	mov r0 @r5+8
	push r0
	mov r0 @r5+16
	push r0
	mov r0 @r5+24
	push r0
	mov r0 @r5+32
	push r0
	mov r0 @r5+40
	push r0
	mov r0 @r5+48
	push r0
	mov r0 @r5+56
	push r0
	mov r0 @r5+64
	push r0
	mov r0 @r5+72
	push r0
	mov r0 @r5+80
	push r0
	mov r0 @r5+88
	push r0
	mov r0 @fp+8
	push r0
	mov r0 @fp+0
	push r0
	pop r0
	pop r1
	add r0 r1
	push r0
	mov r0 fp+16
	push r0
	pop r0
	pop r1
	mov [r0], r1
	mov sp fp
	pop fp
main:
	push fp
	mov fp sp
	push 64
	push 128
	push 128
	push 64
	push 128
	push 128
	push 64
	push 128
	push 128
	push 64
	push 128
	push 128
	mov r5 sp
	mov r0 @fp+0
	push r0
	mov r0 @fp+8
	push r0
	mov r0 @fp+16
	push r0
	mov r0 @fp+24
	push r0
	mov r0 @fp+32
	push r0
	mov r0 @fp+40
	push r0
	mov r0 @fp+48
	push r0
	mov r0 @fp+56
	push r0
	mov r0 @fp+64
	push r0
	mov r0 @fp+72
	push r0
	mov r0 @fp+80
	push r0
	mov r0 @fp+88
	push r0
	call test
	mov sp fp
	pop fp
