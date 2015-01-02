#pragma once
class CPU6502
{
public:
	CPU6502(unsigned char *memory);
	~CPU6502();

	void SetCarry(bool value);
	bool GetCarry();
	void SetZero(bool value);
	void SetInterrupt(bool value);
	void SetNegative(bool value);
	void SetOverflow(bool value);
	void ADC(unsigned char M);
	void Step();


private:
	unsigned short PC;   /* program counter */
	unsigned char  SP;   /* stack pointer */
	unsigned char  A;    /* accumulator register */
	unsigned char  X;    /* index register X */
	unsigned char  Y;    /* index register Y */
	unsigned char  P;    /* processor status */

	unsigned char  *memory;  /* pointer to memory */
};

