#include"pch.h"
#include"InjectWithEvent.h"
#include<Windows.h>
#include<iostream>
#include"CAutoMutex.h"
#include"misc.h"
#include"apis.h"
#include"Injector32.h"
#include"Injector64.h"


#if defined(DISABLE_OUTPUT)
#define Msg(data, ...)
#else
#define Msg(text, ...) wprintf(text, __VA_ARGS__);
#endif


BOOL Core::InjectWithEvent(int PID, const wchar_t* dllPath, const wchar_t* eventId)
{
	CAutoMutex MutexLock;

	TOKEN_PRIVILEGES priv = { 0 };
	HANDLE hToken = NULL;
	EVENT_DATA event_data = {};

	g_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (g_hEvent == NULL) {
		std::cout << "Failed to create g_event. Error code: " << GetLastError() << std::endl;
		return false;
	}

	event_data.path = GetProcessPath(PID);
	event_data.event_id = eventId;
	CloseHandle(CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ShareMemory, (LPVOID)&event_data, NULL, NULL));

	// 等待进程事件信号
	WaitForSingleObject(g_hEvent, INFINITE);


	// 关闭进程事件句柄
	CloseHandle(g_hEvent);

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		priv.PrivilegeCount = 1;
		priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &priv.Privileges[0].Luid))
			AdjustTokenPrivileges(hToken, FALSE, &priv, 0, NULL, NULL);

		CloseHandle(hToken);
	}

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (!hProc) {
		DWORD Err = GetLastError();
		Msg(L"OpenProcess failed: 0x%X\n", Err);
		return FALSE;
	}

	if (GetFileAttributes(dllPath) == INVALID_FILE_ATTRIBUTES) {
		Msg(L"Dll file doesn't exist\n");
		return FALSE;
	}

	std::ifstream File(dllPath, std::ios::binary | std::ios::ate);

	if (File.fail()) {
		Msg(L"Opening the file failed: %X\n", (DWORD)File.rdstate());
		File.close();
		return FALSE;
	}

	auto FileSize = File.tellg();
	if (FileSize < 0x1000) {
		Msg(L"Filesize invalid.\n");
		File.close();
		return FALSE;
	}

	BYTE* pSrcData = new BYTE[(UINT_PTR)FileSize];
	if (!pSrcData) {
		Msg(L"Can't allocate dll file.\n");
		File.close();
		return FALSE;
	}

	File.seekg(0, std::ios::beg);
	File.read((char*)(pSrcData), FileSize);
	File.close();

	Msg(L"Mapping...\n");

	if (!IsProcess64Bit(hProc) && !IsProcess64Bit(GetCurrentProcess()))//如果目标进程是32位，并且自身进程也是32位
	{
		Msg(L"Prepare to inject Process 32-bit.\n");
		if (GetArchitecture(dllPath) == IMAGE_FILE_MACHINE_AMD64)//如果DLL是64位的
		{
			Msg(L"32-bit Process can't load 64-bit dll\n");//32位进程不能注入64位DLL
			return FALSE;
		}
		if (!Injector32::ManualMapDll(hProc, pSrcData, (SIZE_T)FileSize)) {
			delete[] pSrcData;
			CloseHandle(hProc);
			Msg(L"Error while mapping.\n");
			return FALSE;
		}
	}
	else if (!IsProcess64Bit(hProc) && IsProcess64Bit(GetCurrentProcess()))//否则如果目标进程是32位，并且自身进程是64位
	{
		Msg(L"Prepare to inject Process 32-bit.\n");
		if (GetArchitecture(dllPath) == IMAGE_FILE_MACHINE_AMD64)//如果DLL是64位的
		{
			Msg(L"32-bit Process can't be load 64-bit dll\n");//32位进程不能注入64位DLL
			return FALSE;
		}
		if (!Injector32::ManualMapDll(hProc, pSrcData, (SIZE_T)FileSize)) {
			delete[] pSrcData;
			CloseHandle(hProc);
			Msg(L"Error while mapping.\n");
			return FALSE;
		}
	}
	else if (IsProcess64Bit(hProc) && IsProcess64Bit(GetCurrentProcess()))//否则如果目标进程是64位，并且自身进程也是64位
	{
		Msg(L"Prepare to inject Process 64-bit.\n");
		if (GetArchitecture(dllPath) == IMAGE_FILE_MACHINE_I386)//如果DLL是32位的
		{
			Msg(L"64-bit Process can't load 32-bit dll\n");//64位进程不能注入32位DLL
			return FALSE;
		}

		if (!Injector64::ManualMapDll(hProc, pSrcData, (SIZE_T)FileSize)) {//采用64位手动映射注入
			delete[] pSrcData;
			CloseHandle(hProc);
			Msg(L"Error while mapping.\n");
			return FALSE;
		}
	}
	else if (IsProcess64Bit(hProc) && !IsProcess64Bit(GetCurrentProcess()))//否则如果目标进程是64位，并且自身进程也是32位
	{
		Msg(L"Prepare to inject Process 64-bit.\n");
		if (GetArchitecture(dllPath) == IMAGE_FILE_MACHINE_I386)//如果DLL是32位的
		{
			Msg(L"64-bit Process can't load 32-bit dll\n");//64位进程不能注入32位DLL
			return FALSE;
		}

		Msg(L"Not support 32-bit to 64-bit for the time being.\n");//暂时不支持
	}

	delete[] pSrcData;

	CloseHandle(hProc);

	Msg(L"Everything is OK\n");
	return TRUE;
}