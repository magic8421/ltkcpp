#pragma once
#include "ltk.h"

/*
** Copyright (C) 2015-2016 Chen Shi. See Copyright Notice in ltk.h
*/

std::wstring Utf8ToUtf16(LPCSTR strA, int len = -1);
CStringA Utf16ToUtf8(LPCTSTR strW, int len);
CStringA Utf16ToGbk(LPCTSTR strW, int len);


std::wstring WStringFormat(LPCWSTR format, ...);

class Deferred
{
public:
    explicit Deferred(const std::function<void()> &cb) : m_cb(cb)
    {}
    ~Deferred()
    {
        m_cb();
    }

private:
    std::function<void()> m_cb;
};

#define LOGW(msg) do\
{\
	std::wstringstream ss;\
	ss << __FUNCTIONW__ << L"() " msg << std::endl;\
	OutputDebugStringW(ss.str().c_str());\
} while (0)

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&);             \
	void operator=(const TypeName&)

// https://stackoverflow.com/questions/3060006/is-it-worth-setting-pointers-to-null-in-a-destructor
#ifndef INVALID_POINTER
#define INVALID_POINTER(type) reinterpret_cast<type *>(0xDEADBEEF)
#endif

#define SAFE_RELEASE(p) if (p) { (p)->Release(); (p) = nullptr; }

#define RELEASE_AND_INVALIDATE(type, p) if (p) { (p)->Release(); (p) = reinterpret_cast<type *>(0xDEADBEEF); }

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

#define LTK_ASSERT(expr) if (!(expr)) {\
    CStringW msg; msg.Format(L"Assertion Failed: %s\r\n%s(%d)", L#expr, __FILEW__, __LINE__);\
    ::OutputDebugStringW(msg);__debugbreak();}

#define  LTK_LOG(...) LtkLogImpl(__FILE__, __LINE__, __VA_ARGS__)

void LtkLogInit();
void LtkLogImpl(const char *source, int line, const char *format, ...);
