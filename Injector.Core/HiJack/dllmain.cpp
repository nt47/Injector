// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include<iostream>
#include"misc.h"

SHARED_DATA g_shared_data{};

void Console()
{
	wchar_t title[56];
	swprintf_s(title, L"Debug Window - %d", GetCurrentProcessId());

	setlocale(LC_ALL, "chs");
	AllocConsole();
	SetConsoleTitle(title);
	//freopen_s("CON", "w", stdout);
	FILE* consoleOutput;
	freopen_s(&consoleOutput, "CONOUT$", "w", stdout);
}

void Init()
{
	//Console();
	while (1)
	{
		if (ShareMemory(L"#002", &g_shared_data))
			break;
		Sleep(1000);
	}

	MessageBox(0, g_shared_data.target_exe_name, 0, 0);
	MessageBox(0, g_shared_data.target_exe_folder, 0, 0);

#ifdef _WIN64
	MessageBox(0, L"[64位] 你好，世界", L"成功注入", 0);
#else
	MessageBox(0, L"[32位] 你好，世界", L"成功注入", 0);
#endif

}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Init, NULL, 0, NULL);
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

