#pragma once
#include "RTTI.h"
#include "Common.h"
#include "Container.h"

namespace ltk {


class LTK_CPP_API Object : public RTTI
{
public:
	RTTI_DECLARATIONS(Object, RTTI);

	Object() {}
	virtual ~Object();

	static Object *GetDelegateInvoker();
	static void SetDelegateInvoker(Object *);

	void SetName(LPCSTR name);
	LPCSTR GetName();

	bool IsWidget() { return m_bWidget; }
	void AddChild(Object *);
	void RemoveChild(Object *o);
	Object *GetNthChild(size_t i) { return m_children[i]; }
	size_t GetChildCount() { return m_children.size(); }
	Object *GetParent() { return m_parent; }

protected:
	bool m_bWidget = false;

private:
	LPCSTR m_name = nullptr;
	Object *m_parent = nullptr;
	ArrayList<Object *> m_children;
	DISALLOW_COPY_AND_ASSIGN(Object);
};

} // namespace ltk

