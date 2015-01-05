#pragma once

#include <SDL.h>

class PPU
{
public:
	PPU(unsigned char * memory);
	~PPU();

	void Step();

private:
	int currentScanLine;
	int currentCycle;

	unsigned char* globalMemory;

	SDL_Window*   window;
	SDL_Surface*  screenSurface;
};

