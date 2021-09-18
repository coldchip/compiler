	jmp main
print:
	load 0
	syscall 0
	ret
test:
	push_int 1
	load 0
	deref
	add
	load 0
	sta
	ret
main:
	push_int 0
	store 0
	push_int 1
	store 4
w_0:
	push_int 10
	load 0
	cmplt
	push_int 0
	je w_1
	load 0
	call print 4
	push_int 1
	load 0
	add
	store 0
	jmp w_0
w_1:
	ret
