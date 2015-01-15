#include "Emulator.h"

#include "CPU6502_test.h"

#define TEST

Emulator::Emulator()
{
	CPU6502_test  *cpuTest = new CPU6502_test();

	int instrCnt = 0;
	ppu = new PPU(memory);
	cpu = new CPU6502(memory,ppu);
	/* load the Super Mario Bros ROM */
#ifndef TEST
	rom = new ROM("../../roms/Super Mario Bros. (E).nes");
#else
	rom = new ROM("../../roms/nestest.nes");
	cpu->SetPC(0xc000);
#endif
	/* and copy the two PGR-ROM banks */
	rom->CopyPRG(0, memory);
	rom->CopyPRG(1, memory);
	rom->CopyCHR((unsigned char*)ppu->GetMemoryPtr());

#ifdef TEST
	for (std::vector<CPU6502State>::iterator it = cpuTest->groundTruth.begin();
		it != cpuTest->groundTruth.end(); ++it)
	{
		CPU6502State gtState = *it;
		CPU6502State currentState = cpu->GetState();

		if (!gtState.IsEqual(currentState))
		{
			cout << "inconsistent state" << endl;
			exit(0);
		}

		cpu->Step();
	}
#endif


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
