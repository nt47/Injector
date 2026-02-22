#pragma once
#include"Macros.h"
#pragma comment(lib,"Injector.Core.X64.lib")

using namespace System;
using namespace System::Runtime::InteropServices;//Œ™¡À”√Marshal

namespace CLR {

	namespace Native {

		API DLLIMPORT void Console();
		API DLLIMPORT BOOL Inject(int PID, const wchar_t* dllPath);

	}

	public ref class  Injector {

	public:
		static void Console();
		static BOOL Inject(int PID, String^ dllPath);
	};

}



