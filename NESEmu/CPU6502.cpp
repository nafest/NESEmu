#include "CPU6502.h"


CPU6502::CPU6502(unsigned char *memory)
{
	this->memory = memory;
	PC = 0x8000;
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

void CPU6502::SetNegative(bool value) {
	/* the negative flag is in bit 7 of P */
	if (value)
		P = P | (1 << 7);
	else
		P = P & ((0xff ^ (1 << 7)) & 0xff);
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

void CPU6502::Step() {
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

	/* SEI - set intterupt disable */
	case 0x78:
		SetInterrupt(true);
		break;
	}
}
