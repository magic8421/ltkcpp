//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TimerManager.h"
#include "Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

TimerManager *TimerManager::m_instance = nullptr;

TimerManager *TimerManager::Instance()
{
    if (!m_instance) {
        m_instance = new TimerManager;
    }
    return m_instance;
}

void TimerManager::Free()
{
    delete m_instance;
}

TimerManager::TimerManager()
{
    WNDCLASS wc;
    ::ZeroMemory(&wc, sizeof(wc));
    wc.hInstance = ::GetModuleHandle(NULL);
    wc.lpszClassName = L"LtkTimerManager";
    wc.lpfnWndProc = MyWndProc;
    ::RegisterClass(&wc);

    m_hwnd = ::CreateWindow(L"LtkTimerManager", L"", WS_POPUP, 0, 0, 10, 10, 
        HWND_MESSAGE, NULL, HINST_THISCOMPONENT, NULL);
    LTK_ASSERT(::IsWindow(m_hwnd));
    ::SetWindowLongPtr(m_hwnd, GWLP_USERDATA,reinterpret_cast<LPARAM>(this));
}

TimerManager::~TimerManager()
{
    if (m_hwnd) {
        ::DestroyWindow(m_hwnd);
    }
    for (auto iter: m_mapCallback) {
        delete iter.second;
    }
}

LRESULT CALLBACK TimerManager::MyWndProc(
    HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    TimerManager *thiz = reinterpret_cast<TimerManager *>
        (GetWindowLongPtr(hwnd, GWLP_USERDATA));
    switch (message) {
    case WM_TIMER:
        thiz->OnTimer((UINT)wparam);
        break;
    case WM_NCDESTROY:
        thiz->m_hwnd = 0;
        break;
    }
    return 0;
}

void TimerManager::OnTimer(UINT id)
{
    auto iter = m_mapCallback.find(id);
    if (iter != m_mapCallback.end()) {
        TimerNode *node = iter->second;
        node->callback();
        if (node->isOnceTimer) {
            ::KillTimer(m_hwnd, iter->first);
            delete node;
            m_mapCallback.erase(iter);
        }
    } else {
        LTK_ASSERT(false);
    }
}

UINT TimerManager::SetTimer(const std::function<void()>&cb, UINT elapse, bool bOnce)
{
    UINT id = 0;
    auto iter = m_mapCallback.end();
    do {
        id = (UINT)rand() + USER_TIMER_MINIMUM;
        iter = m_mapCallback.find(id);
    } while (iter != m_mapCallback.end());
    auto node = new TimerNode;
    node->callback = cb;
    node->isOnceTimer = bOnce;
    m_mapCallback[id] = node;
    ::SetTimer(m_hwnd, id, elapse, NULL);
    return id;
}

UINT SetTimer(const std::function<void()>&cb, UINT elapse)
{
    return TimerManager::Instance()->SetTimer(cb, elapse, false);
}

UINT SetOnceTimer(const std::function<void()>&cb, UINT elapse)
{
    return TimerManager::Instance()->SetTimer(cb, elapse, true);
}

} // namespace ltk
