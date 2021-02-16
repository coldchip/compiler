main:
	0: push fp           # prologue
	1: mov fp sp          # prologue
	2: mov r0 $4          # local size
	3: add sp r0          # allocate local variables
	4: mov r0 $1          # move literal to register 0
	5: mov @0 r0          # store variable
	6: mov r0 $10000          # move literal to register 0
	7: push r0           # push right
	8: mov r0 @0          # access variable
	9: push r0           # push left
	10: pop r0           # pop right
	11: pop r1           # pop left
	12: cmp r0 r1          # cmp (expr)
	13: setelt r0           # setelt (expr)
	14: cmp r0 $1          # cmp (while)
	15: jne @119           # jmp finish(while)
	16: mov r0 $0          # move literal to register 0
	17: push r0           # push right
	18: mov r0 $3          # move literal to register 0
	19: push r0           # push right
	20: mov r0 @0          # access variable
	21: push r0           # push left
	22: pop r0           # pop right
	23: pop r1           # pop left
	24: mod r0 r1          # mod
	25: push r0           # push left
	26: pop r0           # pop right
	27: pop r1           # pop left
	28: cmp r0 r1          # cmp (expr)
	29: seteeq r0           # seteeq (expr)
	30: cmp r0 $1          # cmp (while)
	31: jne @39           # jmp finish(while)
	32: mov r9 sp          # preserve arguments pointer #1 (fixed)
	33: mov r10 sp          # preserve arguments pointer #2 (unfixed)
	34: mov r0 $111111111          # move literal to register 0
	35: push r0           # push arg to stack
	36: call @0           # call
	37: mov sp r9          # restore sp from args
	38: mov r0 r11          # move return [r11] to local
	39: mov r0 $0          # move literal to register 0
	40: push r0           # push right
	41: mov r0 $5          # move literal to register 0
	42: push r0           # push right
	43: mov r0 @0          # access variable
	44: push r0           # push left
	45: pop r0           # pop right
	46: pop r1           # pop left
	47: mod r0 r1          # mod
	48: push r0           # push left
	49: pop r0           # pop right
	50: pop r1           # pop left
	51: cmp r0 r1          # cmp (expr)
	52: seteeq r0           # seteeq (expr)
	53: cmp r0 $1          # cmp (while)
	54: jne @62           # jmp finish(while)
	55: mov r9 sp          # preserve arguments pointer #1 (fixed)
	56: mov r10 sp          # preserve arguments pointer #2 (unfixed)
	57: mov r0 $222222222          # move literal to register 0
	58: push r0           # push arg to stack
	59: call @0           # call
	60: mov sp r9          # restore sp from args
	61: mov r0 r11          # move return [r11] to local
	62: mov r0 $0          # move literal to register 0
	63: push r0           # push right
	64: mov r0 $5          # move literal to register 0
	65: push r0           # push right
	66: mov r0 @0          # access variable
	67: push r0           # push left
	68: pop r0           # pop right
	69: pop r1           # pop left
	70: mod r0 r1          # mod
	71: push r0           # push left
	72: pop r0           # pop right
	73: pop r1           # pop left
	74: cmp r0 r1          # cmp (expr)
	75: seteneq r0           # seteneq (expr)
	76: push r0           # push right
	77: mov r0 $0          # move literal to register 0
	78: push r0           # push right
	79: mov r0 $3          # move literal to register 0
	80: push r0           # push right
	81: mov r0 @0          # access variable
	82: push r0           # push left
	83: pop r0           # pop right
	84: pop r1           # pop left
	85: mod r0 r1          # mod
	86: push r0           # push left
	87: pop r0           # pop right
	88: pop r1           # pop left
	89: cmp r0 r1          # cmp (expr)
	90: seteneq r0           # seteneq (expr)
	91: push r0           # push left
	92: pop r0           # pop right
	93: pop r1           # pop left
	94: cmp r0 $1          # logical and cmp #1
	95: seteeq r0           # seteeq (expr)
	96: cmp r1 $1          # logical and cmp #2
	97: seteeq r1           # seteeq (expr)
	98: and r0 r1          # AND logical and
	99: cmp r0 $1          # logand cmp AND'ed
	100: seteeq r0           # seteeq (expr)
	101: cmp r0 $1          # cmp (while)
	102: jne @110           # jmp finish(while)
	103: mov r9 sp          # preserve arguments pointer #1 (fixed)
	104: mov r10 sp          # preserve arguments pointer #2 (unfixed)
	105: mov r0 @0          # access variable
	106: push r0           # push arg to stack
	107: call @0           # call
	108: mov sp r9          # restore sp from args
	109: mov r0 r11          # move return [r11] to local
	110: mov r0 $1          # move literal to register 0
	111: push r0           # push right
	112: mov r0 @0          # access variable
	113: push r0           # push left
	114: pop r0           # pop right
	115: pop r1           # pop left
	116: add r0 r1          # add
	117: mov @0 r0          # store
	118: jmp @6           # jmp (while)
	119: mov sp fp          # epilogue
	120: pop fp           # epilogue
