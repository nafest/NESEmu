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

	vramAdress = 0x0;
	vramAdressLatch = true;
	addrXIsNext = true;

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
{
	ctrl1 = value;
}

void PPU::WriteCtrl2(unsigned char value)
{
	ctrl2 = value;
}

void PPU::WriteSPRAddress(unsigned char value)
{
	sprAdress = value;
}

void PPU::DMAtoSPR(unsigned char *addr)
{
	for (int i = 0; i < 256; i++)
		sprmemory[i] = addr[i];
}

void PPU::WriteSPRIO(unsigned char value)
{
	sprmemory[sprAdress] = value;
}

void PPU::WriteVRAMAddress1(unsigned char value)
{
	if (addrXIsNext)
		addrX = value;
	else
		addrY = value;

	addrXIsNext = !addrXIsNext;
}

void PPU::WriteVRAMAddress2(unsigned char value)
{
	if (vramAdressLatch)
	{
		/* clear the upper 8 bits*/
		vramAdress &= 0x00ff;
		vramAdress |= (value << 8);
	}
	else {
		vramAdress &= 0xff00;
		vramAdress |= value;
	}
	vramAdressLatch = !vramAdressLatch;
}

void PPU::WriteVRAMIO(unsigned char value)
{
	memory[vramAdress] = value;
	if (ctrl1 & (1 << 2))
		vramAdress += 32;
	else
		vramAdress++;
}

unsigned char PPU::ReadStatus()
{
	return status;
}

Tile PPU::fetchSpriteTile(int spriteidx)
{
	Tile     tile;

    unsigned char patternTable = (ctrl1 & (1 << 3)) ? 0x1000 : 0x0000;

	unsigned short spriteAddress = 4*spriteidx;
	tile.Y = sprmemory[spriteAddress];
	tile.X = sprmemory[spriteAddress + 3];
	tile.attBits = sprmemory[spriteAddress + 2];

	unsigned short patternAddress = patternTable + 16 * sprmemory[spriteAddress + 1];

	for (int i = 0; i < 8; i++)
	{
		tile.lowBits[i] = memory[patternAddress + i];
		tile.highBits[i] = memory[patternAddress + 8 + i];
	}

	tile.idx = spriteidx;

	return tile;
}

Tile PPU::fetchBGTile(int nameTableIdx, int x, int y)
{
	Tile     tile;
	unsigned short baseAddress = 0x2000 + nameTableIdx*0x400;
    unsigned short tileAddress = baseAddress + y * 32 + x;
	unsigned short patternTable = 0x0000;

    patternTable = (ctrl1 & (1 << 4)) ? 0x1000 : 0x0000;

	unsigned short patternAddress = memory[tileAddress]*16 + patternTable;

	for (int i = 0; i < 8; i++)
	{
		tile.lowBits[i] = memory[patternAddress + i];
		tile.highBits[i] = memory[patternAddress + 8 + i];
	}

	unsigned short attrAddress = baseAddress + 0x3c0 + (y / 4) * 8 + (x / 4);

	unsigned short attributes = memory[attrAddress];

	int square = 0;

	if (y % 4 > 1)
		square += 2;
	if (x % 4 > 0)
		square += 1;

	/* attr contains the bits in 33221100 */
	tile.attBits = (attributes >> (2 * square)) & 0x3;

	return tile;
}

void PPU::Step() {

	if (currentScanLine == 261) {
		/* pre-render */
		/* unset status bit 7 */
		status &= (0xff ^ (1 << 7));
	} else if (currentScanLine >= 0 && currentScanLine <= 239) {
		if (currentScanLine == 0 && currentCycle == 0)
		{
			// unset hit flag
			status &= (0xff ^ (1 << 6));
		}

		if (currentCycle == 0)
		{
			scanlineSprites.clear();
			for (int sdx = 0; sdx < 64; sdx++)
			{
				Tile sprTile = fetchSpriteTile(sdx);
				
				if (sprTile.Y + 8 >= currentScanLine && (sprTile.Y+1) <= currentScanLine
					&& scanlineSprites.size() < 8)
					scanlineSprites.push_back(sprTile);
			}
		}
		if (currentCycle < 256)
		{
			int tx = (currentCycle+addrX) / 8;
			int ty = currentScanLine / 8;

			bool bgIsTransparent = true;
			
			int nameTable = (ctrl1 & 0x3);

			if (tx > 31)
			{
				tx -= 32;
				if (nameTable == 0)
					nameTable++;
				else
					nameTable--;
			}

			/* fetch the background tile */
			Tile bgTile = fetchBGTile(nameTable, tx, ty);

			unsigned char attBits = bgTile.attBits << 2;

			unsigned char tlow = bgTile.lowBits[currentScanLine % 8];
			unsigned char thigh = bgTile.highBits[currentScanLine % 8];

			int x = (currentCycle+addrX) % 8;

			unsigned char pdx = ((tlow >> (7 - x)) & 1) | (((thigh >> (7 - x)) & 1) << 1);

			if (pdx == 0)
				bgIsTransparent = false;

			pdx |= attBits;

			unsigned char color = memory[0x3f00 + pdx];
			//color = pdx;

			SDL_SetRenderDrawColor(renderer, palette[3 * color], palette[3 * color + 1], palette[3 * color + 2], 255);
			SDL_RenderDrawPoint(renderer, currentCycle, currentScanLine);

			/* iterate over all sprites */

			for (auto it = scanlineSprites.begin(); it != scanlineSprites.end(); it++)
			{
				/* coordinate in the sprite tile */
				int x = currentCycle - (it->X+1);
				int y = currentScanLine - (it->Y+1);

				if (x < 0 || x > 7)
					continue;

				if ((it->attBits & (1 << 6)) != 0)
				{
					//mirror vertically;
					x = 7 - x;
				}

				if ((it->attBits & (1 << 7)) != 0)
				{
					// mirror horizontally;
					y = 7 - x;
				}

				tlow = it->lowBits[y];
				thigh = it->highBits[y];

				pdx = ((tlow >> (7 - x)) & 1) | (((thigh >> (7 - x)) & 1) << 1);

				if (pdx != 0 && !bgIsTransparent && it->idx == 0)
				{
					// set sprite 0 hit flag
					status |= 0x40; /* set hit flag*/
				}

				if (pdx == 0 || (ctrl2 & (1 << 4)) == 0)
					continue;

				unsigned char attBits = (it->attBits & 0x03) << 2;

				pdx |= attBits;

				unsigned char color = memory[0x3f10 + pdx];
				//color = pdx;

				SDL_SetRenderDrawColor(renderer, palette[3 * color], palette[3 * color + 1], palette[3 * color + 2], 255);
				SDL_RenderDrawPoint(renderer, currentCycle, currentScanLine);

			}


		}

		if (currentScanLine == 239 && currentCycle == 256)
		  SDL_RenderPresent(renderer);
	}
	else if (currentScanLine == 240) {
		/* unset status bit 7*/
		status &= (0xff ^ (1 << 7));
		if (currentCycle == 0)
		{
			if ((ctrl1 & (1 << 7)) != 0)
			{
				// set NMI
				cpu->SetInterrupt(eNMI);
			}
		}
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
