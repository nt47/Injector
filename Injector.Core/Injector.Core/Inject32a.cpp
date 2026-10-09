#include"pch.h"
#include"Inject32a.h"
#include"Injector32.h"
#include<iostream>

//legacy for test
BOOL Inject32a(int PID, const wchar_t* dllPath)//这个路径是个绝对路径
{
	const char* testPath = "C:\\Users\\Pudge\\source\\repos\\Dpi\\FixDpi.Monitor\\FixDpi.Monitor\\bin\\Release\\HiJack32.dll";
	TOKEN_PRIVILEGES priv = { 0 };
	HANDLE hToken = NULL;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		priv.PrivilegeCount = 1;
		priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &priv.Privileges[0].Luid))
			AdjustTokenPrivileges(hToken, FALSE, &priv, 0, NULL, NULL);

		CloseHandle(hToken);
	}

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (!hProcess) {
		DWORD Err = GetLastError();
		wprintf(L"OpenProcess failed: 0x%X\n", Err);
		return FALSE;
	}


	// 在目标进程中分配内存以存储DLL路径
	LPVOID pDllPath = VirtualAllocEx(hProcess, NULL, strlen(testPath) + 1, MEM_COMMIT, PAGE_READWRITE);
	if (pDllPath == NULL) {
		std::wcout << "Failed to allocate memory in target process" << std::endl;
		CloseHandle(hProcess);
		return FALSE;
	}

	// 写入DLL路径到目标进程内存
	if (!WriteProcessMemory(hProcess, pDllPath, testPath, strlen(testPath) + 1, NULL)) {
		std::wcout << "Failed to write DLL path to target process memory" << std::endl;
		VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return FALSE;
	}

	// 获取LoadLibraryA函数地址
	//LPVOID pLoadLibrary = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

	LPVOID pLoadLibrary = (LPVOID)Injector32::GetProcAddressIn32BitProcess(hProcess, "kernel32.dll", "LoadLibraryA");

	if (pLoadLibrary == NULL) {
		std::wcout << "Failed to get address of LoadLibraryA" << std::endl;
		VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return FALSE;
	}

	// 在目标进程中创建远程线程执行LoadLibraryA函数
	HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, pDllPath, 0, NULL);
	if (hRemoteThread == NULL) {
		std::wcout << "Failed to create remote thread in target process" << std::endl;
		VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return FALSE;
	}

	std::wcout << "WaitForSingleObject" << std::endl;
	// 等待远程线程执行完毕
	WaitForSingleObject(hRemoteThread, INFINITE);

	// 清理资源
	VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
	CloseHandle(hRemoteThread);
	CloseHandle(hProcess);


	return TRUE;
}