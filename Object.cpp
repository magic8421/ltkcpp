#include "stdafx.h"
#include "Object.h"
#include "Object_p.h"
#include "Common.h"

namespace ltk {

static __declspec(thread) RTTI * sDelegateInvoker = nullptr;

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
}

Object::Object(ObjectPrivate *d)
{
	d_ptr = d;
}

Object::~Object() 
{
	delete d_ptr;
}

void Object::CheckThread()
{
	auto current = ::GetCurrentThreadId();
	if (d_ptr->thread_id != current) {
		LTK_LOG("Ltk API was called in a non-gui thread: %d, %d",
			d_ptr->thread_id, current);
		LTK_ASSERT(false);
	}
}

void Object::MoveToThread(DWORD id)
{
	d_ptr->thread_id = id;
}

void Object::Delete()
{
	delete this;
}

void Object::SetDelegeteInvoker(RTTI *sender)
{
	sDelegateInvoker = sender;
}

RTTI * Object::GetDelegateInvoker()
{
	return sDelegateInvoker;
}

} // namespace ltk
