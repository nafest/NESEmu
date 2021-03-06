#include "ROM.h"
#include <fstream>

ROM::ROM() {
}

ROM::ROM(string fileName) {
	ifstream fstream(fileName, ios::binary);

	fstream.seekg(0, ios::end);
	streamsize size = fstream.tellg();
	fstream.seekg(0, ios::beg);

	data.resize(size);

	fstream.read(data.data(), size);
}

void ROM::CopyPRG(int bank, unsigned char * memory)
{
	int memOffset = (bank == 0) ? 0x8000 : 0xc000;

	if (bank >= data[4])
		bank = data[4] - 1;
	
	int romOffset = 16 + 16384 * bank;

	memcpy(memory + memOffset, data.data() + romOffset, 16384);
}

void ROM::CopyCHR(unsigned char * memory)
{
	int romOffset = 16 + 16384 * data[4];

	memcpy(memory, data.data() + romOffset, 8196);
}

ROM::~ROM()
{
}
