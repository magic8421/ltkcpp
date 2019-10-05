//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "Object.h"
#include "MulticastDelegate.h"

namespace ltk {

class LTK_API Timer : public Object
{
public:
	Timer() {}
	virtual ~Timer();

	void SetInterval(UINT ms);
	void Start();
	void StartOnce();
	void Stop();
	UINT GetId();

	MulticastDelegate0 TimeoutDelegate;

private:
	void Triger();

	UINT id = 0;
	UINT elapse = 0;
	bool bOnce = false;

	friend class TimerManager;
};

class TimerManager
{
public:
    static TimerManager *Instance();
    static void Free();

	UINT SetTimer(Timer *timer);
	void KillTimer(Timer *timer);

private:
    TimerManager();
    ~TimerManager();
    static TimerManager *m_instance;

    static LRESULT CALLBACK MyWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
    void OnTimer(UINT id);

    HWND m_hwnd = 0;
	std::unordered_map<UINT, Timer *> m_mapCallback;
};

} // namespace ltk
