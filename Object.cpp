#include "stdafx.h"
#include "Object.h"
#include "Common.h"
#include "StyleManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

static __declspec(thread) Object *sDelegateInvoker = nullptr;
//static std::unordered_set<Object*>* sObjectSet; // TODO 改成双链表足以

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
}

Object::~Object()
{
}

void Object::Free()
{
}

void Object::SetSourceLine(LPCSTR source, int line)
{
}

void Object::DumpObjectLeaks()
{
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
	return true;
}

ULONG Object::AddRef()
{
	InterlockedIncrement(&m_cRef);
	return m_cRef;
}

ULONG Object::Release()
{
	ULONG ulRefCount = InterlockedDecrement(&m_cRef);
	if (0 == m_cRef) {
		delete this;
	}
	return ulRefCount;
}


} // namespace ltk
