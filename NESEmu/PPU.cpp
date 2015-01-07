#include "PPU.h"
#include <SDL.h>


PPU::PPU(unsigned char *memory) {
	currentScanLine = 261;
	currentCycle = 0;
	globalMemory = memory;

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("NESEmu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 256, 240, SDL_WINDOW_SHOWN);

	screenSurface = SDL_GetWindowSurface(window);
}


PPU::~PPU() {
}

const unsigned char * PPU::GetMemoryPtr() const
{
	return &(memory[0]);
}

void PPU::WriteCtrl1(unsigned char value)
{}

void PPU::WriteCtrl2(unsigned char value)
{}

void PPU::WriteSPRAddress(unsigned char value)
{}

void PPU::WriteSPRIO(unsigned char value)
{}

void PPU::WriteVRAMAddress1(unsigned char value)
{}

void PPU::WriteVRAMAddress2(unsigned char value)
{}

void PPU::WriteVRAMIO(unsigned char value)
{}

unsigned char PPU::ReadStatus()
{
	return status;
}

void PPU::Step() {

	if (currentScanLine == 261) {
		/* pre-render */
		/* unset status bit 7 */
		status &= (0xff ^ (1 << 7));
	} else if (currentScanLine >= 0 && currentScanLine <= 239) {
		/* unset status bit 7 */
		status &= (0xff ^ (1 << 7));
	}
	else if (currentScanLine == 240) {
		/* unset status bit 7*/
		status &= (0xff ^ (1 << 7));
	}
	else if (currentScanLine >= 240 && currentScanLine < 261)
	{
		status |= 1 << 7;
	}

	globalMemory[0x2002] = status;

	currentCycle += 1;

	if (currentCycle == 341) {
		currentCycle = 0;
		currentScanLine += 1;

		if (currentScanLine == 262)
			currentScanLine = 0;
	}

}
