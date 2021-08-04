//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "Object.h"
#include "RefCounted.h"

namespace ltk {

class Timer : public Object
{
	std::function<void(UINT)> callback;
	Weak<Object> ref;

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

	UINT Start(UINT id, UINT elapse, bool bOnce, Weak<Object> ref, std::function<void(UINT)> callback);
	void Stop(UINT id);

private:
    TimerManager();
    ~TimerManager();
    static TimerManager *m_instance;

    static LRESULT CALLBACK MyWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
    void OnTimer(UINT id);

    HWND m_hwnd = 0;
	std::unordered_map<UINT, Ptr<Timer>> m_mapCallback;
};

} // namespace ltk
