#include "stdafx.h"
#include "Object.h"
#include "Common.h"
#include "StyleManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

static __declspec(thread) Object *sDelegateInvoker = nullptr;


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

LPCSTR Object::GetName()
{
	return m_name;
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

} // namespace ltk
