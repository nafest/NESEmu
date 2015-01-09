#include "PPU.h"
#include <SDL.h>


PPU::PPU(unsigned char *memory) {
	currentScanLine = 261;
	currentCycle = 0;
	globalMemory = memory;

	SDL_Init(SDL_INIT_VIDEO);

	//window = SDL_CreateWindow("NESEmu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 256, 240, SDL_WINDOW_SHOWN);

	//screenSurface = SDL_GetWindowSurface(window);

	SDL_CreateWindowAndRenderer(256, 240, 0, &window, &renderer);


	unsigned char myPal[3 * 64] = { 124, 124, 124,
		0, 0, 252,
		0, 0, 188,
		68, 40, 188,
		148, 0, 132,
		168, 0, 32,
		168, 16, 0,
		136, 20, 0,
		80, 48, 0,
		0, 120, 0,
		0, 104, 0,
		0, 88, 0,
		0, 64, 88,
		0, 0, 0,
		0, 0, 0,
		0, 0, 0,
		188, 188, 188,
		0, 120, 248,
		0, 88, 248,
		104, 68, 252,
		216, 0, 204,
		228, 0, 88,
		248, 56, 0,
		228, 92, 16,
		172, 124, 0,
		0, 184, 0,
		0, 168, 0,
		0, 168, 68,
		0, 136, 136,
		0, 0, 0,
		0, 0, 0,
		0, 0, 0,
		248, 248, 248,
		60, 188, 252,
		104, 136, 252,
		152, 120, 248,
		248, 120, 248,
		248, 88, 152,
		248, 120, 88,
		252, 160, 68,
		248, 184, 0,
		184, 248, 24,
		88, 216, 84,
		88, 248, 152,
		0, 232, 216,
		120, 120, 120,
		0, 0, 0,
		0, 0, 0,
		252, 252, 252,
		164, 228, 252,
		184, 184, 248,
		216, 184, 248,
		248, 184, 248,
		248, 164, 192,
		240, 208, 176,
		252, 224, 168,
		248, 216, 120,
		216, 248, 120,
		184, 248, 184,
		184, 248, 216,
		0, 252, 252,
		248, 216, 248,
		0, 0, 0,
		0, 0, 0 };

	for (int i = 0; i < 3 * 64; i++)
		palette[i] = myPal[i];
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
		if (currentCycle == 0)
		{
			/* get the first tile in L0 */
			/* L0 has 32x30 bytes */
			unsigned char tile = memory[0x2000];
			unsigned char attributes = memory[0x23c0];
			/* the tile consist of 16 bytes */
			/* get the first line */

			unsigned char attbits = (attributes & 0x03) << 2;

			for (int j = 0; j < 8; j++)
			{
				unsigned char tlow = memory[16 * tile + 0 + j];
				unsigned char thigh = memory[16 * tile + 8 + j];
				for (int i = 0; i < 8; i++)
				{
					unsigned char pdx = ((tlow >> (7 - i)) & 1) | (((thigh >> (7 - i)) & 1) << 1);
					pdx |= attbits;

					unsigned char color = memory[0x3f00 + pdx];

					SDL_SetRenderDrawColor(renderer, palette[3 * color], palette[3 * color + 1], palette[3 * color + 2], 255);
					SDL_RenderDrawPoint(renderer, i, j);

				}
			}

			SDL_RenderPresent(renderer);
		}
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
