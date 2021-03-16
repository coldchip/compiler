ENTRY(program);

program:
	PUSH32(fp); 
	MOV32(fp, sp);
	ADD32(sp, 128);

	MOV32(STACK32(4), 0);
	MOV32(STACK32(8), 1);
	MOV32(STACK32(12), 0);
	
L0:

	MOV32(r0, STACK32(4));
	MOV32(r1, STACK32(8));

	MOV32(r3, sp);
	PUSH32(r0);
	PUSH32(r1);
	CALL(add);
	POP32E();
	POP32E();
	MOV32(r0, r4);

	MOV32(STACK32(12), r0);

	MOV32(STACK32(4), STACK32(8));
	MOV32(STACK32(8), STACK32(12));

	printf("%lu\n", STACK32(4));

	CMP(STACK32(4), 1000000000);
	JL(L0);
L1:
	MOV32(sp, fp);
	POP32(fp);

	MOV32(r3, sp);
	PUSH32(STACK32(16));
	PUSH32(1);
	CALL(add);
	POP32E();
	POP32E();
	MOV32(r0, r4);
	MOV32(STACK32(16), r4);

	CMP(STACK32(16), 200000);
	JL(program);
	EXIT();
add:
	PUSH32(fp); 
	MOV32(fp, sp);
	ADD32(sp, 32);

	PUSH32(STACK32(r3));
	PUSH32(STACK32(r3 + 4));
	POP32(r0);
	POP32(r1);

	ADD32(r0, r1);

	MOV32(r4, r0);

	MOV32(sp, fp);
	POP32(fp);
	RET();