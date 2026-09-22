#pragma once
#include"Macros.h"
namespace CLR {
	namespace Native {
		BOOL InjectWithEvent(int PID, const wchar_t* dllPath, const wchar_t* eventId);
	}
}