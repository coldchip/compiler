	jmp main
print_int:
	load 0
	syscall 0
	ret 0
print_float:
	load 0
	syscall 1
	ret 0
socket:
	load 0
	load 4
	load 8
	syscall 2
	ret 0
sin:
	push_int 0
	store 4
	load 0
	store 8
	push_int 1065353216
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
	divf
	load 4
	addf
	store 4
	load 0
	load 0
	push_int 1065353216
	push_int 0
	subf
	mulf
	mulf
	load 8
	mulf
	store 8
	push_int 1065353216
	push_int 1065353216
	load 16
	ci2f32
	addf
	push_int 1073741824
	mulf
	addf
	push_int 1065353216
	load 16
	ci2f32
	addf
	push_int 1073741824
	mulf
	mulf
	load 12
	mulf
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
	push_int 0
	store 0
w_2:
	push_int 1092616192
	load 0
	cmpltf
	push_int 0
	je w_3
	load 0
	call sin 4
	call print_float 4
	push_int 1045220557
	load 0
	addf
	store 0
	jmp w_2
w_3:
	ret 0
