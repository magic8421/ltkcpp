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

	bool IsWidget();
	void AddChild(Object *);
	Object *GetNthChild(size_t i);
	size_t GetChildCount();

private:
	LPCSTR m_name = nullptr;
	bool m_bWidget = false;
	DISALLOW_COPY_AND_ASSIGN(Object);
};

} // namespace ltk

