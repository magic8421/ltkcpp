#include "stdafx.h"
#include "Object.h"
#include "Common.h"

namespace ltk {

static __declspec(thread) Object *sDelegateInvoker = nullptr;

Object::Object()
{
	m_obctrl = new ObserverCtrl(this);
}

Object::~Object() 
{
	m_obctrl->Set(nullptr);
	m_obctrl->Release();
}


void Object::SetInvalid()
{
	m_obctrl->Set(nullptr);
}

Object * Object::GetDelegateInvoker()
{
	return sDelegateInvoker;
}

void Object::SetDelegateInvoker(Object *sender)
{
	sDelegateInvoker = sender;
}

#ifndef LTK_NO_CINTERFACE

static Object *sEventSender;

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
	sEventSender = this; // TODO thread?

	auto iter = m_mapCallbacks.find(event_id);
	if (iter == m_mapCallbacks.end()) {
		return;
	}
	auto& vecCallbacks = iter->second; // TODO copy?
	for (UINT i = vecCallbacks.size(); i > 0; i--) {
		const CallbackInfo &info = vecCallbacks[i - 1];
		va_list args;
		va_start(args, event_id);
		DoInvokeCallback(event_id, info.callback, info.userdata, args);
		// 感觉提前终止也不好 看看gtk有没有这个功能
		//if (DoInvokeCallback(event_id, info.callback, info.userdata, args)) {
		//	va_end(args);
		//	break;
		//}
		va_end(args);
	}
}

Object *Object::GetEventSender()
{
	return sEventSender;
}

#endif

} // namespace ltk
