#pragma once

#include <iostream>

using namespace std;

class PPU;
class Controller;

enum eInterrupt
{
	eNoInterrupt,
	eNMI
};

class CPU6502State
{
public:
	unsigned short PC;   /* program counter */
	unsigned char  SP;   /* stack pointer */
	unsigned char  A;    /* accumulator register */
	unsigned char  X;    /* index register X */
	unsigned char  Y;    /* index register Y */
	unsigned char  P;    /* processor status */

	bool IsEqual(CPU6502State other)
	{
		return ((PC == other.PC) && (SP == other.SP) && (A == other.A) &&
			(X == other.X) && (Y == other.Y )&& (P == other.P));
	}

	void PrintState() {
		cout << hex << (int)PC << "\tA: " << hex << (int)A;
		cout << "\tX: " << hex << (int)X;
		cout << "\tY: " << hex << (int)Y;
		cout << "\tP: " << hex << (int)P;
		cout << "\tSP: " << hex << (int)SP;

		cout << endl;
	}
};

class CPU6502
{
public:
	CPU6502(unsigned char *memory, PPU *ppu);
	~CPU6502();

	void SetController1(Controller *ctrl)
	{
		ctrl1 = ctrl;
	}

	void PrintState();
	CPU6502State GetState() const;

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

	unsigned short AdressIndirectX()
	{
		unsigned short zpAddr = memory[PC] + X;

		if (zpAddr > 255)
			zpAddr -= 256;

		unsigned char low = memory[zpAddr];
		zpAddr++;
		if (zpAddr > 255)
			zpAddr -= 256;

		unsigned char high = memory[zpAddr];

		unsigned short addr = (high << 8) + low;

		return addr;
	}

	unsigned short AdressIndirectY()
	{
		unsigned short zpAddr = memory[PC];
		unsigned char low = memory[zpAddr];
		zpAddr++;
		if (zpAddr > 255)
			zpAddr -= 256;

		unsigned char high = memory[zpAddr];

		unsigned short addr = (high << 8) + low;

		return addr + Y;
	}

	unsigned char Read(unsigned short addr);
	void Store(unsigned short addr, unsigned char value);
	
	void ADC(unsigned char M);
	void AND(unsigned char M);
	void ASLA();
	void ASLMem(unsigned short addr);
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
	void LSRA();
	void LSRMem(unsigned short addr);
	void ORA(unsigned char M);
	void ROLA();
	void ROLMem(unsigned short addr);
	void RORA();
	void RORMem(unsigned short addr);
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
	Controller     *ctrl1;

	eInterrupt  interrupt;

	long cycles;
};

