#pragma once
#pragma once
#include <Windows.h>
#include <iostream>
#include "Memory.h"


constexpr const char* retcheckBytes = "72 ? A1 ? ? ? ? 8B 4D 04 2B C8";

namespace CallingConventions
{
	enum CallingConvention
	{
		Cdecl,
		Stdcall,
		Fastcall,
		Thiscall
	};

}

template<class T, class T2 = int>
class FunctionWrapper
{
	T func;
	void* retcheck;
	bool spoofSEH;
	bool retcheckEnabled;
	bool isnewthread;

public:
	std::uintptr_t address;
	CallingConventions::CallingConvention callingConvention;
	template<class ... Arguments>
	T2 operator()(Arguments... args)
	{
		const auto reg = reinterpret_cast<PEXCEPTION_REGISTRATION_RECORD>(__readfsdword(0));
		const auto backup = reg->Handler;
		if (spoofSEH)
		{
			reg->Handler = reinterpret_cast<PEXCEPTION_ROUTINE>(func);
		}

		if (retcheckEnabled)
		{
			Memory::Write<unsigned char>(retcheck, 0xEB);
		}

		T2 ret = 0;
		switch (callingConvention)
		{
		case CallingConventions::Cdecl:
			ret = ((T2(__cdecl*)(Arguments...))func)(args...);
			break;
		case CallingConventions::Stdcall:
			ret = ((T2(__stdcall*)(Arguments...))func)(args...);
			break;
		case CallingConventions::Fastcall:
			ret = ((T2(__fastcall*)(Arguments...))func)(args...);
			break;
		case CallingConventions::Thiscall:
			ret = ((T2(__thiscall*)(Arguments...))func)(args...);
			break;
		}

		if (retcheckEnabled)
		{
			Memory::Write<unsigned char>(retcheck, 0x72);
		}

		if (spoofSEH)
		{
			reg->Handler = backup;
		}

		return ret;
	}

	void SetFunc(std::uintptr_t func, bool spoofSEH = false, bool retcheckEnabled = true, CallingConventions::CallingConvention ThisCallingConvention = CallingConventions::CallingConvention::Cdecl)
	{
		this->address = func;
		this->func = reinterpret_cast<T>(func);
		this->retcheckEnabled = retcheckEnabled;
		if (retcheckEnabled)
		{
			retcheck = reinterpret_cast<void*>(Memory::Scan(func, 0, retcheckBytes));
		}
		this->spoofSEH = spoofSEH;
		this->callingConvention = ThisCallingConvention;
	}

	template<class T3>
	T3 GetFunc()
	{
		return reinterpret_cast<T3>(func);
	}

	FunctionWrapper(T func, bool spoofSEH = false)
	{
		SetFunc(func, spoofSEH);
	}

	FunctionWrapper() {}
};