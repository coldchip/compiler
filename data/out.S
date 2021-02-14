add:
	0: push fp           # prologue
	1: mov fp sp          # prologue
	2: mov r0 $8          # local size
	3: add sp r0          # allocate local variables
	4: movind @4 r10          # mov arguments to local stack
	5: mov r0 $4          # move arg size
	6: add r10 r0          # increment the arg size
	7: movind @0 r10          # mov arguments to local stack
	8: mov r0 $4          # move arg size
	9: add r10 r0          # increment the arg size
	10: mov r0 @4          # access variable
	11: push r0           # push right
	12: mov r0 @0          # access variable
	13: push r0           # push left
	14: pop r0           # pop right
	15: pop r1           # pop left
	16: add r0 r1          # add
	17: mov r11 r0          # move data to return reg [r11]
	18: mov sp fp          # epilogue
	19: pop fp           # epilogue
main:
	0: push fp           # prologue
	1: mov fp sp          # prologue
	2: mov r0 $8          # local size
	3: add sp r0          # allocate local variables
	4: mov r0 $0          # move literal to register 0
	5: mov @0 r0          # store variable
	6: mov r0 $999          # move literal to register 0
	7: push r0           # push right
	8: mov r0 @0          # access variable
	9: push r0           # push left
	10: pop r0           # pop right
	11: pop r1           # pop left
	12: cmp r0 r1          # cmp (expr)
	13: setelt r0           # setelt (expr)
	14: cmp r0 $0          # cmp (while)
	15: je @33           # jmp finish(while)
	16: mov r9 sp          # preserve arguments pointer #1 (fixed)
	17: mov r10 sp          # preserve arguments pointer #2 (unfixed)
	18: mov r0 @4          # access variable
	19: push r0           # push arg to stack
	20: call @0           # call
	21: mov sp r9          # restore sp from args
	22: mov r0 r11          # move return [r11] to local
	23: mov r0 $1          # move literal to register 0
	24: push r0           # push right
	25: mov r0 @0          # access variable
	26: push r0           # push left
	27: pop r0           # pop right
	28: pop r1           # pop left
	29: add r0 r1          # add
	30: mov @0 r0          # store
	31: mov @4 r0          # store
	32: jmp @6           # jmp (while)
	33: mov sp fp          # epilogue
	34: pop fp           # epilogue
