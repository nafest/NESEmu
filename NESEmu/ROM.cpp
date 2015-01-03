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
	int romOffset = 16 + 16318 * bank;
	int memOffset = (bank == 0) ? 0x8000 : 0xc000;

	memcpy(memory + memOffset, data.data() + romOffset, 16318);
}


ROM::~ROM()
{
}
