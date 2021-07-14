//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "ScopeGuard.h"

//#ifdef LTK_EXPORTS
//#define LTK_CPP_API __declspec(dllexport)
//#else
//#define LTK_CPP_API __declspec(dllimport)
//#endif
#define LTK_CPP_API

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&) = delete; \
	void operator=(const TypeName&) = delete;

#ifdef _DEBUG
#define LTK_ASSERT(expr) if (!(expr)) {__debugbreak();} // super KISS
#else
#define LTK_ASSERT(expr) (void)(expr)
#endif


LTK_CPP_API std::wstring Utf8ToUtf16(LPCSTR strA, int len = -1);
std::string Utf16ToUtf8(LPCWSTR strW, int len);
std::string Utf16ToGbk(LPCWSTR strW, int len);

std::wstring LtkA2W(LPCSTR strA, int len = -1);
std::string LtkW2A(LPCWSTR strW, int len = -1);

LTK_CPP_API std::wstring WStringFormat(LPCWSTR format, ...);


#define LOGW(msg) do\
{\
	std::wstringstream ss;\
	ss << __FUNCTIONW__ << L"() " msg << std::endl;\
	OutputDebugStringW(ss.str().c_str());\
} while (0)

// https://stackoverflow.com/questions/3060006/is-it-worth-setting-pointers-to-null-in-a-destructor
#ifndef INVALID_POINTER
#define INVALID_POINTER(type) reinterpret_cast<type *>(0xDEADBEEF)
#endif

#define SAFE_RELEASE(p) if (p) { (p)->Release(); (p) = nullptr; }

#define RELEASE_AND_INVALIDATE(type, p) if (p) { (p)->Release(); (p) = reinterpret_cast<type *>(0xDEADBEEF); }

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

//#define LTK_ASSERT(expr) if (!(expr)) {\
//    CStringW msg; msg.Format(L"Assertion Failed: %s\r\n%s(%d)", L#expr, __FILEW__, __LINE__);\
//    ::OutputDebugStringW(msg);__debugbreak();}

class CriticalSection
{
public:
	friend class AutoLock;

	CriticalSection()
	{
		::InitializeCriticalSection(&m_sec);
	}
	~CriticalSection()
	{
		::DeleteCriticalSection(&m_sec);
	}

private:
	CRITICAL_SECTION m_sec;
};

class AutoLock
{
public:
	AutoLock(CriticalSection &cs)
		: m_cs(cs)
	{
		::EnterCriticalSection(&m_cs.m_sec);
	}
	~AutoLock()
	{
		::LeaveCriticalSection(&m_cs.m_sec);
	}

private:
	CriticalSection &m_cs;
};


enum LtkLogLevel {
	logInfo, logWarn, logError, logFatal
};

#define  LTK_LOG(...) LtkLogImpl(__FILE__, __LINE__, __VA_ARGS__)

LTK_CPP_API void LtkLogInit();
LTK_CPP_API void LtkLogImpl(const char *source, int line, const char *format, ...);
