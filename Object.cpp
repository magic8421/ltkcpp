#include "stdafx.h"
#include "Object.h"
#include "Common.h"
#include "StyleManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

static __declspec(thread) Object *sDelegateInvoker = nullptr;
static std::unordered_set<Object*>* sObjectSet;

Object::Object()
{
	// TODO lock for multithread.
	if (!sObjectSet) {
		sObjectSet = new std::unordered_set<Object*>;
	}
	sObjectSet->insert(this);
}

Object::~Object()
{
	m_bDeleting = true;
	for (size_t i = 0; i < m_children.size(); i++) {
		delete m_children.IndexNoCheck(i);
	}
	// if we delete an object which is not the root object
	if (m_parent && !m_parent->m_bDeleting) {
		m_parent->RemoveChild(this);
	}
	// TODO lock for multithread.
	auto iter = sObjectSet->find(this);
	if (iter == sObjectSet->end()) {
		LTK_ASSERT(false);
	}
	sObjectSet->erase(iter);
}

void Object::Free()
{
	delete sObjectSet;
}

Object * Object::GetDelegateInvoker()
{
	return sDelegateInvoker;
}

void Object::SetDelegateInvoker(Object *sender)
{
	sDelegateInvoker = sender;
}

void Object::SetName(LPCSTR name)
{
	m_name = ltk::InternString(name);
}

void Object::AddChild(Object* o)
{
	if (o->m_parent == this)
		return;
	if (o->m_parent) {
		o->m_parent->RemoveChild(o);
	}
	m_children.push_back(o);
	o->m_parent = this;
}

void Object::RemoveChild(Object* o)
{
	// maybe searh from the end is better, because we always push to the end.
	for (int i = m_children.size() - 1; i >= 0; i--) {
		auto o2 = m_children[i];
		if (o2 == o) {
			o2->m_parent = nullptr;
			for (int j = i + 1; j < (int)m_children.size(); j++) {
				m_children[j - 1] = m_children[j];
			}
			m_children.pop_back();
			return;
		}
	}
}

void Object::SetParent(Object* p)
{
	p->AddChild(this);
}

/////////////////////////////////////////////////////////////////

void Object::SetSourceLine(LPCSTR source, int line)
{
	m_source = source;
	m_line = line;
}

bool Object::CheckValid(Object* o)
{
	// TODO lock for multithread.
	return sObjectSet->find(o) != sObjectSet->end();
}

void Object::DumpObjectLeaks()
{
	// TODO lock for multithread.
	char buf[512];
	for (Object* obj : *sObjectSet) {
		::StringCbPrintfA(
			buf, sizeof(buf), "LtkObject leak: [%s] %s(%d)\r\n",
			obj->TypeNameInstance(), obj->m_source, obj->m_line);
		::OutputDebugStringA(buf);
	}
}

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
	//m_staticSender = this; // TODO

	auto iter = m_mapCallbacks.find(event_id);
	if (iter == m_mapCallbacks.end()) {
		return;
	}
	auto& vecCallbacks = iter->second; // TODO copy?
	BOOL bHandled = FALSE;

	for (UINT i = vecCallbacks.size(); i > 0; i--) {
		const CallbackInfo &info = vecCallbacks[i - 1];
		va_list args;
		va_start(args, event_id);
		DoInvokeCallback(event_id, info.callback, info.userdata, args, &bHandled);
		va_end(args);
	}
}

std::vector<CallbackInfo>* Object::GetCallbackList(UINT event_id)
{
	auto iter = m_mapCallbacks.find(event_id);
	if (iter == m_mapCallbacks.end()) {
		return nullptr;
	}
	else {
		return &iter->second;
	}
}


} // namespace ltk
