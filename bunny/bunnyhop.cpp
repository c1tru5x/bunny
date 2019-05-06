#include "bunnyhop.h"
#include <Windows.h>
#include "connector.h"

CHackProcess fProcess;

bunny::bunny()
{
    #define onGround		257
    #define crouchedGround	263

	const DWORD dwPlayerBase = 0xCBD6B4;
	const DWORD dwJump = 0x5170DB0;
	const DWORD m_fFlags = 0x104;

	DWORD dwLocalP = 0x0;

		
	int flag = 0;
		
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + dwPlayerBase), &dwLocalP, sizeof(DWORD), 0);
    ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(dwLocalP + m_fFlags), &flag, sizeof(int), 0);


	int doJump = 5;
	int stopJump = 4;

	if (flag == onGround || flag == crouchedGround)
	{
		WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + dwJump), &doJump, sizeof(doJump), 0);
	}
	else
	{
		WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + dwJump), &stopJump, sizeof(stopJump), 0);
	}
}

bunny::~bunny()
{

}