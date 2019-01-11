// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <future>
#include <LogLib/Log.h>
#include <ProcessLib/Memory/Memory.h>
#include "GameEnviroment.h"
#include "TextExpr.h"


#define _SELF L"dllmain.cpp"

#ifdef _DEBUG
#pragma comment(lib,"LogLib_x64_Debug.lib")
#pragma comment(lib,"CharacterLib_x64_Debug.lib")
#pragma comment(lib,"ProcessLib_x64_Debug.lib")
#else
#pragma comment(lib,"LogLib_x64.lib")
#pragma comment(lib,"CharacterLib_x64.lib")
#pragma comment(lib,"ProcessLib_x64.lib")
#endif // _DEBUG



extern "C" VOID WINAPIV HookPullCard();
extern "C" VOID HookPullCALL_CallBack()
{
	if (libTools::CMemory::ReadDWORD(0x1410d7fc4) != 1)
		return;

	WORD wdValue = libTools::CMemory::ReadWORD(0x1410d7fc0) >> 0xF;
	if (wdValue != 0)
		return;


	auto ulCardCountPtr = libTools::CMemory::ReadMemValue(0x1410d7fc4);

	// remove low Value
	ulCardCountPtr >>= 0x20;
	ulCardCountPtr <<= 0x20;
	ulCardCountPtr += 2;
	*reinterpret_cast<DWORD64*>(0x1410d7fc4) = ulCardCountPtr;
}

DWORD WINAPI _Initializer(LPVOID)
{
	static CTextExpr Instance;

	libTools::CCmdLog::GetInstance().Run(L"YuGiOh", Instance.GetVec());
	libTools::CLog::GetInstance().SetClientName(L"YuGiOh", L"Z:\\");
	


	// Hook Pull Card.  Automaic set Pull Card Count = 2 when Round = self
	DWORD64 dwProcAddr = reinterpret_cast<DWORD64>(HookPullCard);
	LOG_C_D(L"dwProcAddr=[%llX]", dwProcAddr);

	
	DWORD64 ulHookAddr = 0x14049BCFC;

	DWORD dwOldProject = 0;
	::VirtualProtect(reinterpret_cast<LPVOID>(ulHookAddr), 8 * 2, PAGE_EXECUTE_READWRITE, &dwOldProject);

	DWORD64 dwOpCode = (reinterpret_cast<DWORD64>(HookPullCard) << 0x10) + 0xB848;
	*reinterpret_cast<DWORD64*>(ulHookAddr) = dwOpCode;
	*reinterpret_cast<DWORD64*>(ulHookAddr + 0x8) = 0x90909090E0FF0000;
	return 0;
}

BOOL APIENTRY DllMain( HMODULE ,
                       DWORD  ul_reason_for_call,
                       LPVOID 
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)_Initializer, NULL, NULL, NULL);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

