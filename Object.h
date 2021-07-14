#pragma once
#include "RefCounted.h"
#include "Common.h"
#include "Container.h"
#include "MulticastDelegate.h"

#define TRACE_ALLOC(obj) (obj)->SetSourceLine(__FILE__, __LINE__);

namespace ltk {

class LTK_CPP_API Object : public RTTI
{
public:
	RTTI_DECLARATIONS(Object, RTTI);

	static void Init();
	static void Free();

	ULONG AddRef()
	{
		InterlockedIncrement(&m_refCount);
		LTK_ASSERT(m_refCount < 9999999);
		return m_refCount;
	}
	ULONG Release()
	{
		// https://docs.microsoft.com/en-us/office/client-developer/outlook/mapi/implementing-iunknown-in-c-plus-plus
		ULONG ulRefCount = InterlockedDecrement(&m_refCount);
		if (0 == m_refCount)
		{
			delete this;
		}
		return ulRefCount;
	}

	virtual void Dispose() {}

	Object();
	virtual ~Object();

	void DeleteLater();

	static Object *GetDelegateInvoker();
	static void SetDelegateInvoker(Object *);

	void SetName(LPCSTR name);
	LPCSTR GetName() { return m_name; }

	/////////////////////////////////////////////////////////////////
	void SetSourceLine(LPCSTR source, int line);
	static void DumpObjectLeaks();

	static LPCSTR InternString(LPCSTR str);

private:
	ULONG volatile m_refCount;

#ifndef LTK_NO_TRACE_MEMORY
	Object* m_prev = nullptr;
	Object* m_next = nullptr;
	const char* m_source = nullptr;
	int m_line = -1;
#endif // !LTK_NO_TRACE_MEMORY

	LPCSTR m_name = nullptr; // TODO: move this to Widget class

	DISALLOW_COPY_AND_ASSIGN(Object);
};


} // namespace ltk

