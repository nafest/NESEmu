#pragma once

#include <string>
#include <vector>

using namespace std;

class ROM
{
public:
	ROM();
	ROM(string fileName);

	void CopyPRG(int bank, unsigned char *memory);
	void CopyCHR(unsigned char *memory);

	~ROM();

private:
	vector<char> data;
};

