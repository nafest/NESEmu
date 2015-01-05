#include "Emulator.h"


Emulator::Emulator()
{
	int instrCnt = 0;
	cpu = new CPU6502(memory);
	ppu = new PPU(memory);
	/* load the Super Mario Bros ROM */
	rom = new ROM("../../roms/Super Mario Bros. (E).nes");

	/* and copy the two PGR-ROM banks */
	rom->CopyPRG(0, memory);
	rom->CopyPRG(1, memory);
	for (; ; ) {
		int cpuCycles = cpu->Step();
		for (int i = 0; i < 3 * cpuCycles; i++)
			ppu->Step();

		instrCnt++;
	}
}


Emulator::~Emulator()
{
}
