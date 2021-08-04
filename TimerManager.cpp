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
    WNDCLASS wc = { 0 };
    wc.hInstance = HINST_THISCOMPONENT;
    wc.lpszClassName = L"LtkTimerManager";
    wc.lpfnWndProc = MyWndProc;
    ::RegisterClass(&wc);

    m_hwnd = ::CreateWindowW(L"LtkTimerManager", L"", WS_POPUP, 0, 0, 10, 10, 
        HWND_MESSAGE, NULL, HINST_THISCOMPONENT, NULL);
    LTK_ASSERT(::IsWindow(m_hwnd));
    ::SetWindowLongPtr(m_hwnd, GWLP_USERDATA,reinterpret_cast<LPARAM>(this));
}

TimerManager::~TimerManager()
{
    if (m_hwnd) {
        ::DestroyWindow(m_hwnd);
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
        if (thiz) {
            thiz->m_hwnd = 0;
        } else {
            LTK_LOG("this is null");
        }
        break;
    }
    return ::DefWindowProc(hwnd, message, wparam, lparam);
}

void TimerManager::OnTimer(UINT id)
{
	auto iter = m_mapCallback.find(id);
	if (iter != m_mapCallback.end()) {
		auto timer = iter->second;
		LTK_ASSERT(timer->id == id);
		if (!timer->ref.Expired()) {
			auto strong_ref = timer->ref.Lock(); // 机智如我 强引用防止回调内this意外无效
			timer->callback(id);
		}
		if (timer->bOnce) {
			timer->id = 0;
			::KillTimer(m_hwnd, id);
			m_mapCallback.erase(iter);
		}
	}
	else {
		LTK_ASSERT(false);
	}
}

UINT TimerManager::Start(UINT id, UINT elapse, bool bOnce, Weak<Object> ref, std::function<void(UINT)> callback)
{
	Ptr timer (new Timer);
	timer->id = id;
	timer->elapse = elapse;
	timer->bOnce = bOnce;
	timer->ref = ref;
	timer->callback = callback;

	if (id == 0) {
		auto iter = m_mapCallback.end();
		do {
			id = (UINT)rand() + USER_TIMER_MINIMUM;
			iter = m_mapCallback.find(id);
		} while (iter != m_mapCallback.end());

		m_mapCallback[id] = timer;
		::SetTimer(m_hwnd, id, elapse, NULL);
		timer->id = id;
	}
	else {
		auto iter = m_mapCallback.find(id);
		if (iter != m_mapCallback.end()) {
			LTK_ASSERT(iter->second == timer);
			::SetTimer(m_hwnd, id, elapse, NULL); // refresh the timer.
		}
		else {
			LTK_ASSERT(false);
		}
	}
	return id;
}

void TimerManager::Stop(UINT id)
{
	if (id == 0) {
		return; // TODO [0] cannot be searched by stl hash table?
	}
	auto iter = m_mapCallback.find(id);
	if (iter != m_mapCallback.end()) {
		//timer->id = 0;
		::KillTimer(m_hwnd, id);
		m_mapCallback.erase(iter);
	}
}

} // namespace ltk
