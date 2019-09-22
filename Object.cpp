#include "stdafx.h"
#include "Object.h"
#include "Common.h"

namespace ltk {

void Object::RegisterCallback(UINT event_id, LtkCallback cb, void* userdata)
{
	auto& vecCallbacks = m_mapCallbacks[event_id];
	bool bFound = false;
	for (auto info : vecCallbacks) {
		if (info.userdata == userdata) {
			bFound = true;
			break;
		}
	}
	if (bFound) {
		LTK_LOG("RegisterCallback duplicated userdata, event_id: %d", event_id);
		return;
	}
	CallbackInfo info;
	info.callback = cb;
	info.userdata = userdata;
	vecCallbacks.push_back(info);
}

void Object::InvokeCallback(UINT event_id, ...)
{
	auto iter = m_mapCallbacks.find(event_id);
	if (iter == m_mapCallbacks.end()) {
		return;
	}
	auto& vecCallbacks = iter->second; // TODO copy?
	for (UINT i = vecCallbacks.size(); i > 0; i--) {
		const CallbackInfo &info = vecCallbacks[i - 1];
		va_list args;
		va_start(args, event_id);
		//info.callback(info.userdata, event_id, arg1, arg2, arg3, arg4);
		if (DoInvokeCallback(event_id, info.callback, info.userdata, args)) {
			va_end(args);
			break;
		}
		va_end(args);
	}
}

} // namespace ltk
