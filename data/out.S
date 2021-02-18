printf:
	0: push fp           # prologue
	1: mov fp sp          # prologue
	2: mov r0 $4          # local size
	3: add sp r0          # allocate local variables
	4: mov @0 [r10]          # mov arguments to local stack
	5: mov r0 $4          # move arg size
	6: add r10 r0          # increment the arg size
	7: mov r10 sp          # preserve arguments pointer #2 (unfixed)
	8: mov r0 @0          # access variable
	9: push r0           # push arg to stack
	10: push r10           # push arg last sp
	11: syscall            # syscall
	12: pop sp           # restore sp from args
	13: mov r0 r11          # move return [r11] to local
	14: mov sp fp          # epilogue
	15: pop fp           # epilogue
	16: ret            # ret
main:
	17: push fp           # prologue
	18: mov fp sp          # prologue
	19: mov r0 $4          # local size
	20: add sp r0          # allocate local variables
	21: mov r0 $0          # move literal to register 0
	22: mov @0 r0          # store variable
	23: mov r0 $999999          # move literal to register 0
	24: push r0           # push right
	25: mov r0 @0          # access variable
	26: push r0           # push left
	27: pop r0           # pop right
	28: pop r1           # pop left
	29: cmp r0 r1          # cmp (expr)
	30: setelt r0           # setelt (expr)
	31: cmp r0 $1          # cmp (while)
	32: jne @49           # jmp finish(while)
	33: mov r10 sp          # preserve arguments pointer #2 (unfixed)
	34: mov r0 @0          # access variable
	35: push r0           # push arg to stack
	36: push r10           # push arg last sp
	37: call @0           # call
	38: pop sp           # restore sp from args
	39: mov r0 r11          # move return [r11] to local
	40: mov r0 $1          # move literal to register 0
	41: push r0           # push right
	42: mov r0 @0          # access variable
	43: push r0           # push left
	44: pop r0           # pop right
	45: pop r1           # pop left
	46: add r0 r1          # add
	47: mov @0 r0          # store
	48: jmp @23           # jmp (while)
	49: mov sp fp          # epilogue
	50: pop fp           # epilogue
	51: ret            # ret
