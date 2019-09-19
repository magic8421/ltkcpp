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
	void InvokeCallback(UINT event_id, LTK_ARG arg1, LTK_ARG arg2, LTK_ARG arg3, LTK_ARG arg4);

private:
	std::map<UINT, std::vector<CallbackInfo>> m_mapCallbacks;
};

} // namespace ltk

