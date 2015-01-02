#pragma once
class CPU6502
{
public:
	CPU6502();
	~CPU6502();

	void SetCarry(bool value) {

	}

	bool GetCarry() {
		return true;
	}

	void SetZero(bool value) {

	}

	void SetNegative(bool value) {

	}

	void SetOverflow(bool value) {

	}

	void ADC(unsigned char M)
	{
		unsigned short result = A + M + GetCarry() ? 1 : 0;

		SetCarry(result > 255);
		SetZero(result == 0);
		if ((((A ^ M) & 0x80) == 0) && (((A ^ result) & 0x80) != 0))
			SetOverflow(true);
		else
			SetOverflow(false);
		SetNegative(result > 127);
	}

	void Step() {
		/* fetch the next instruction */
		unsigned char OpCode = memory[PC];
		
		/* increase the program counter */
		PC++;

		/* execute the next instruction */
		
		switch (OpCode) {
		unsigned char addr, M;
			/* ADC - Add with carry */
		case 0x65:
			/* ZeroPage */
			addr = memory[PC];
			M = memory[addr];
			PC++;
			ADC(M);
			break;

		case 0x69:
			/* immediate addressing */
			M = memory[PC];
			PC++;
			ADC(M);
			break;

		case 0x75:
			/* ZeroPage, X */
			addr = memory[PC];
			addr = (addr + X) & 0xff;
			M = memory[addr];
			PC++;
			ADC(M);
			break;
		}
	}


private:
	unsigned short PC;   /* program counter */
	unsigned char  SP;   /* stack pointer */
	unsigned char  A;    /* accumulator register */
	unsigned char  X;    /* index register X */
	unsigned char  Y;    /* index register Y */
	unsigned char  P;    /* processor status */

	unsigned char  *memory;  /* pointer to memory */
};

