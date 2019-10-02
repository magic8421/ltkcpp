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
	// TODO 如果一个对象在树里 然后对它delete 要特殊处理 RemoveChild()
	auto d = d_ptr;
	for (size_t i = 0; i < d->children.size(); i++) {
		delete d->children[i];
	}
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

void Object::AddChild(Object *child)
{
	ObjectPrivate *d = d_ptr;

	Object *parent = child->d_ptr->parent;
	if (parent == nullptr) {
		child->d_ptr->parent = this;
		d->children.push_back(child);
	} else if (parent == this) {
		return;
	} else {
		parent->RemoveChild(child);
		child->d_ptr->parent = this;
		d->children.push_back(child);
	}
}

void Object::RemoveChild(Object *child)
{
	ObjectPrivate *d = d_ptr;

	for (int i = d->children.size() - 1; i >= 0; i--) {
		auto obj = d->children[i];
		if (obj == child) {
			child->d_ptr->parent = nullptr;
			for (int j = i + 1; j < (int)d->children.size(); j++) {
				d->children[j - 1] = d->children[j];
			}
			d->children.pop_back();
			break;
		}
	}
}

void Object::SetParent(Object *parent)
{
	ObjectPrivate *d = d_ptr;

	if (parent == nullptr) {
		if (d->parent) {
			d->parent->RemoveChild(this);
		}
		return;
	}
	if (d->parent == parent) {
		return;
	} else if (d->parent) {
		d->parent->RemoveChild(this);
		d->parent = parent;
		parent->d_ptr->children.push_back(this);
	} else {
		d->parent = parent;
		parent->d_ptr->children.push_back(this);
	}
}

Object *Object::GetParent()
{
	return d_ptr->parent;
}

UINT Object::GetChildCount()
{
	ObjectPrivate *d = d_ptr;
	return d->children.size();
}

Object *Object::GetChildAt(UINT idx)
{
	ObjectPrivate *d = d_ptr;
	return d->children[idx];
}

bool Object::IsSpriteType()
{
	return d_ptr->isSpriteType;
}

} // namespace ltk
