printf:
	push %fp          # prologue
	mov %fp, %sp          # prologue
	mov %r0, $4          # local size
	add %sp, %r0          # allocate local variables
	mov %fp, PTR %r10          # mov arguments to local stack
	mov %r10, %sp          # preserve arguments pointer #2 (unfixed)
	mov %r0, $4          # move literal to register 0
	push %r0          # push arg to stack
	lea %r0, @0          # genaddr
	add %r0, %fp          # add base to ptr
	push %r0          # push arg to stack
	mov %r0, $0          # move literal to register 0
	push %r0          # push arg to stack
	push %r10          # push arg last sp
	syscall           # syscall
	pop %sp          # restore sp from args
	mov %r0, %r11          # move return [r11] to local
	mov %sp, %fp          # epilogue
	pop %fp          # epilogue
	ret           # ret
main:
	push %fp          # prologue
	mov %fp, %sp          # prologue
	mov %r0, $0          # local size
	add %sp, %r0          # allocate local variables
	mov %r10, %sp          # preserve arguments pointer #2 (unfixed)
	mov %r0, $444          # move literal to register 0
	push %r0          # push arg to stack
	push %r10          # push arg last sp
	call printf          # call
	pop %sp          # restore sp from args
	mov %r0, %r11          # move return [r11] to local
	mov %sp, %fp          # epilogue
	pop %fp          # epilogue
	ret           # ret
