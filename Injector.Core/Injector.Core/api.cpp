#include"pch.h"
#include"api.h"
#include <locale>
#include<iostream>
#include<Psapi.h>
#include<TlHelp32.h>


#if defined(DISABLE_OUTPUT)
#define Msg(data, ...)
#else
#define Msg(text, ...) wprintf(text, __VA_ARGS__);
#endif

using namespace std;

API DLLEXPORT void Console()
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

bool IsCorrectTargetArchitecture(HANDLE hProc) {
	BOOL bTarget = FALSE;
	if (!IsWow64Process(hProc, &bTarget)) {//此函数检测目标进程是否为32位进程
		Msg(L"Can't confirm target process architecture: 0x%X\n", GetLastError());
		return false;
	}

	BOOL bHost = FALSE;
	IsWow64Process(GetCurrentProcess(), &bHost);

	return (bTarget == bHost);
}

bool Is32BitExe(const wchar_t* exePath) {

	DWORD dwFileType;

	BOOL bRet = GetBinaryType(exePath, &dwFileType);
	if (!bRet)
	{
		DWORD Err = GetLastError();
		Msg(L"GetBinaryType failed: 0x%X\n", Err);
	}
	if (dwFileType == SCS_32BIT_BINARY)
		return true;

	return false;
}

int GetArchitecture(const wchar_t* dllPath) {
	// 打开DLL文件
	HANDLE hFile = CreateFile(dllPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		std::cerr << "Failed to open DLL file. Error code: " << GetLastError() << std::endl;
		return false;
	}

	// 读取PE头
	IMAGE_DOS_HEADER dosHeader;
	DWORD bytesRead;
	if (!ReadFile(hFile, &dosHeader, sizeof(IMAGE_DOS_HEADER), &bytesRead, NULL)) {
		std::cerr << "Failed to read DOS header. Error code: " << GetLastError() << std::endl;
		CloseHandle(hFile);
		return false;
	}

	// 验证PE头
	if (dosHeader.e_magic != IMAGE_DOS_SIGNATURE) {
		std::cerr << "Invalid DOS header signature." << std::endl;
		CloseHandle(hFile);
		return false;
	}

	// 移动文件指针到PE头
	if (SetFilePointer(hFile, dosHeader.e_lfanew, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
		std::cerr << "Failed to move file pointer to PE header. Error code: " << GetLastError() << std::endl;
		CloseHandle(hFile);
		return false;
	}

	// 读取PE头的签名
	DWORD peSignature;
	if (!ReadFile(hFile, &peSignature, sizeof(DWORD), &bytesRead, NULL)) {
		std::cerr << "Failed to read PE signature. Error code: " << GetLastError() << std::endl;
		CloseHandle(hFile);
		return false;
	}

	// 验证PE头的签名
	if (peSignature != IMAGE_NT_SIGNATURE) {
		std::cerr << "Invalid PE signature." << std::endl;
		CloseHandle(hFile);
		return false;
	}

	// 读取PE头
	IMAGE_FILE_HEADER fileHeader;
	if (!ReadFile(hFile, &fileHeader, sizeof(IMAGE_FILE_HEADER), &bytesRead, NULL)) {
		std::cerr << "Failed to read file header. Error code: " << GetLastError() << std::endl;
		CloseHandle(hFile);
		return false;
	}

	// 检查文件头的位数信息
	if (fileHeader.Machine == IMAGE_FILE_MACHINE_I386) {
		std::cout << "The DLL is 32-bit." << std::endl;
	}
	else if (fileHeader.Machine == IMAGE_FILE_MACHINE_AMD64) {
		std::cout << "The DLL is 64-bit." << std::endl;
	}
	else {
		std::cout << "Unknown architecture." << std::endl;
	}

	// 关闭文件句柄
	CloseHandle(hFile);

	return fileHeader.Machine;
}


BOOL DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath)
{
	TCHAR			szDriveStr[500];
	TCHAR			szDrive[3];
	TCHAR			szDevName[100];
	INT				cchDevName;
	INT				i;

	//检查参数
	if (!pszDosPath || !pszNtPath)
		return FALSE;

	//获取本地磁盘字符串
	if (GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr))
	{
		for (i = 0; szDriveStr[i]; i += 4)
		{
			if (!lstrcmpi(&(szDriveStr[i]), _T("A:\\")) || !lstrcmpi(&(szDriveStr[i]), _T("B:\\"))) { continue; }

			szDrive[0] = szDriveStr[i];
			szDrive[1] = szDriveStr[i + 1];
			szDrive[2] = '\0';
			// 查询 Dos 设备名
			if (!QueryDosDevice(szDrive, szDevName, 100)) { return FALSE; }

			// 命中
			cchDevName = lstrlen(szDevName);
			if (_tcsnicmp(pszDosPath, szDevName, cchDevName) == 0) {
				// 复制驱动器
				lstrcpy(pszNtPath, szDrive);

				// 复制路径
				lstrcat(pszNtPath, pszDosPath + cchDevName);

				return TRUE;
			}
		}
	}

	lstrcpy(pszNtPath, pszDosPath);

	return FALSE;
}

// 获取进程全路径
BOOL GetProcessFullPath(HANDLE hProcess, wstring& fullPath) {
	TCHAR		szImagePath[MAX_PATH];
	TCHAR		pszFullPath[MAX_PATH];

	// 初始化失败
	if (!pszFullPath) { return FALSE; }
	pszFullPath[0] = '\0';


	// 获取进程完整路径失败
	if (!GetProcessImageFileName(
		hProcess,					// 进程句柄
		szImagePath,				// 接收进程所属文件全路径的指针
		MAX_PATH					// 缓冲区大小
	)) {
		CloseHandle(hProcess);
		return FALSE;
	}

	// 路径转换失败
	if (!DosPathToNtPath(szImagePath, pszFullPath)) {

		return FALSE;
	}

	// 导出文件全路径
	fullPath = pszFullPath;

	return TRUE;
}

bool IsProcess32Bit(HANDLE hProcess)
{
	wstring fullPath;
	GetProcessFullPath(hProcess, fullPath);

	Msg(L"%s\n", fullPath.c_str());

	return Is32BitExe(fullPath.c_str());
}

BOOL IsProcess64Bit(HANDLE hProcess)
{
	// 判断64位系统下, 进程指定是32位还是64位
	BOOL bWow64Process = FALSE;

	// 判断进程是否处于WOW64仿真环境中
	IsWow64Process(hProcess, &bWow64Process);

	return !bWow64Process;
}

//判断进程id是否存在
//@param:process_id:需要传入的进程id值
//return:True:存在，False:不存在
API DLLEXPORT BOOL isExistProcess(DWORD process_id)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return NULL;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	for (BOOL ret = Process32First(hSnapshot, &pe); ret; ret = Process32Next(hSnapshot, &pe)) {

		if (pe.th32ProcessID == process_id)
		{
			return TRUE;
		}
	}
	CloseHandle(hSnapshot);
	return FALSE;
}

API DLLEXPORT bool IsDebuggerAttached(DWORD PID) {

	// 打开目标进程
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (!hProcess) {
		DWORD Err = GetLastError();
		Msg(L"OpenProcess failed: 0x%X\n", Err);
		return FALSE;
	}

	// 检测目标进程是否被调试器附加
	BOOL isDebuggerPresent = FALSE;
	if (!CheckRemoteDebuggerPresent(hProcess, &isDebuggerPresent)) {
		Msg(L"Failed to check debugger presence");
		CloseHandle(hProcess);
		return false;
	}

	// 关闭目标进程句柄
	CloseHandle(hProcess);

	return isDebuggerPresent ? true : false;
}