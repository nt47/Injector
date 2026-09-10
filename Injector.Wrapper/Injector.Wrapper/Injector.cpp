#include "pch.h"
#include"Injector.h"
#include<exception>
#include <vcclr.h>//PtrToStringChars
#include<string>
#include<msclr/marshal_cppstd.h>//marshal_as

using namespace System::Windows::Forms;
using namespace msclr::interop;

namespace CLR {

	void Injector::Console()
	{
		return Native::Console();
	}
	BOOL Injector::Inject(int PID, String^ dllPath)
	{
		IntPtr Uni = IntPtr::Zero;
		try {
			auto Uni = Marshal::StringToHGlobalUni(dllPath);
			const wchar_t* r = (wchar_t*)Uni.ToPointer();
			return Native::Inject(PID, r);
		}
		catch (Exception^ ex)
		{
			MessageBox::Show(ex->ToString(), "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		catch (std::exception ex)
		{
			MessageBoxA(0, ex.what(), "Error", MB_OK);
		}
		finally {
			if (Uni != IntPtr::Zero)
				Marshal::FreeHGlobal(Uni);
		}
		return FALSE;
	}
	BOOL Injector::InjectWithEvent(int PID, String^ dllPath, String^ eventId)
	{
		//pin_ptr<const wchar_t> raw_dllPath = PtrToStringChars(dllPath);
		//pin_ptr<const wchar_t> raw_eventId = PtrToStringChars(eventId);
		std::wstring raw_dllPath = marshal_as<std::wstring>(dllPath);
		std::wstring raw_eventId = marshal_as<std::wstring>(eventId);

		return CLR::Native::InjectWithEvent(PID, raw_dllPath.c_str(), raw_eventId.c_str());
	}
}



