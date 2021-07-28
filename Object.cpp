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

CriticalSection g_lockInternStr;

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

CriticalSection g_lockTrackAlloc;
static Object* g_track_head = nullptr;
static Object* g_track_tail = nullptr;

void Object::Init()
{
}

void Object::Free()
{
	do {
		AutoLock lock(g_lockInternStr);
		for (auto iter = g_internedStrings.begin(); iter != g_internedStrings.end(); iter++) {
			free((void*)*iter);
		}
	} while (0);
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
	do {
		AutoLock lock(g_lockInternStr);

		auto iter = g_internedStrings.find(psz);
		if (iter == g_internedStrings.end()) {
			auto ret = g_internedStrings.insert(_strdup(psz));
			LTK_ASSERT(ret.second); // the insertion took place.
			result = (*ret.first);
		}
		else {
			result = *iter;
		}
	} while (0);

	return result;
}

} // namespace ltk
