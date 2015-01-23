#pragma once
class Controller
{
public:
	Controller()
	{
		stateA = false;
		stateB = false;
		stateLeft = false;
		stateRight = false;
		stateUp = false;
		stateDown = false;
		stateStart = false;
		stateSelect = false;

		nextState = 0;
	}
	~Controller();

	void SetA(bool v)
	{
		stateA = v;
	}

	bool GetA()
	{
		return stateA;
	}

	void SetB(bool v)
	{
		stateB = v;
	}

	bool GetB()
	{
		return stateB;
	}

	void SetStart(bool v)
	{
		stateStart = v;
	}

	void SetSelect(bool v)
	{
		stateSelect = v;
	}

	void SetLeft(bool v)
	{
		stateLeft = v;
	}

	void SetRight(bool v)
	{
		stateRight = v;
	}

	void SetUp(bool v)
	{
		stateUp = v;
	}

	void SetDown(bool v)
	{
		stateDown = v;
	}


	void ResetState()
	{
		nextState = 0;
	}

	bool ReadNextState()
	{
		nextState = nextState + 1;
		switch (nextState - 1)
		{
		case 0:
			return stateA;
	
		case 1:
			return stateB;

		case 2:
			return stateSelect;

		case 3:
			return stateStart;

		case 4:
			return stateUp;

		case 5:
			return stateDown;

		case 6:
			return stateLeft;

		case 7:
			return stateRight;
			
		}
	
		return false;
	}


private:
	bool stateA;
	bool stateB;
	bool stateLeft;
	bool stateRight;
	bool stateUp;
	bool stateDown;

	bool stateStart;
	bool stateSelect;

	int nextState;
};

