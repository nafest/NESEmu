#include "Emulator.h"


Emulator::Emulator()
{
	cpu = new CPU6502(memory);
	/* load the Super Mario Bros ROM */
	rom = new ROM("../../roms/Super Mario Bros. (E).nes");

	/* and copy the two PGR-ROM banks */
	rom->CopyPGR(0, memory);
	rom->CopyPGR(1, memory);

	cpu->Step();
}


Emulator::~Emulator()
{
}
