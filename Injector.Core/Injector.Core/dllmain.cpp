// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"


#if defined(DISABLE_OUTPUT)
#define Msg(data, ...)
#else
#define Msg(text, ...) wprintf(text, __VA_ARGS__);
#endif

using namespace std;


//如果开启CLR会导致平台不兼容而导致注入的目标进程崩溃，删除DllMain也没用
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

