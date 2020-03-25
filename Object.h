#pragma once
#include "RTTI.h"
#include "Common.h"

namespace ltk {


class LTK_CPP_API Object : public RTTI
{
public:
	RTTI_DECLARATIONS(Object, RTTI);

	Object() {}
	virtual ~Object() {}

	static Object *GetDelegateInvoker();
	static void SetDelegateInvoker(Object *);

	void SetName(LPCSTR name);
	LPCSTR GetName();

private:
	LPCSTR m_name = nullptr;
	DISALLOW_COPY_AND_ASSIGN(Object);
};

} // namespace ltk

