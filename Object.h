#pragma once
#include "Common.h"
#include "LtkInterface.h"

namespace ltk {


#define LTK_OBJECT_IMPL() \
	STDMETHOD_(ULONG, AddRef)() override { \
		return Object::AddRef(); \
	} \
	STDMETHOD_(ULONG, Release)() override { \
		return Object::Release(); \
	}

class Object
{
public:
	Object();
	virtual ~Object();

	static void Free();

	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

	static Object *GetDelegateInvoker();
	static void SetDelegateInvoker(Object *);

	void SetSourceLine(LPCSTR source, int line);
	static bool CheckValid(Object* o);
	static void DumpObjectLeaks();

	void SetName(LPCSTR name);
	LPCSTR GetName();

private:
	LPCSTR m_name = nullptr;

	volatile ULONG m_cRef = 1;

	DISALLOW_COPY_AND_ASSIGN(Object);
};

} // namespace ltk

