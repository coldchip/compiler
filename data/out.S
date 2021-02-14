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
	2: mov r0 $12          # local size
	3: add sp r0          # allocate local variables
	4: mov r0 $0          # move literal to register 0
	5: mov @0 r0          # store variable
	6: mov r0 $1          # move literal to register 0
	7: mov @4 r0          # store variable
	8: mov r0 $0          # move literal to register 0
	9: mov @8 r0          # store variable
	10: mov r0 $999999999          # move literal to register 0
	11: push r0           # push right
	12: mov r0 @0          # access variable
	13: push r0           # push left
	14: pop r0           # pop right
	15: pop r1           # pop left
	16: cmp r0 r1          # cmp (expr)
	17: setelt r0           # setelt (expr)
	18: cmp r0 $0          # cmp (while)
	19: je @42           # jmp finish(while)
	20: mov r9 sp          # preserve arguments pointer #1 (fixed)
	21: mov r10 sp          # preserve arguments pointer #2 (unfixed)
	22: mov r0 @4          # access variable
	23: push r0           # push arg to stack
	24: mov r0 @0          # access variable
	25: push r0           # push arg to stack
	26: call @0           # call
	27: mov sp r9          # restore sp from args
	28: mov r0 r11          # move return [r11] to local
	29: mov @8 r0          # store
	30: mov r0 @4          # access variable
	31: mov @0 r0          # store
	32: mov r0 @8          # access variable
	33: mov @4 r0          # store
	34: mov r9 sp          # preserve arguments pointer #1 (fixed)
	35: mov r10 sp          # preserve arguments pointer #2 (unfixed)
	36: mov r0 @8          # access variable
	37: push r0           # push arg to stack
	38: call @1           # call
	39: mov sp r9          # restore sp from args
	40: mov r0 r11          # move return [r11] to local
	41: jmp @10           # jmp (while)
	42: mov sp fp          # epilogue
	43: pop fp           # epilogue
