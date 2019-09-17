#include "stdafx.h"
#include "Object.h"
#include "Common.h"

namespace ltk {

void Object::RegisterCallback(UINT event_id, LtkEventCallback cb, void* userdata)
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

void Object::InvokeCallback(UINT event_id, void* arg)
{
	auto iter = m_mapCallbacks.find(event_id);
	if (iter == m_mapCallbacks.end()) {
		return;
	}
	auto& vecCallbacks = m_mapCallbacks[event_id];
	for (auto info : vecCallbacks) {
		info.callback(info.userdata, event_id, this, arg);
	}
}

} // namespace ltk
