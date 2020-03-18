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

Object * Object::GetDelegateInvoker()
{
	return sDelegateInvoker;
}

void Object::SetDelegateInvoker(Object *sender)
{
	sDelegateInvoker = sender;
}

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

void Object::SetSourceLine(LPCSTR source, int line)
{
	m_source = source;
	m_line = line;
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

void Object::SetName(LPCSTR name)
{
	m_name = StyleManager::Instance()->InternString(name);
}

LPCSTR Object::GetName()
{
	return m_name;
}

bool Object::CheckValid(Object* o)
{
	// TODO lock for multithread.
	return sObjectSet->find(o) != sObjectSet->end();
}

void Object::AddNotifyCallback(void* userdata, LtkCallback callback)
{
	CallbackInfo info{ userdata, callback };
	m_vecCallback.push_back(info);
}

void Object::RemoveNotifyCallback(void* userdata, LtkCallback callback)
{
	LTK_ASSERT(false);
}

BOOL Object::DispatchNotify(LtkEvent* ev)
{
	for (const auto& info : m_vecCallback) {
		info.callback(info.userdata, ev);
	}
	return FALSE;
}

void Object::SetEventCallback(void* userdata, LtkCallback callback)
{
	m_virtualContext = userdata;
	m_virtualHandler = callback;
}

void* Object::GetEventContext()
{
	return m_virtualContext;
}

LtkCallback Object::GetEventCallback()
{
	return m_virtualHandler;
}

BOOL Object::CallEventCallback(LtkEvent* ev)
{
	if (!m_virtualContext)
		return FALSE;
	return m_virtualHandler(m_virtualContext, ev);
}



} // namespace ltk
