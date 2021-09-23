	jmp main
alloc:
	loadi 0
	syscall 0
	ret 0
print_int:
	loadi 0
	syscall 0
	ret 0
print_float:
	loadi 0
	syscall 1
	ret 0
socket:
	loadi 0
	loadi 4
	loadi 8
	syscall 2
	ret 0
sin:
	pushi 0
	storei 4
	loadi 0
	storei 8
	pushi 1065353216
	storei 12
	pushi 0
	storei 16
l0:
	pushi 5
	loadi 16
	cmplt
	pushi 0
	je l1
	loadi 12
	loadi 8
	divf
	loadi 4
	addf
	storei 4
	loadi 0
	loadi 0
	pushi 1
	pushi 0
	sub
	ci2f32
	mulf
	mulf
	loadi 8
	mulf
	storei 8
	pushi 1
	pushi 1
	loadi 16
	add
	pushi 2
	mul
	add
	pushi 1
	loadi 16
	add
	pushi 2
	mul
	mul
	ci2f32
	loadi 12
	mulf
	storei 12
	pushi 1
	loadi 16
	add
	storei 16
	jmp l0
l1:
	loadi 4
	ret 4
main:
	pushi 0
	storei 0
l2:
	pushi 1
	pushi 0
	je l3
	loadi 0
	call sin 4
	call print_float 4
	pushi 981668463
	loadi 0
	addf
	storei 0
	jmp l2
l3:
	ret 0
