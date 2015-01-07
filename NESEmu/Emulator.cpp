#include "Emulator.h"


Emulator::Emulator()
{
	int instrCnt = 0;
	ppu = new PPU(memory);
	cpu = new CPU6502(memory,ppu);
	/* load the Super Mario Bros ROM */
	rom = new ROM("../../roms/Super Mario Bros. (E).nes");
	//rom = new ROM("../../roms/nestest.nes");
	//cpu->SetPC(0xc000);
	/* and copy the two PGR-ROM banks */
	rom->CopyPRG(0, memory);
	rom->CopyPRG(1, memory);
	rom->CopyCHR((unsigned char*)ppu->GetMemoryPtr());
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
