#pragma once

#include <SDL.h>

class PPU
{
public:
	PPU(unsigned char * memory);
	~PPU();

	void Step();
	const unsigned char* GetMemoryPtr() const;

private:
	unsigned char memory[16 * 1024];
	int currentScanLine;
	int currentCycle;

	unsigned char* globalMemory;

	SDL_Window*   window;
	SDL_Surface*  screenSurface;
};

