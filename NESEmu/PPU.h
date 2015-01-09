#pragma once

#include <SDL.h>

class PPU
{
public:
	PPU(unsigned char * memory);
	~PPU();

	void Step();

	void WriteCtrl1(unsigned char value);
	void WriteCtrl2(unsigned char value);
	void WriteSPRAddress(unsigned char value);
	void WriteSPRIO(unsigned char value);
	void WriteVRAMAddress1(unsigned char value);
	void WriteVRAMAddress2(unsigned char value);
	void WriteVRAMIO(unsigned char value);

	unsigned char ReadStatus();

	const unsigned char* GetMemoryPtr() const;

private:
	unsigned char memory[16 * 1024];

	unsigned char palette[3 * 64];

	unsigned char ctrl1;
	unsigned char ctrl2;
	unsigned char status;

	int currentScanLine;
	int currentCycle;

	unsigned char* globalMemory;

	SDL_Window*   window;
	SDL_Renderer* renderer;
	SDL_Surface*  screenSurface;
};

