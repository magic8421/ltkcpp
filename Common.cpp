//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common.h"
#include "UniConversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

std::wstring Utf8ToUtf16(LPCSTR strA, int len)
{
	if(len < 0) {
		len = (int)strlen(strA);
	}
    std::wstring strW;
    int lenW = UTF16Length(strA, len);
    strW.resize(lenW);
    UTF16FromUTF8(strA, len, &strW[0], lenW);
    return std::move(strW);
}

std::string Utf16ToUtf8(LPCTSTR strW, int len)
{
    if (len < 0) {
        len = (int)wcslen(strW);
    }
    std::string strA;
	int lenA = UTF8Length(strW, len);
    strA.resize(lenA);

	UTF8FromUTF16(strW, len, &strA[0], lenA);

	return std::move(strA);
}

std::string Utf16ToGbk(LPCTSTR strW, int len)
{
    if (len < 0) {
        len = (int)wcslen(strW);
    }
    int lenA = ::WideCharToMultiByte(936, 0, strW, len, NULL, 0, NULL, NULL);
    std::string strA;
    strA.resize(lenA);
    ::WideCharToMultiByte(936, 0, strW, len, &strA[0], lenA, NULL, NULL);
    return std::move(strA);
}

std::wstring WStringFormat(LPCWSTR format, ...)
{
	std::wstring str;
    va_list arg;
    va_start(arg, format);
    auto len = _vscwprintf(format, arg);
    str.resize(len);
    //auto buf = str.Allocate(len);
	vswprintf(&str[0], len + 1,format, arg);
    va_end(arg);
    return std::move(str);
}

///////////////////////////////////////////////////////////////////////////////
// LTK LOG
///////////////////////////////////////////////////////////////////////////////

static bool g_ltk_log_to_ods = true;
static bool g_ltk_log_to_file = false;
static HANDLE g_ltk_log_file_handle = 0;

static const wchar_t *LOG_DIR = L"C:\\ZLOG";
static const int MAX_LOG_NUM = 15;

static void CreateLogFile()
{
    if (g_ltk_log_file_handle) {
        return;
    }
    if (!::PathFileExists(LOG_DIR)) {
        ::CreateDirectory(LOG_DIR, NULL);
    }
    wchar_t buffer[64];
    tm tm_;
    time_t time_ = time(NULL);
    localtime_s(&tm_, &time_);
    wcsftime(buffer, ARRAYSIZE(buffer), L"%Y%m%d-%H%M%S", &tm_);
    std::wstring path = LOG_DIR;
    path += L"\\";
    path += buffer;
    path += L".log";
    g_ltk_log_file_handle = ::CreateFile(path.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == g_ltk_log_file_handle) {
        LTK_ASSERT(false);
    }
}

static bool Compare(const WIN32_FIND_DATA &lhs, const WIN32_FIND_DATA &rhs)
{
    return ::CompareFileTime(&lhs.ftLastWriteTime, &rhs.ftLastWriteTime) < 0;
}

static void DeleteOldLog()
{
    std::vector<WIN32_FIND_DATA> vecFiles;
    WIN32_FIND_DATA wfd;
    std::wstring path = LOG_DIR;
    path += L"\\*.log";
    HANDLE hFind = ::FindFirstFile(path.c_str(), &wfd);
    if (hFind != INVALID_HANDLE_VALUE) {
        vecFiles.push_back(wfd);
        while (::FindNextFile(hFind, &wfd))
        {
            vecFiles.push_back(wfd);
        }
        ::FindClose(hFind);
    }
    if (vecFiles.size() > MAX_LOG_NUM) {
        std::sort(vecFiles.begin(), vecFiles.end(), Compare);
        for (size_t i = 0; i < vecFiles.size() && i <= vecFiles.size() - MAX_LOG_NUM; ++i) {
            std::wstring file = LOG_DIR;
            file += L"\\";
            file += vecFiles.at(i).cFileName;
            ::DeleteFile(file.c_str());
        }
    }
}

void LtkLogInit()
{
    if (::PathFileExists(LOG_DIR)) {
        DeleteOldLog();
        CreateLogFile();
    }
}

void LtkLogImpl(const char *source, int line, const char *format, ...)
{
    const size_t SIZE = 1024 * 2;
    char buffer[SIZE];
    va_list varg;
    va_start(varg, format);
    ::StringCbVPrintfA(buffer, SIZE, format, varg);
    va_end(varg);
    char buffer2[SIZE];
    ::StringCbPrintfA(buffer2, SIZE, "%s\t%s(%d)\r\n", buffer, source, line);

    if (g_ltk_log_to_ods) {
        ::OutputDebugStringA(buffer2);
    }
    if (g_ltk_log_to_file) {
        size_t len = strlen(buffer2);
        DWORD out_len = 0;
        ::WriteFile(g_ltk_log_file_handle, buffer2, (DWORD)len, &out_len, NULL);
    }
}

