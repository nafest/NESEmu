#include "CPU6502.h"


CPU6502::CPU6502(unsigned char *memory)
{
	this->memory = memory;
	PC = 0x8000;
	SP = 0;
	A = 0;
	X = 0;
	Y = 0;
	P = 0;
}


CPU6502::~CPU6502()
{
}


void CPU6502::SetCarry(bool value) {
	/* the carry flag is in bit 0 of P */
	if (value)
		P = P | 1;
	else
		P = P & 0xfe;
}

bool CPU6502::GetCarry() {
	if (P & 1)
		return true;
	else
		return false;
}

void CPU6502::SetZero(bool value) {
	/* the zero flag is in bit 1 of P */
	if (value)
		P = P | (1 << 1);
	else
		P = P & ((0xff ^ (1 << 1)) & 0xff);
}


void CPU6502::SetInterrupt(bool value) {
	/* the interrupt flag is in bit 2 of P */
	if (value)
		P = P | (1 << 2);
	else
		P = P & ((0xff ^ (1 << 2)) & 0xff);
}

void CPU6502::SetDecimal(bool value) {
	/* the decimal mode flag is in bit 3 of P */
	if (value)
		P = P | (1 << 3);
	else
		P = P & ((0xff ^ (1 << 3)) & 0xff);
}

void CPU6502::SetNegative(bool value) {
	/* the negative flag is in bit 7 of P */
	if (value)
		P = P | (1 << 7);
	else
		P = P & ((0xff ^ (1 << 7)) & 0xff);
}

bool CPU6502::GetNegative() {
	if (P & (1 << 7))
		return true;
	else
		return false;
}

void CPU6502::SetOverflow(bool value) {
	/* the overflow flag is in bit 6 of P */
	if (value)
		P = P | (1 << 6);
	else
		P = P & ((0xff ^ (1 << 6)) & 0xff);
}

void CPU6502::ADC(unsigned char M)
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

void CPU6502::LDA(unsigned char M)
{
	A = M;
	SetZero(A == 0);
	SetNegative(A > 127);
}

void CPU6502::LDX(unsigned char M)
{
	X = M;
	SetZero(X == 0);
	SetNegative(X > 127);
}

void CPU6502::LDY(unsigned char M)
{
	Y = M;
	SetZero(Y == 0);
	SetNegative(Y > 127);
}

void CPU6502::Step() {
	/* fetch the next instruction */
	unsigned char OpCode = memory[PC];

	/* increase the program counter */
	PC++;

	/* execute the next instruction */

	switch (OpCode) {
	unsigned char M;
	signed char offset8;
	unsigned short addr;
    
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

	/* SEI - set intterupt disable */
	case 0x78:
		SetInterrupt(true);
		break;

	/* STA - store accumulator */
	case 0x8d:
		/* absolute addressing */
		addr = *((unsigned short*)(memory + PC));
		PC += 2;
		memory[addr] = A;
		break;

	/* LDA - load Accumulator */
	case 0xa9:
		/* immediate addressing */
		M = memory[PC];
		PC++;
		LDA(M);
		break;

	case 0xad:
		/* absolute addressing */
		addr = *((unsigned short*)(memory + PC));
		PC += 2;
		M = memory[addr];
		LDA(M);
		break;

	/* LDX - load X Register */
	case 0xa2:
		/* immediate addressing */
		M = memory[PC];
		PC++;
		LDX(M);
		break;

	case 0xa6:
		/* ZeroPage addressing */
		addr = memory[PC];
		M = memory[addr];
		PC++;
		LDX(M);
		break;

	case 0xb6:
		/* ZeroPage,Y addressing */
		addr = memory[PC];
		addr = (addr + Y) & 0xff;
		M = memory[addr];
		PC++;
		LDX(M);
		break;

	/* LDY - load X Register */
	case 0xa0:
		/* immediate addressing */
		M = memory[PC];
		PC++;
		LDY(M);
		break;

	case 0xa4:
		/* ZeroPage addressing */
		addr = memory[PC];
		M = memory[addr];
		PC++;
		LDY(M);
		break;

	case 0xb4:
		/* ZeroPage,X addressing */
		addr = memory[PC];
		addr = (addr + X) & 0xff;
		M = memory[addr];
		PC++;
		LDY(M);
		break;

	/* TXS - Transfer X to Stack Pointer */
	case 0x9a:
		SP = X;
		break;

	/* CLD - Clear Decimal Mode */
	case 0xd8:
		SetDecimal(false);
		break;

	/* BPL - Branch if Positive */
	case 0x10:
		offset8 = (char)memory[PC];
		if (!GetNegative())
			PC += offset8;
		else
			PC++;
		break;

	}
}
