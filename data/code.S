test:
	push_int 0
	store z
	load_addr z
	store y
	push_int 20
	push_int 1
	load y
	add
	store_ptr
	ret
main:
	push_int 99
	store g
	push_int 9999
	store h
	push_int 6969
	store a
	push_int 1
	load_addr a
	add
	store b
	push_int 1
	load b
	sub
	deref
	store c
