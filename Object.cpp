#include "stdafx.h"
#include "Object.h"
#include "Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

static __declspec(thread) Object *sDelegateInvoker = nullptr;
static std::unordered_set<Object*> *sObjectSet;

Object::Object()
{
	m_obctrl = new ObserverCtrl(this);

	// TODO lock for multithread.
	if (!sObjectSet) {
		sObjectSet = new std::unordered_set<Object *>;
	}
	sObjectSet->insert(this);
}

Object::~Object() 
{
	m_obctrl->Set(nullptr);
	m_obctrl->Release();

	// TODO lock for multithread.
	auto iter = sObjectSet->find(this);
	if (iter == sObjectSet->end()) {
		LTK_ASSERT(false);
	}
	sObjectSet->erase(iter);
}

void Object::DumpObjectLeaks()
{
	// TODO lock for multithread.
	char buf[512];
	for (Object *obj : *sObjectSet) {
		::StringCbPrintfA(
			buf, sizeof(buf), "LtkObject leak: [%s] %s(%d)\r\n",
			obj->TypeNameInstance(), obj->m_source, obj->m_line);
		::OutputDebugStringA(buf);
	}
	delete sObjectSet;
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

static __declspec(thread) Object *sEventSender;

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
	sEventSender = this;

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
