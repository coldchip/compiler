ENTRY(program);

program:
	PUSH32(fp); 
	MOV32(fp, sp);
	ADD32(sp, 32);

	MOV32(STACK32(0), 0);
	MOV32(STACK32(4), 1);
	MOV32(STACK32(8), 0);
L0:

	MOV32(r0, STACK32(0));
	MOV32(r1, STACK32(4));

	ADD32(r0, r1);

	MOV32(STACK32(8), r0);

	MOV32(STACK32(0), STACK32(4));
	MOV32(STACK32(4), STACK32(8));

	printf("%lu\n", r1);

	CMP(r0, 1000000000);
	JL(L0);
L1:
	MOV32(sp, fp);
	POP32(fp);