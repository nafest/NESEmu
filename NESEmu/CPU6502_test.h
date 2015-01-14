#pragma once

#include "CPU6502.h"
#include <vector>

using namespace std;

class CPU6502_test
{
public:
	CPU6502_test();
	~CPU6502_test();

	void LoadGroundTruth();

public:
	vector<CPU6502State> groundTruth;
};

