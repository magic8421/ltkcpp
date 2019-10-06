//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "ScopeGuard.h"

#ifdef LTK_EXPORTS
#define LTK_API __declspec(dllexport)
#else
#define LTK_API __declspec(dllimport)
#endif

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&) = delete; \
	void operator=(const TypeName&) = delete;

#ifdef _DEBUG
#define LTK_ASSERT(expr) if (!(expr)) {__debugbreak();} // super KISS
#else
#define LTK_ASSERT(expr) (void)(expr)
#endif


class ImmutableString
{
public:
	ImmutableString() {}
	explicit ImmutableString(LPCSTR str)
	{
		m_ptr = _strdup(str);
	}
	ImmutableString(ImmutableString &&rhs)
	{
		m_ptr = rhs.m_ptr;
		rhs.m_ptr = nullptr;
	}
	~ImmutableString()
	{
		free((void *)m_ptr);
	}
	bool operator==(LPCSTR rhs)
	{
		if (m_ptr && rhs) {
			return strcmp(m_ptr, rhs) == 0;
		} else {
			return m_ptr == rhs;
		}
	}
	void operator=(LPCSTR str)
	{
		free((void *)m_ptr);
		if (str) {
			m_ptr = _strdup(str);
		} else {
			m_ptr = nullptr;
		}
	}
	operator LPCSTR()
	{
		return m_ptr;
	}

private:
	const char *m_ptr = nullptr;
	DISALLOW_COPY_AND_ASSIGN(ImmutableString);
};

class ImmutableWString
{
public:
	ImmutableWString()
	{
	}
	explicit ImmutableWString(LPCWSTR str)
	{
		m_ptr = _wcsdup(str);
	}
	ImmutableWString(ImmutableWString &&rhs)
	{
		m_ptr = rhs.m_ptr;
		rhs.m_ptr = nullptr;
	}
	~ImmutableWString()
	{
		free((void *)m_ptr);
	}
	bool operator==(LPCWSTR rhs)
	{
		if (m_ptr && rhs) {
			return wcscmp(m_ptr, rhs) == 0;
		}
		else {
			return m_ptr == rhs;
		}
	}
	void operator=(LPCWSTR str)
	{
		free((void *)m_ptr);
		if (str) {
			m_ptr = _wcsdup(str);
		}
		else {
			m_ptr = nullptr;
		}
	}
	operator LPCWSTR()
	{
		return m_ptr;
	}
	wchar_t *Allocate(UINT size)
	{
		free((void *)m_ptr);
		m_ptr = (wchar_t *)malloc((size + 1) * sizeof(wchar_t));
		return const_cast<wchar_t*>(m_ptr);
	}
private:
	const wchar_t *m_ptr = nullptr;
	DISALLOW_COPY_AND_ASSIGN(ImmutableWString);
};

LTK_API std::wstring Utf8ToUtf16(LPCSTR strA, int len = -1);
CStringA Utf16ToUtf8(LPCTSTR strW, int len);
CStringA Utf16ToGbk(LPCTSTR strW, int len);

LTK_API ImmutableWString WStringFormat(LPCWSTR format, ...);


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


#define  LTK_LOG(...) LtkLogImpl(__FILE__, __LINE__, __VA_ARGS__)

LTK_API void LtkLogInit();
LTK_API void LtkLogImpl(const char *source, int line, const char *format, ...);
