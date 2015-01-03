#pragma once
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
};

