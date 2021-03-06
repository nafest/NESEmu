#pragma once

#include <SDL.h>
#include "CPU6502.h"
#include <vector>

class Tile
{
public:
	unsigned char lowBits[8];
	unsigned char highBits[8];
	unsigned char attBits;

	unsigned char X, Y;

	unsigned char idx;

};

class PPU
{
public:
	PPU(unsigned char * memory);
	~PPU();

	void SetCPU(CPU6502 * cpu)
	{
		this->cpu = cpu;
	}

	void Step();

	void WriteCtrl1(unsigned char value);
	void WriteCtrl2(unsigned char value);
	void WriteSPRAddress(unsigned char value);
	void WriteSPRIO(unsigned char value);
	void WriteVRAMAddress1(unsigned char value);
	void WriteVRAMAddress2(unsigned char value);
	void WriteVRAMIO(unsigned char value);
	void DMAtoSPR(unsigned char *addr);

	Tile fetchBGTile(int nameTableIdx, int x, int y);
	Tile fetchSpriteTile(int spriteidx);
	unsigned char ReadStatus();

	const unsigned char* GetMemoryPtr() const;

private:
	CPU6502  *cpu;

	unsigned char memory[16 * 1024];
	unsigned char sprmemory[256];

	unsigned char palette[3 * 64];

	unsigned char ctrl1;
	unsigned char ctrl2;
	unsigned char status;

	unsigned char addrX;
	unsigned char addrY;

	bool addrXIsNext;

	vector<Tile> scanlineSprites;

	unsigned char sprAdress;
	
	bool vramAdressLatch;
	unsigned short vramAdress;

	int currentScanLine;
	int currentCycle;

	unsigned char* globalMemory;

	SDL_Window*   window;
	SDL_Renderer* renderer;
	SDL_Surface*  screenSurface;
};

