#pragma once
#include "RTTI.h"
#include "Common.h"
#include "Container.h"
#include "LtkInterface.h"

namespace ltk {

struct CallbackInfo {
	LtkCallback callback = nullptr;
	void* userdata = nullptr;
};

class LTK_CPP_API Object : public RTTI
{
public:
	RTTI_DECLARATIONS(Object, RTTI);

	static void Free();

	Object();
	virtual ~Object();

	static Object *GetDelegateInvoker();
	static void SetDelegateInvoker(Object *);

	void SetName(LPCSTR name);
	LPCSTR GetName() { return m_name; }

	bool IsWidget() { return m_bWidget; }
	void AddChild(Object *);
	void RemoveChild(Object *o);
	Object *GetNthChild(size_t i) { return m_children[i]; }
	size_t GetChildCount() { return m_children.size(); }
	Object *GetParent() { return m_parent; }
	void SetParent(Object *);

	virtual void SetAttribute(LPCSTR name, LPCSTR value) {}

	/////////////////////////////////////////////////////////////////
	void SetSourceLine(LPCSTR source, int line);
	static bool CheckValid(Object* o);
	static void DumpObjectLeaks();
	void RegisterCallback(UINT event_id, LtkCallback cb, void* userdata);
	void InvokeCallback(UINT event_id, ...);
	std::vector<CallbackInfo>* GetCallbackList(UINT event_id);

protected:
	virtual void DoInvokeCallback(
		UINT event_id, LtkCallback cb, void* userdata, va_list args, BOOL *bHandled) {}

protected:
	bool m_bWidget = false;
	bool m_bDeleting = false;

private:
	LPCSTR m_name = nullptr;
	Object *m_parent = nullptr;
	ArrayList<Object *> m_children;

	std::unordered_map<UINT, std::vector<CallbackInfo>> m_mapCallbacks;

	const char* m_source = nullptr; // 好像没必要 外部使用者应该用umdh来查内存泄漏
	int m_line = -1;

	DISALLOW_COPY_AND_ASSIGN(Object);
};

} // namespace ltk

