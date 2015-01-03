#include "PPU.h"


PPU::PPU(unsigned char *memory) {
	currentScanLine = 261;
	currentCycle = 0;
	globalMemory = memory;
}


PPU::~PPU() {
}

void PPU::Step() {

	if (currentScanLine == 261) {
		/* pre-render */
		globalMemory[0x2002] = 0;
	} else if (currentScanLine >= 0 && currentScanLine <= 239) {
		globalMemory[0x2002] = 0;
	}
	else if (currentScanLine == 240) {
		globalMemory[0x2002] = 0;
	}
	else if (currentScanLine >= 240 && currentScanLine < 261)
	{
		globalMemory[0x2002] = 1 << 7;
	}

	currentCycle += 1;

	if (currentCycle == 341) {
		currentCycle = 0;
		currentScanLine += 1;

		if (currentScanLine == 262)
			currentScanLine = 0;
	}

}
