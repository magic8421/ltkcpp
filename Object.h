#pragma once
#include "LtkInterface.h"

namespace ltk {


struct CallbackInfo
{
	LtkCallback callback = nullptr;
	void* userdata = nullptr;
};

class Object
{
public:
	Object() {}
	virtual ~Object() {}

	void RegisterCallback(UINT event_id, LtkCallback cb, void* userdata);
	void InvokeCallback(UINT event_id, ...);

	virtual BOOL DoInvokeCallback(UINT event_id, LtkCallback cb,
		void* userdata, va_list args) { return 1; }

private:
	std::map<UINT, std::vector<CallbackInfo>> m_mapCallbacks;
};

} // namespace ltk

