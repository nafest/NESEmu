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


ROM::~ROM()
{
}
