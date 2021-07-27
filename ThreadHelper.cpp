//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <cassert>
#include "ThreadHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk{

HWND g_hwndUITask = NULL;
const UINT WM_POST_THREAD_TASK = WM_USER + 1;
const UINT WM_POST_UI_TASK = WM_USER + 2;

LRESULT CALLBACK MyWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_POST_UI_TASK:
        do
        {
            LTK_LOG("thread: %d", ::GetCurrentThreadId());

            auto p = reinterpret_cast<IThreadMessage *>(wparam);
            p->Execute();
            delete p;
        } while (0);
        break;
    default:
        break;
    }
    return ::DefWindowProc(hwnd, message, wparam, lparam);
}

void InitThreadHelper()
{
    WNDCLASS wc;
    ::ZeroMemory(&wc, sizeof(wc));
    wc.hInstance = ::GetModuleHandle(NULL);
    wc.lpszClassName = L"MyMsgWnd";
    wc.lpfnWndProc = MyWndProc;
    ::RegisterClass(&wc);

    g_hwndUITask = ::CreateWindow(L"MyMsgWnd", L"", WS_POPUP, 0, 0, 10, 10, HWND_MESSAGE, NULL, ::GetModuleHandle(NULL), NULL);
    assert(::IsWindow(g_hwndUITask));
}

WorkerThread::WorkerThread() :
    mThreadId(0)
{
}

void WorkerThread::Start()
{
    _beginthreadex(NULL, 0, ThreadFunc, 0, 0, reinterpret_cast<unsigned int*>(&mThreadId));

    // wait for thread message queue created.
    while (!::PostThreadMessage(mThreadId, WM_NULL, 0, 0))
    {
        LTK_LOG("wait.");
    }
}

unsigned int __stdcall WorkerThread::ThreadFunc(void *)
{
    MSG msg;
    while (::GetMessage(&msg, NULL, 0, 0))
    {
        if (WM_POST_THREAD_TASK == msg.message)
        {
            LTK_LOG("thread: %d", ::GetCurrentThreadId());

            auto p = reinterpret_cast<IThreadMessage *>(msg.wParam);
            p->Execute();
            delete p;
        }
    }
    return 0;
}

void WorkerThread::CheckThread()
{
    assert(::GetCurrentThreadId() == mThreadId);
}

bool WorkerThread::IsRunningOnThread()
{
    return ::GetCurrentThreadId() == mThreadId;
}

void MyCreateFile(std::wstring name, std::function<void(std::wstring new_name, HANDLE file)> callback)
{
    PostThreadPoolTask([name, callback](){
        // 进到线程池了
        std::wstring new_name = name + L" (1)";
        HANDLE hFile = (HANDLE) 1;
        // 费时IO操作
        ::Sleep(2000);

        PostUITask([new_name, hFile, callback](){
            // 回到UI(逻辑控制)线程
            callback(new_name, hFile);
        });
    });
}

DWORD WINAPI MyThreadPoolFunc(void *param)
{
    auto p = reinterpret_cast<IThreadMessage *>(param);
    p->Execute();
    delete p;
    return 0;
}

} // namespace