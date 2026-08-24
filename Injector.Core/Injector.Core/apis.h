#pragma once
#include"Macros.h"
#include<Windows.h>
#include<string>
#include<tchar.h>

API DLLEXPORT void Console();//being used

bool IsCorrectTargetArchitecture(HANDLE hProc);
bool Is32BitExe(const wchar_t* exePath);
int GetArchitecture(const wchar_t* dllPath);//being used
BOOL DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath);
BOOL GetProcessFullPath(HANDLE hProcess, std::wstring& fullPath);
bool IsProcess32Bit(HANDLE hProcess);
BOOL IsProcess64Bit(HANDLE hProcess);//being used
API DLLEXPORT BOOL isExistProcess(DWORD process_id);//being used
API DLLEXPORT bool IsDebuggerAttached(DWORD PID);//being used
