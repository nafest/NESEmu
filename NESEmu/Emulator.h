#pragma once

#include "CPU6502.h"
#include "ROM.h"

class Emulator
{
public:
	Emulator();
	~Emulator();

private:
	unsigned char memory[0xffff];
	ROM      *rom;
	CPU6502  *cpu;

};

