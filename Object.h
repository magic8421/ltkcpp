#pragma once
#include "RTTI.h"
#include "Common.h"
#include "Container.h"
#include "LtkInterface.h"

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
	void SetParent(Object *);

	virtual void SetAttribute(LPCSTR name, LPCSTR value) {}

	/////////////////////////////////////////////////////////////////
	
	void RegisterEvent(UINT code, LtkCallback cb, void *userdata);

private:
	struct CallbackInfo {
		void* userdata = nullptr;
		LtkCallback callback = nullptr;
	};
	void AddEventToList(std::vector<CallbackInfo>& list, LtkCallback callback, void* userdata);

protected:
	bool m_bWidget = false;
	bool m_bDeleting = false;

private:
	LPCSTR m_name = nullptr;
	Object *m_parent = nullptr;
	ArrayList<Object *> m_children;


	std::unordered_map<UINT, std::vector<CallbackInfo>> m_mapCallback;

	DISALLOW_COPY_AND_ASSIGN(Object);
};

} // namespace ltk

