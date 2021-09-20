	jmp main
sleep:
	load 0
	syscall 2
	ret 0
print:
	load 0
	syscall 0
	ret 0
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
test:
	load 8
	load 4
	load 0
	add
	add
	ret 4
main:
	push_int 0
	store 0
	push_int 1036831949
	store 4
	push_int 0
	store 8
	push_int 0
	store 12
	push_int 0
	store 16
w_2:
	push_int 30
	load 0
	cmplt
	push_int 0
	je w_3
	load 16
	load 12
	load 8
	call test 12
	store 20
	load 20
	call print 4
	push_int 1
	load 8
	add
	store 8
	push_int 1
	load 12
	add
	store 12
	push_int 1
	load 16
	add
	store 16
	push_int 1
	load 0
	add
	store 0
	push_int 1
	call sleep 4
	jmp w_2
w_3:
	push_int 2
	push_int 1
	div
	call sin 4
	call print 4
	push_int 1060320051
	call sin 4
	store 24
	load 24
	call print_float 4
	ret 0
