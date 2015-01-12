#pragma once

class PPU;

enum eInterrupt
{
	eNoInterrupt,
	eNMI
};

class CPU6502
{
public:
	CPU6502(unsigned char *memory, PPU *ppu);
	~CPU6502();

	void PrintState();
	void SetCarry(bool value);
	bool GetCarry();
	void SetZero(bool value);
	bool GetZero();
	void SetBreak(bool value);
	void SetInterrupt(bool value);
	void SetDecimal(bool value);
	void SetNegative(bool value);
	bool GetNegative();
	void SetOverflow(bool value);
	bool GetOverflow();
	
	void Push(unsigned char value);
	unsigned char Pop();

	unsigned char Read(unsigned short addr);
	void Store(unsigned short addr, unsigned char value);
	
	void ADC(unsigned char M);
	void AND(unsigned char M);
	void ASL(unsigned char M);
	void BIT(unsigned char M);
	void LDA(unsigned char M);
	void LDX(unsigned char M);
	void LDY(unsigned char M);
	void CMP(unsigned char M);
	void CPX(unsigned char M);
	void CPY(unsigned char M);
	void DEC(unsigned short addr);
	void EOR(unsigned char M);
	void INC(unsigned short addr);
	void LSR(unsigned char M);
	void ORA(unsigned char M);
	void ROL(unsigned char M);
	void ROR(unsigned char M);
	void SBC(unsigned char M);
	int Step();
	int OneStep();
	void SetPC(unsigned short PC);

	void SetInterrupt(eInterrupt intr);

private:
	unsigned short PC;   /* program counter */
	unsigned char  SP;   /* stack pointer */
	unsigned char  A;    /* accumulator register */
	unsigned char  X;    /* index register X */
	unsigned char  Y;    /* index register Y */
	unsigned char  P;    /* processor status */

	unsigned char  *memory;  /* pointer to memory */
	PPU            *ppu;

	eInterrupt  interrupt;

	long cycles;
};

