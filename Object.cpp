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
}

Object::Object(ObjectPrivate *pp)
{
	d_ptr = pp;
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

} // namespace ltk
