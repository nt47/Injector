#pragma once
#include"Macros.h"

namespace Core {
	BOOL InjectWithEvent(int PID, const wchar_t* dllPath, const wchar_t* eventId);
}