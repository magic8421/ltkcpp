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
		m_refCount++;
		LTK_ASSERT(m_refCount < 9999999);
		return m_refCount;
	}
	ULONG Release()
	{
		LTK_ASSERT(m_refCount > 0);

		if (--m_refCount == 0)         {
			delete this;
		}
		return m_refCount;
	}

	Object() {}

	virtual ~Object()
	{
		LTK_ASSERT(m_refCount == 0);
	}

	virtual void Dispose() {}

	static Object *GetDelegateInvoker();
	static void SetDelegateInvoker(Object *);

	void SetName(LPCSTR name);
	LPCSTR GetName() { return m_name; }

	/////////////////////////////////////////////////////////////////
	void SetSourceLine(LPCSTR source, int line);
	static void DumpObjectLeaks();

	static LPCSTR InternString(LPCSTR str);

private:
	ULONG m_refCount = 0;

	LPCSTR m_name = nullptr; // TODO: move this to Widget class

	DISALLOW_COPY_AND_ASSIGN(Object);
};


} // namespace ltk

