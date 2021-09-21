	jmp main
print_float:
	load 0
	syscall 1
	ret 0
sin:
	push_int 0
	store 4
	load 0
	store 8
	push_int 1
	store 12
	push_int 0
	store 16
w_0:
	push_int 5
	load 16
	cmplt
	push_int 0
	je w_1
	load 12
	load 8
	div
	load 4
	add
	store 4
	load 0
	load 0
	push_int 1
	push_int 0
	sub
	mul
	mul
	load 8
	mul
	store 8
	push_int 1
	push_int 1
	load 16
	add
	push_int 2
	mul
	add
	push_int 1
	load 16
	add
	push_int 2
	mul
	mul
	load 12
	mul
	store 12
	push_int 1
	load 16
	add
	store 16
	jmp w_0
w_1:
	load 4
	ret 4
main:
	push_int 10
	push_int 2
	call sin 4
	mul
	store 0
	load 0
	call print_float 4
	ret 0
