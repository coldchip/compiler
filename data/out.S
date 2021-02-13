printf:
	0: push fp 
	1: mov fp sp
	2: mov r0 $0
	3: add sp r0
	4: mov sp fp
	5: pop fp 
main:
	0: push fp 
	1: mov fp sp
	2: mov r0 $4
	3: add sp r0
	4: mov r0 $10
	5: push r0 
	6: mov r0 $3
	7: push r0 
	8: mov r0 $5
	9: push r0 
	10: mov r0 $5
	11: push r0 
	12: mov r0 $2
	13: push r0 
	14: mov r0 $2
	15: push r0 
	16: pop r0 
	17: pop r1 
	18: mul r0 r1
	19: push r0 
	20: mov r0 $20
	21: push r0 
	22: pop r0 
	23: pop r1 
	24: div r0 r1
	25: push r0 
	26: pop r0 
	27: pop r1 
	28: mul r0 r1
	29: push r0 
	30: pop r0 
	31: pop r1 
	32: add r0 r1
	33: push r0 
	34: pop r0 
	35: pop r1 
	36: div r0 r1
	37: push r0 
	38: pop r0 
	39: pop r1 
	40: mul r0 r1
	41: mov @0 r0
	42: mov sp fp
	43: pop fp 
