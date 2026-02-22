#include "pch.h"
#include"Injector.h"

namespace CLR {

	void Injector::Console()
	{
		return Native::Console();
	}
	BOOL Injector::Inject(int PID, String^ dllPath)
	{
		auto Uni = Marshal::StringToHGlobalUni(dllPath);
		const wchar_t* r = (wchar_t*)Uni.ToPointer();
		return Native::Inject(PID, r);
	}
}



