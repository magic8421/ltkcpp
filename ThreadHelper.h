#pragma once

#include "stdafx.h"
#include <functional>
#include "Common.h"

namespace cs {
    
    extern HWND g_hwndUITask;
    extern const UINT WM_POST_THREAD_TASK;
    extern const UINT WM_POST_UI_TASK;

    void InitThreadHelper();

    class IThreadMessage
    {
    public:
        virtual ~IThreadMessage() {}
        virtual void Execute() = 0;
    };

    template<class T>
    class MyThreadMessage : public IThreadMessage
    {
    public:
        MyThreadMessage(const T &func) :
            mFunc(func)
        {
        }
        void Execute()
        {
            LTK_LOG("thread: %d", ::GetCurrentThreadId());

            mFunc();
        }
    private:
        T mFunc;
    };

    template<class T>
    void PostUITask(const T &func)
    {
        LTK_LOG("thread: %d", ::GetCurrentThreadId());

        auto p = new MyThreadMessage<T>(func);
        ::PostMessage(g_hwndUITask, WM_POST_UI_TASK, reinterpret_cast<WPARAM>(p), NULL);
    }

    DWORD WINAPI MyThreadPoolFunc(void *);

    template<class T>
    void PostThreadPoolTask(const T &func)
    {
        auto p = new MyThreadMessage<T>(func);
        ::QueueUserWorkItem(MyThreadPoolFunc, p, 0);
    }

    void MyCreateFile(std::wstring name, std::function<void(std::wstring new_name, HANDLE file)> callback);

    // TODO 这个Codeproject上面有个可以抄的
    // https://www.codeproject.com/Articles/1095196/Win-Thread-Wrapper-with-Synchronized-Start
    // 写得比较仓促 还要多检查一下 然后就是少一个同步启动的功能 我也不知道为什么一定需要这个 那篇文章里有 
    class WorkerThread
    {
    public:
        WorkerThread();
        ~WorkerThread() {}

        void Start();

        template<class T>
        void PostTask(const T &func)
        {
            LTK_LOG("thread: %d", ::GetCurrentThreadId());

            auto p = new MyThreadMessage<T>(func);
            ::PostThreadMessage(mThreadId, WM_POST_THREAD_TASK, reinterpret_cast<WPARAM>(p), NULL);
        }

        void CheckThread();

        bool IsRunningOnThread();

        DWORD GetThreadId() { return mThreadId; }

    private:
        static unsigned int __stdcall ThreadFunc(void *);
        DWORD mThreadId;
    };
}