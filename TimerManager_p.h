#pragma once
#include "Object_p.h"
#include "Delegate/MulticastDelegate.h"

namespace ltk {

class TimerPrivate : public ObjectPrivate
{
public:
	TimerPrivate(Timer *q);
	virtual ~TimerPrivate();

	UINT id = 0;
	UINT elapse = 0;
	bool bOnce = false;

	MulticastDelegate0 delegateTrigerred;

	void Triger();
	void SetId(UINT id);

	LTK_DECLARE_PUBLIC(Timer);
};

struct TimerNode {
	Timer *timer = nullptr;
	// Sprite *sprite; // TODO Ê²Ã´¶¼Ñ§QT
	bool isOnceTimer = false;
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
	std::unordered_map<UINT, TimerNode *> m_mapCallback;
};

} // namespace ltk
