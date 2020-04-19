#include "stdafx.h"
#include "Object.h"
#include "Common.h"
#include "StyleManager.h"
#include "HiddenWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

static __declspec(thread) Object *sDelegateInvoker = nullptr;
static std::unordered_set<Object*>* sObjectSet;

typedef int (CALLBACK* ObjectDeleteCallback)(void *userdata);

CRITICAL_SECTION Object::m_lockInternStr;

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
#ifdef LTK_C_API
	// TODO lock for multithread.
	if (!sObjectSet) {
		sObjectSet = new std::unordered_set<Object*>;
	}
	sObjectSet->insert(this);
#endif
}

Object::~Object()
{
	this->DeleteDelegate();

#ifdef LTK_C_API
	// TODO lock for multithread.
	auto iter = sObjectSet->find(this);
	if (iter == sObjectSet->end()) {
		LTK_ASSERT(false);
	}
	sObjectSet->erase(iter);

	InvokeCallbacks<ObjectDeleteCallback>(LTK_DELETE_EVENT);
#endif

}

void Object::DeleteLater()
{
	HiddenWindow::PostDeleteLater(this);
}

void Object::Init()
{
	::InitializeCriticalSection(&m_lockInternStr);
}

void Object::Free()
{
#ifdef LTK_C_API
	delete sObjectSet;
#endif

	::EnterCriticalSection(&m_lockInternStr);
	for (auto iter = g_internedStrings.begin(); iter != g_internedStrings.end(); iter++) {
		free((void*)*iter);
	}
	::LeaveCriticalSection(&m_lockInternStr);

	::DeleteCriticalSection(&m_lockInternStr);
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

LPCSTR Object::InternString(LPCSTR psz)
{
	if (!psz) {
		return nullptr;
	}
	
	LPCSTR result = nullptr;
	::EnterCriticalSection(&m_lockInternStr);

	auto iter = g_internedStrings.find(psz);
	if (iter == g_internedStrings.end()) {
		auto ret = g_internedStrings.insert(_strdup(psz));
		LTK_ASSERT(ret.second); // the insertion took place.
		result = (*ret.first);
	} else {
		result = *iter;
	}
	::LeaveCriticalSection(&m_lockInternStr);

	return result;
}

/////////////////////////////////////////////////////////////////

#ifdef LTK_C_API
void Object::SetSourceLine(LPCSTR source, int line)
{
	m_source = source;
	m_line = line;
}

bool Object::CheckValid(Object* o)
{
	// TODO lock for multithread.
	return sObjectSet->find(o) != sObjectSet->end();
}

void Object::DumpObjectLeaks()
{
	// TODO lock for multithread.
	char buf[512];
	for (Object* obj : *sObjectSet) {
		::StringCbPrintfA(
			buf, sizeof(buf), "LtkObject leak: [%s] %s(%d)\r\n",
			obj->TypeNameInstance(), obj->m_source, obj->m_line);
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
#else
void Object::SetSourceLine(LPCSTR source, int line) {}
bool Object::CheckValid(Object* o) { return true; }
void Object::DumpObjectLeaks() {}
void Object::RegisterCallback(UINT event_id, LtkCallback cb, void* userdata) {}
#endif

} // namespace ltk
