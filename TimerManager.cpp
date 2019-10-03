//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common.h"
#include "TimerManager.h"
#include "TimerManager_p.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

Timer::Timer() : Object(new TimerPrivate(this))
{
}

Timer::Timer(TimerPrivate *d) : Object(d)
{
}

void Timer::SetInterval(UINT ms)
{
	LTK_PUBLIC_DQ(Timer);
	d->elapse = ms;
}

void Timer::Start()
{
	LTK_PUBLIC_DQ(Timer);
	d->bOnce = false;
	TimerManager::Instance()->SetTimer(this);
}

void Timer::StartOnce()
{
	LTK_PUBLIC_DQ(Timer);
	d->bOnce = true;
	TimerManager::Instance()->SetTimer(this);
}

void Timer::Stop()
{
	TimerManager::Instance()->KillTimer(this);
}

void Timer::Reset()
{
	TimerManager::Instance()->SetTimer(this);
}

UINT Timer::GetId()
{
	LTK_PUBLIC_DQ(Timer);
	return d->id;
}

void Timer::AttatchTimeoutDelegate(const Delegate0<> &cb)
{
	LTK_PUBLIC_DQ(Timer);
	d->delegateTrigerred += cb;
}

void Timer::RemoveTimeoutDelegate(const Delegate0<> &cb)
{
	LTK_PUBLIC_DQ(Timer);
	d->delegateTrigerred -= cb;
}

TimerPrivate::TimerPrivate(Timer *q) : ObjectPrivate(q)
{
}

TimerPrivate::~TimerPrivate()
{
	TimerManager::Instance()->KillTimer(q_func());
}


void TimerPrivate::Triger()
{
	Object::SetDelegeteInvoker(q_func());
	this->delegateTrigerred();
}

//////////////////////////////////////////////////////////////////////////

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
        TimerNode *node = iter->second;
        //node->callback();
		TimerPrivate *d = node->timer->d_func();
		LTK_ASSERT(d->id == id);
		d->Triger();
        if (node->isOnceTimer) {
			d->id = 0;
            ::KillTimer(m_hwnd, id);
            delete node;
            m_mapCallback.erase(iter);
        }
    } else {
        LTK_ASSERT(false);
    }
}

//UINT TimerManager::SetTimer(UINT id, const std::function<void()>&cb, UINT elapse, bool bOnce)
UINT TimerManager::SetTimer(Timer *timer)
{
	UINT id = timer->d_func()->id;
	bool bOnce = timer->d_func()->bOnce;
	UINT elapse = timer->d_func()->elapse;

    if (id == 0) {
        auto iter = m_mapCallback.end();
        do {
            id = (UINT)rand() + USER_TIMER_MINIMUM;
            iter = m_mapCallback.find(id);
        } while (iter != m_mapCallback.end());
        auto node = new TimerNode;
		node->timer = timer;
        //node->callback = cb;
        node->isOnceTimer = bOnce;
        m_mapCallback[id] = node;
        ::SetTimer(m_hwnd, id, elapse, NULL);
		timer->d_func()->id = id;
    } else {
        auto iter = m_mapCallback.find(id);
        if (iter != m_mapCallback.end()) {
            iter->second->isOnceTimer = bOnce;
			iter->second->timer = timer;
			//iter->second->callback = cb;
            ::SetTimer(m_hwnd, id, elapse, NULL); // refresh the timer.
        } else {
            LTK_ASSERT(false);
        }
    }
    return id;
}

void TimerManager::KillTimer(Timer *timer)
{
	UINT id = timer->d_func()->id;

	if (id == 0) {
		return; // TODO [0] cannot be searched by stl hash table?
	}
    auto iter = m_mapCallback.find(id);
    if (iter != m_mapCallback.end()) {
		timer->d_func()->id = 0;
        delete iter->second;
        ::KillTimer(m_hwnd, id);
        m_mapCallback.erase(iter);
    }
}

} // namespace ltk
