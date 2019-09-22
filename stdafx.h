//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <new> // for `placement new`
#include <cassert>
#include <vector>
#include <forward_list>
#include <string>
#include <sstream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <memory>
#include <algorithm>
#include <functional>
#include <thread>

using std::wstring;
using std::wstringstream;
using std::stringstream;
using std::unique_ptr;
using std::vector;

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <commdlg.h>

#include <tchar.h>
#include <strsafe.h>
#include <atlstr.h>
#include <atlconv.h>

#pragma warning(push)
#pragma warning(disable:4458)
#include <GdiPlus.h>
#pragma warning(pop)


#include <ShlObj.h>

#include <d2d1.h>
#include <Wincodec.h>
#include <Dwrite.h>

#include "3rd\tinyxml2.h"

//#include <vld.h>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#ifdef _DEBUG
#define DEBUG_NEW   new( _NORMAL_BLOCK, __FILE__, __LINE__)
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
//allocations to be of _CLIENT_BLOCK type
#else
#define DEBUG_NEW
#endif // _DEBUG

// https://stackoverflow.com/questions/3060006/is-it-worth-setting-pointers-to-null-in-a-destructor
#ifndef INVALID_POINTER
#define INVALID_POINTER(type) reinterpret_cast<type *>(0xDEADBEEF)
#endif

#define RELEASE_AND_INVALIDATE(type, p) if (p) { (p)->Release(); (p) = reinterpret_cast<type *>(0xDEADBEEF); }

//#define LTK_ASSERT(expr) if (!(expr)) {\
//    CStringW msg; msg.Format(L"Assertion Failed: %s\r\n%s(%d)", L#expr, __FILEW__, __LINE__);\
//    ::OutputDebugStringW(msg);__debugbreak();}

#define LOGW(msg) do\
{\
	std::wstringstream ss;\
	ss << __FUNCTIONW__ << L"() " msg << std::endl;\
	OutputDebugStringW(ss.str().c_str());\
} while (0)

#ifdef _DEBUG
#define LTK_CHECK_THREAD Object::CheckThread()
#else
#define LTK_CHECK_THREAD
#endif