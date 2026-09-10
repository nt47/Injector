#pragma once
#include"Macros.h"
#pragma comment(lib,"Injector.Core.X64.lib")

using namespace System;
using namespace System::Runtime::InteropServices;//为了用Marshal




namespace CLR {

	namespace Native {
		//先编译出dll，再编译lib，警告消失
		//静态lib导出也接纳
		API DLLIMPORT void Console();
		API DLLIMPORT BOOL Inject(int PID, const wchar_t* dllPath);
		BOOL InjectWithEvent(int PID, const wchar_t* dllPath, const wchar_t* eventId);

	}

	public ref class  Injector {

	public:
		static void Console();
		static BOOL Inject(int PID, String^ dllPath);
		static BOOL InjectWithEvent(int PID, String^ dllPath, String^ eventId);
	};

}



