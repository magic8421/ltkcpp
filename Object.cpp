#include "stdafx.h"
#include "Object.h"
#include "Common.h"
#include "StyleManager.h"
#include "HiddenWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

static __declspec(thread) Object *g_delegateInvoker = nullptr;
static std::unordered_set<Object*> g_objectSet;

typedef int (CALLBACK* ObjectDeleteCallback)(void *userdata);

static CriticalSection g_lockInternStr;
static CriticalSection g_lockObjectSet;

struct SZHash{
	//BKDR hash algorithm
	int operator()(LPCSTR str)const
	{
		int seed = 131;//31  131 1313 13131131313 etc//
		int hash = 0;
		while(*str)
		{
			hash = (hash * seed) + (*str);
			str ++;
		}

		return hash & (0x7FFFFFFF);
	}
};

struct SZEqual
{  
	bool operator()(LPCSTR lhs, LPCSTR rhs) const  { 
		return strcmp( lhs, rhs ) == 0;
	}  
};

std::unordered_set<LPCSTR, SZHash, SZEqual> g_internedStrings;

Object::Object()
{
}

Object::~Object()
{
	this->DeleteDelegate();


	if (m_bTracking) {
		AutoLock lock(g_lockObjectSet);
		// TODO lock for multithread.
		auto iter = g_objectSet.find(this);
		if (iter == g_objectSet.end()) {
			LTK_ASSERT(false);
		}
		else {
			g_objectSet.erase(iter);
		}
	}
}

void Object::Track(Object* obj)
{
	AutoLock lock(g_lockObjectSet);
	g_objectSet.insert(obj);
}

void Object::DeleteLater()
{
	HiddenWindow::PostDeleteLater(this);
}

void Object::Init()
{
}

void Object::Free()
{
	AutoLock lock(g_lockInternStr);
	for (auto iter = g_internedStrings.begin(); iter != g_internedStrings.end(); iter++) {
		free((void*)*iter);
	}
}

Object * Object::GetDelegateInvoker()
{
	return g_delegateInvoker;
}

void Object::SetDelegateInvoker(Object *sender)
{
	g_delegateInvoker = sender;
}

void Object::SetName(LPCSTR name)
{
	m_name = ltk::InternString(name);
}

LPCSTR Object::InternString(LPCSTR psz)
{
	if (!psz) {
		return nullptr;
	}
	
	LPCSTR result = nullptr;

	AutoLock lock(g_lockInternStr);

	auto iter = g_internedStrings.find(psz);
	if (iter == g_internedStrings.end()) {
		auto ret = g_internedStrings.insert(_strdup(psz));
		LTK_ASSERT(ret.second); // the insertion took place.
		result = (*ret.first);
	} else {
		result = *iter;
	}

	return result;
}

bool Object::CheckValid(Object* o)
{
	// TODO lock for multithread.
	return g_objectSet.find(o) != g_objectSet.end();
}

/////////////////////////////////////////////////////////////////

#ifdef LTK_C_API

void Object::DumpObjectLeaks()
{
	// TODO lock for multithread.
	char buf[512];
	for (Object* obj : *g_objectSet) {
		if (obj->m_source) {
			::StringCbPrintfA(
				buf, sizeof(buf), "%s(%d) : LtkObject leak [%s]\r\n",
				obj->m_source, obj->m_line, obj->TypeNameInstance());
		}
		else {
			::StringCbPrintfA(
				buf, sizeof(buf), "LtkObject leak [%s]\r\n",
				obj->TypeNameInstance());
		}
		::OutputDebugStringA(buf);
	}
}

void Object::RegisterCallback(UINT event_id, LtkCallback cb, void* userdata)
{
	size_t idx = (size_t)-1;
	for (size_t i = 0; i < m_vecCallbacks.size(); ++i) { // 正序查找 event_id
		if (m_vecCallbacks[i].code == event_id) {
			idx = i;
			break;
		}
	}
	if (idx == (size_t)-1) {
		CallbackNode node;
		node.code = event_id;
		m_vecCallbacks.push_back(node);
		idx = m_vecCallbacks.size() - 1;
	}
	auto& node = m_vecCallbacks[idx];
	size_t idx2 = (size_t)-1;
	for (size_t i = node.list.size(); i > 0; --i) { // 反序查找 userdata
		if (node.list[i].userdata == userdata) {
			idx2 = i;
			break;
		}
	}
	if (idx2 != (size_t)-1) {
		LTK_LOG("RegisterCallback duplicated userdata, event_id: %d", event_id);
		return;
	}
	CallbackInfo info;
	info.callback = cb;
	info.userdata = userdata;
	node.list.push_back(info);
}

#endif

} // namespace ltk
