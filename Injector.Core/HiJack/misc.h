#pragma once
#include<Windows.h>

typedef struct _SHARED_DATA
{
	wchar_t src_exe_folder[MAX_PATH];
	wchar_t src_dll_folder[MAX_PATH];

	wchar_t target_exe_folder[MAX_PATH];
	wchar_t target_exe_name[MAX_PATH];

} SHARED_DATA, * PSHARED_DATA;


bool ShareMemory(const wchar_t* eventId, PSHARED_DATA new_shared_data);