#pragma once
#include "LtkInterface.h"

namespace ltk {


struct CallbackInfo
{
	LtkEventCallback callback = nullptr;
	void* userdata = nullptr;
};

class Object
{
public:
	Object() {}
	virtual ~Object() {}

	void RegisterCallback(UINT event_id, LtkEventCallback cb, void* userdata);
	void InvokeCallback(UINT event_id, void* arg);

private:
	std::map<UINT, std::vector<CallbackInfo>> m_mapCallbacks;
};

} // namespace ltk

