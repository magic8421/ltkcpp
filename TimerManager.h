//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "Object.h"
#include "Delegate/Delegate.h"

namespace ltk {

class Timer;
class TimerPrivate;

class Timer : public Object
{
public:
	Timer();

	void SetInterval(UINT ms);
	void Start();
	void StartOnce();
	void Stop();
	void Reset();
	UINT GetId();

	void AttatchTimeoutDelegate(const Delegate0<> &cb);
	void RemoveTimeoutDelegate(const Delegate0<> &cb);

private:
	LTK_DECLARE_PRIVATE(Timer);
	friend class TimerManager;

protected:
	Timer(TimerPrivate *d);
};

} // namespace ltk
