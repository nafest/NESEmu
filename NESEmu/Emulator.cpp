#include "Emulator.h"
#include "Controller.h"

#include "CPU6502_test.h"

//#define TEST

Emulator::Emulator()
{
	CPU6502_test  *cpuTest = new CPU6502_test();
	bool quit = false;
	SDL_Event event;

	SDL_Init(SDL_INIT_VIDEO);

	int instrCnt = 0;
	ppu = new PPU(memory);
	cpu = new CPU6502(memory,ppu);
	
	Controller *ctrl1 = new Controller();

	cpu->SetController1(ctrl1);

	/* load the Super Mario Bros ROM */
#ifndef TEST
	rom = new ROM("../../roms/Super Mario Bros. (E).nes");
#else
	rom = new ROM("../../roms/nestest.nes");
	cpu->SetPC(0xc000);
#endif
	/* and copy the two PGR-ROM banks */
	rom->CopyPRG(0, memory);
	rom->CopyPRG(1, memory);
	rom->CopyCHR((unsigned char*)ppu->GetMemoryPtr());

#ifdef TEST
	for (std::vector<CPU6502State>::iterator it = cpuTest->groundTruth.begin();
		it != cpuTest->groundTruth.end(); ++it)
	{
		CPU6502State gtState = *it;
		CPU6502State currentState = cpu->GetState();

		if (!gtState.IsEqual(currentState))
		{
			cout << "inconsistent state" << endl;
			exit(0);
		}

		cpu->Step();
	}
#endif

	while (!quit)
	{
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					ctrl1->SetUp(true);
					break;

				case SDLK_DOWN:
					ctrl1->SetDown(true);
					break;

				case SDLK_LEFT:
					ctrl1->SetLeft(true);
					break;

				case SDLK_RIGHT:
					ctrl1->SetRight(true);
					break;

				case SDLK_k:
					ctrl1->SetStart(true);
					break;

				case SDLK_l:
					ctrl1->SetSelect(true);
					break;

				case SDLK_LCTRL:
					ctrl1->SetA(true);
					break;

				case SDLK_LALT:
					ctrl1->SetB(true);


				}
				break;

			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					ctrl1->SetUp(false);
					break;

				case SDLK_DOWN:
					ctrl1->SetDown(false);
					break;

				case SDLK_LEFT:
					ctrl1->SetLeft(false);
					break;

				case SDLK_RIGHT:
					ctrl1->SetRight(false);
					break;

				case SDLK_k:
					ctrl1->SetStart(false);
					break;

				case SDLK_l:
					ctrl1->SetSelect(false);
					break;

				case SDLK_LCTRL:
					ctrl1->SetA(false);
					break;

				case SDLK_LALT:
					ctrl1->SetB(false);
					break;
				}
				break;
			}
		}

		int cpuCycles = cpu->Step();
		for (int i = 0; i < 3 * cpuCycles; i++)
			ppu->Step();

		instrCnt++;
	}

}


Emulator::~Emulator()
{
}
