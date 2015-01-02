#pragma once

#include <string>
#include <vector>

using namespace std;

class ROM
{
public:
	ROM();
	ROM(string fileName);

	void CopyPGR(int bank, unsigned char *memory);

	~ROM();

private:
	vector<char> data;
};

