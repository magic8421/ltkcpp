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
}

void Object::DeleteLater()
{
	HiddenWindow::PostDeleteLater(this);
}

void Object::Free()
{
#ifdef LTK_C_API
	delete sObjectSet;
#endif
	for (auto iter = g_internedStrings.begin(); iter != g_internedStrings.end(); iter++) {
		free((void*)*iter);
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

Object::~Object()
{
}

void Object::Free()
{
}

void Object::SetSourceLine(LPCSTR source, int line)
{
}

void Object::DumpObjectLeaks()
{
}

LPCSTR Object::InternString(LPCSTR psz)
{
	if (!psz) {
		return nullptr;
	}
	// TODO 多线程加锁
	auto iter = g_internedStrings.find(psz);
	if (iter == g_internedStrings.end()) {
		auto ret = g_internedStrings.insert(_strdup(psz));
		LTK_ASSERT(ret.second); // the insertion took place.
		return (*ret.first);
	} else {
		return *iter;
	}
}

LPCSTR Object::GetName()
{
	return m_name;
}

bool Object::CheckValid(Object* o)
{
	return true;
}

ULONG Object::AddRef()
{
	InterlockedIncrement(&m_cRef);
	return m_cRef;
}

ULONG Object::Release()
{
	ULONG ulRefCount = InterlockedDecrement(&m_cRef);
	if (0 == m_cRef) {
		delete this;
	}
	return ulRefCount;
}


} // namespace ltk
