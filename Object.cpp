#include "stdafx.h"
#include "Object.h"
#include "Common.h"

namespace ltk {

Object::Object()
{
	m_obctrl = new ObserverCtrl;
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

} // namespace ltk
