#include "stdafx.h"
#include "Object.h"
#include "Object_p.h"
#include "Common.h"

namespace ltk {

Object::Object()
{
	//LTK_ASSERT(false);
	m_obctrl = new ObserverCtrl(this);
}

Object::Object(ObjectPrivate *pp)
{
	d_ptr = pp;
	m_obctrl = new ObserverCtrl(this);
}

Object::~Object() 
{
	m_obctrl->Set(nullptr);
	m_obctrl->Release();
	delete d_ptr;
}


void Object::SetInvalid()
{
	m_obctrl->Set(nullptr);
}

} // namespace ltk
