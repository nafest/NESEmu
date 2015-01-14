#include "CPU6502_test.h"

#include <string>
#include <fstream>
#include <sstream>

using namespace std;

CPU6502_test::CPU6502_test()
{
	LoadGroundTruth();
}


CPU6502_test::~CPU6502_test()
{
}


void CPU6502_test::LoadGroundTruth()
{
	string fileName = "../../roms/nestest.log";
	ifstream fstream(fileName);
	string line;

	while (getline(fstream, line))
	{
		istringstream iss(line);
		string tPC;

		iss >> tPC;

		// read until A:
		string token;
		while (1)
		{
			iss >> token;

			if (token.find("A:") == 0)
				break;
		}

		string tA, tX, tY, tP, tSP;
		tA = token;
		iss >> tX >> tY >> tP >> tSP;

		CPU6502State state;

		string hexVal = "0x" + tA.substr(2, 2);
		int val = stol(hexVal, 0, 16);
		state.A = val;

		hexVal = "0x" + tX.substr(2, 2);
		val = stol(hexVal, 0, 16);
		state.X = val;

		hexVal = "0x" + tY.substr(2, 2);
		val = stol(hexVal, 0, 16);
		state.Y = val;

		hexVal = "0x" + tP.substr(2, 2);
		val = stol(hexVal, 0, 16);
		state.P = val;

		hexVal = "0x" + tSP.substr(3, 2);
		val = stol(hexVal, 0, 16);
		state.SP = val;

		hexVal = "0x" + tPC;
		val = stol(hexVal, 0, 16);
		state.PC = val;

		groundTruth.push_back(state);
	}
}