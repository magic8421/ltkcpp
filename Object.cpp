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

} // namespace ltk
