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

/////////////////////////////////////////////////////////////////

void Object::RegisterEvent(UINT code, LtkCallback callback, void* userdata)
{
	auto iter = m_mapCallback.find(code);
	if (iter != m_mapCallback.end()) { // exist in map
		AddEventToList(iter->second, callback, userdata);
	}
	else {
		auto [iter2, bSucceed] = m_mapCallback.emplace(
			std::make_pair(code, std::vector <CallbackInfo>()));
		LTK_ASSERT(bSucceed);
		AddEventToList(iter2->second, callback, userdata);
	}
}

void Object::AddEventToList(std::vector<CallbackInfo>& list, LtkCallback callback, void* userdata)
{
	bool bFound = false;
	// find duplicate
	for (size_t i = list.size(); i > 0; i --) { // maybe better in reversed order.
		const auto& item = list[i - 1];
		if (item.callback == callback && item.userdata == userdata) {
			bFound = true;
			break;
		}
	}
	if (bFound) {
		return;
	}
	list.push_back(CallbackInfo{userdata, callback});
}


} // namespace ltk
