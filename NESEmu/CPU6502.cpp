#include "CPU6502.h"
#include <iostream>

using namespace std;


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

bool CPU6502::GetZero() {
	if (P & (1 << 1))
		return true;
	else
		return false;
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

void CPU6502::SetBreak(bool value) {
	/* the break flag is in bit 4 of P */
	if (value)
		P = P | (1 << 4);
	else
		P = P & ((0xff ^ (1 << 4)) & 0xff);
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

bool CPU6502::GetOverflow() {
	if (P & (1 << 6))
		return true;
	else
		return false;
}

void CPU6502::Push(unsigned char value) {
	memory[0x100 + SP] = value;
	SP--;
}

unsigned char CPU6502::Pop() {
	SP++;
	return memory[0x100 + SP];
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

void CPU6502::AND(unsigned char M)
{
	A = A & M;

	SetZero(A == 0);
	SetNegative(A > 127);
}

void CPU6502::BIT(unsigned char M)
{
	unsigned char result = A & M;

	SetZero(result == 0);
	SetOverflow((result & (1 << 6)) != 0);
	SetNegative((result & (1 << 7)) != 0);
}

void CPU6502::CMP(unsigned char M)
{
	unsigned short result = A - M;

	SetCarry(A >= M);
	SetZero(A == M);

	SetNegative(result > 127);
}

void CPU6502::CPX(unsigned char M)
{
	unsigned short result = X - M;

	SetCarry(X >= M);
	SetZero(X == M);

	SetNegative(result > 127);
}

void CPU6502::CPY(unsigned char M)
{
	unsigned short result = Y - M;

	SetCarry(Y >= M);
	SetZero(Y == M);

	SetNegative(result > 127);
}

void CPU6502::DEC(unsigned short addr)
{
	unsigned char result = memory[addr] - 1;

	SetZero(result == 0);
	SetNegative(result > 127);

	memory[addr] = result;
}

void CPU6502::ASL(unsigned char M)
{
	unsigned short result = M << 1;

	SetCarry((M & (1 << 7)) != 0);
	SetZero((result & 0xff) == 0);
	SetNegative((result & (1 << 7)) != 0);

	A = result && 0xff;
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

int CPU6502::Step() {
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
		return 3;

	case 0x69:
		/* immediate addressing */
		M = memory[PC];
		PC++;
		ADC(M);
		return 2;

	case 0x75:
		/* ZeroPage, X */
		addr = memory[PC];
		addr = (addr + X) & 0xff;
		M = memory[addr];
		PC++;
		ADC(M);
		return 4;

	case 0x6d:
		/* absolute addressing */
		addr = *((unsigned short*)(memory + PC));
		PC += 2;
		M = memory[addr];
		ADC(M);
		return 4;

	case 0x7d:
		/* absolute,X addressing */
		addr = *((unsigned short*)(memory + PC));
		addr = addr + X;
		PC += 2;
		M = memory[addr];
		ADC(M);
		return 4;

	case 0x79:
		/* absolute,Y addressing */
		addr = *((unsigned short*)(memory + PC));
		addr = addr + Y;
		PC += 2;
		M = memory[addr];
		ADC(M);
		return 4;

	case 0x61:
		/* indirect,X addressing */
		addr = memory[PC];
		addr = addr + X;
		PC += 1;
		M = memory[addr];
		ADC(M);
		return 6;

	case 0x71:
		/* indirect,Y addressing */
		addr = memory[PC];
		addr = memory[addr] + Y;
		PC += 1;
		M = memory[addr];
		ADC(M);
		return 5;

	/* AND - logical AND */
	case 0x25:
		/* ZeroPage */
		addr = memory[PC];
		M = memory[addr];
		PC++;
		AND(M);
		return 3;

	case 0x29:
		/* immediate addressing */
		M = memory[PC];
		PC++;
		AND(M);
		return 2;

	case 0x35:
		/* ZeroPage, X */
		addr = memory[PC];
		addr = (addr + X) & 0xff;
		M = memory[addr];
		PC++;
		AND(M);
		return 4;

	case 0x2d:
		/* absolute addressing */
		addr = *((unsigned short*)(memory + PC));
		PC += 2;
		M = memory[addr];
		AND(M);
		return 4;

	case 0x3d:
		/* absolute,X addressing */
		addr = *((unsigned short*)(memory + PC));
		addr = addr + X;
		PC += 2;
		M = memory[addr];
		AND(M);
		return 4;

	case 0x39:
		/* absolute,Y addressing */
		addr = *((unsigned short*)(memory + PC));
		addr = addr + Y;
		PC += 2;
		M = memory[addr];
		AND(M);
		return 4;

	case 0x21:
		/* indirect,X addressing */
		addr = memory[PC];
		addr = addr + X;
		PC += 1;
		M = memory[addr];
		AND(M);
		return 6;

	case 0x31:
		/* indirect,Y addressing */
		addr = memory[PC];
		addr = memory[addr] + Y;
		PC += 1;
		M = memory[addr];
		AND(M);
		return 5;

	/* ASL - Arithmetic Shift Left */
	case 0x06:
		/* ZeroPage */
		addr = memory[PC];
		M = memory[addr];
		PC++;
		ASL(M);
		return 5;

	case 0x0a:
		/* immediate addressing */
		M = memory[PC];
		PC++;
		ASL(M);
		return 2;

	case 0x16:
		/* ZeroPage, X */
		addr = memory[PC];
		addr = (addr + X) & 0xff;
		M = memory[addr];
		PC++;
		ASL(M);
		return 6;

	case 0x0e:
		/* absolute addressing */
		addr = *((unsigned short*)(memory + PC));
		PC += 2;
		M = memory[addr];
		ASL(M);
		return 6;

	case 0x1e:
		/* absolute,X addressing */
		addr = *((unsigned short*)(memory + PC));
		addr = addr + X;
		PC += 2;
		M = memory[addr];
		ASL(M);
		return 7;


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

	case 0xbd:
		/* absolute,X addressing */
		addr = *((unsigned short*)(memory + PC));
		PC += 2;
		addr += X;
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

	
	/* BCC - Branch if Carry Clear */
	case 0x90:
		offset8 = (char)memory[PC];
		if (!GetCarry())
		{
			PC += 1 + offset8;
			return 3;
		}
		else
		{
			PC++;
			return 2;
		}

	/* BPL - Branch if Positive */
	case 0x10:
		offset8 = (char)memory[PC];
		if (!GetNegative())
			PC += 1 + offset8;
		else
			PC++;
		break;

	/* BCS - Branch if Carry Set */
	case 0xb0:
		offset8 = (char)memory[PC];
		if (GetCarry())
		{
			PC += 1 + offset8;
			return 3;
		}
		else
		{
			PC++;
			return 2;
		}

	/* BEQ - Branch if Equal */
	case 0xf0:
		offset8 = (char)memory[PC];
		if (GetZero())
		{
			PC += 1 + offset8;
			return 3;
		}
		else
		{
			PC++;
			return 2;
		}

	/* BIT - Bit Test */
	case 0x24:
		/* ZeroPage addressing */
		addr = memory[PC];
		M = memory[addr];
		PC++;
		BIT(M);
		return 3;

	case 0x2c:
		/* absolute addressing */
		addr = *((unsigned short*)(memory + PC));
		PC += 2;
		M = memory[addr];
		BIT(M);
		return 4;

	/* BMI - Branch if Minus */
	case 0x30:
		offset8 = (char)memory[PC];
		if (GetNegative())
		{
			PC += 1 + offset8;
			return 3;
		}
		else
		{
			PC++;
			return 2;
		}

	/* BNE - Branch if Net Equal */
	case 0xd0:
		offset8 = (char)memory[PC];
		if (!GetZero())
		{
			PC += 1 + offset8;
			return 3;
		}
		else
		{
			PC++;
			return 2;
		}

	/* BRK - Force Interrupt */
	case 0x00:
		/* push PC and P to the stack */
		Push(PC && 0xff);
		Push((PC >> 8) && 0xff);
		Push(P);

		SetBreak(true);
		PC = *((unsigned short *)(memory + 0xfffe));
		return 7;

	/* BVC - Branch if Overflow Clear */
	case 0x50:
		offset8 = (char)memory[PC];
		if (!GetOverflow())
		{
			PC += 1 + offset8;
			return 3;
		}
		else
		{
			PC++;
			return 2;
		}

	/* BVS - Branch if Overflow Set */
	case 0x70:
		offset8 = (char)memory[PC];
		if (GetOverflow())
		{
			PC += 1 + offset8;
			return 3;
		}
		else
		{
			PC++;
			return 2;
		}

	/* CLC Clear Carry Flag */
	case 0x18:
		SetCarry(false);
		return 2;
    
	/* CLD - Clear Decimal Mode */
	case 0xd8:
		SetDecimal(false);
		return 2;

	/* CLI - Clear Interrupt Disable */
	case 0x58:
		SetInterrupt(false);
		return 2;

	/* CLV - Clear Overflow Flag */
	case 0xb8:
		SetOverflow(false);
		return 2;

	/* CMP - compare */
	case 0xc9:
		/* immediate */
		M = memory[PC];
		PC++;
		CMP(M);
		return 2;

	case 0xc5:
		/* ZeroPage */
		addr = memory[PC];
		M = memory[addr];
		PC++;
		CMP(M);
		return 3;

	case 0xd5:
		/* ZeroPage, X */
		addr = memory[PC];
		addr = (addr + X) & 0xff;
		M = memory[addr];
		PC++;
		CMP(M);
		return 4;

	case 0xcd:
		/* absolute addressing */
		addr = *((unsigned short*)(memory + PC));
		PC += 2;
		M = memory[addr];
		CMP(M);
		return 4;

	case 0xdd:
		/* absolute,X addressing */
		addr = *((unsigned short*)(memory + PC));
		addr = addr + X;
		PC += 2;
		M = memory[addr];
		CMP(M);
		return 4;

	case 0xd9:
		/* absolute,Y addressing */
		addr = *((unsigned short*)(memory + PC));
		addr = addr + Y;
		PC += 2;
		M = memory[addr];
		CMP(M);
		return 4;

	case 0xc1:
		/* indirect,X addressing */
		addr = memory[PC];
		addr = addr + X;
		PC += 1;
		M = memory[addr];
		CMP(M);
		return 6;

	case 0xd1:
		/* indirect,Y addressing */
		addr = memory[PC];
		addr = memory[addr] + Y;
		PC += 1;
		M = memory[addr];
		CMP(M);
		return 5;

	/* CPX - compare X Register*/
	case 0xe0:
		/* immediate */
		M = memory[PC];
		PC++;
		CPX(M);
		return 2;

	case 0xe4:
		/* ZeroPage */
		addr = memory[PC];
		M = memory[addr];
		PC++;
		CPX(M);
		return 3;

	case 0xec:
		/* absolute addressing */
		addr = *((unsigned short*)(memory + PC));
		PC += 2;
		M = memory[addr];
		CPX(M);
		return 4;

	/* CPY - compare Y Register4*/
	case 0xc0:
		/* immediate */
		M = memory[PC];
		PC++;
		CPY(M);
		return 2;

	case 0xc4:
		/* ZeroPage */
		addr = memory[PC];
		M = memory[addr];
		PC++;
		CPY(M);
		return 3;

	case 0xcc:
		/* absolute addressing */
		addr = *((unsigned short*)(memory + PC));
		PC += 2;
		M = memory[addr];
		CPY(M);
		return 4;

	/* DEC - Decrement Memory */
	case 0xc6:
		/* ZeroPage */
		addr = memory[PC];
		PC++;
		DEC(addr);
		return 5;

	case 0xd6:
		/* ZeroPage, X */
		addr = memory[PC];
		addr = (addr + X) & 0xff;
		PC++;
		DEC(addr);
		return 6;

	case 0xce:
		/* absolute addressing */
		addr = *((unsigned short*)(memory + PC));
		PC += 2;
		DEC(addr);
		return 6;

	case 0xde:
		/* absolute,X addressing */
		addr = *((unsigned short*)(memory + PC));
		addr = addr + X;
		PC += 2;
		DEC(addr);
		return 7;

		/* unimplemented instruction */
	default:
		exit(-1);
		break;
	}
}
