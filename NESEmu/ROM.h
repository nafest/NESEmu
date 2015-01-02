#pragma once

#include <string>
#include <vector>

using namespace std;

class ROM
{
public:
	ROM();
	ROM(string fileName);

	~ROM();

private:
	vector<char> data;
};

