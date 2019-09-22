#include "stdafx.h"
#include "Object.h"
#include "Object_p.h"
#include "Common.h"

namespace ltk {

ObjectPrivate::ObjectPrivate() : ObjectPrivate(nullptr)
{
}

ObjectPrivate::ObjectPrivate(Object *q) : q_ptr(q) 
{
#ifdef _DEBUG
	this->thread_id = ::GetCurrentThreadId();
#endif
}

Object::Object()
{
	//LTK_ASSERT(false);
	d_ptr = new ObjectPrivate;
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

void Object::CheckThread()
{
	auto current = ::GetCurrentThreadId();
	if (d_ptr->thread_id != current) {
		LTK_LOG("Ltk API called in wrong thread. %d %d",
			d_ptr->thread_id, current);
		LTK_ASSERT(false);
	}
}

void Object::MoveToThread(DWORD id)
{
	d_ptr->thread_id = id;
}

} // namespace ltk
