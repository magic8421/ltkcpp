#pragma once
#include "Trackable.h"
#include "Common.h"
#include "Container.h"
#include "LtkInterface.h"

namespace ltk {

struct CallbackInfo {
	LtkCallback callback = nullptr;
	void* userdata = nullptr;
};

class LTK_CPP_API Object : public Trackable
{
public:
	RTTI_DECLARATIONS(Object, Trackable);

	static void Free();

	Object();
	virtual ~Object();

	void DeleteLater();

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

	virtual void SetAttribute(LPCSTR name, LPCSTR value);
	virtual void OnChildAttribute(Object* child, LPCSTR name, LPCSTR value) {}


	/////////////////////////////////////////////////////////////////
	void SetSourceLine(LPCSTR source, int line);
	static bool CheckValid(Object* o);
	static void DumpObjectLeaks();
	void RegisterCallback(UINT event_id, LtkCallback cb, void* userdata);

	template<typename CB, typename... Params>
	int InvokeCallbacks(UINT event_id, Params... params);

	static LPCSTR InternString(LPCSTR str);

protected:
	bool m_bWidget = false;
	bool m_bDeleting = false;

private:
	LPCSTR m_name = nullptr;
	Object *m_parent = nullptr;
	ArrayList<Object *> m_children;

#ifdef LTK_C_API
	std::unordered_map<UINT, std::vector<CallbackInfo>> m_mapCallbacks;

	const char* m_source = nullptr; // 好像没必要 外部使用者应该用umdh来查内存泄漏
	int m_line = -1;
#endif

	DISALLOW_COPY_AND_ASSIGN(Object);
};


template<typename CB, typename... Params>
int Object::InvokeCallbacks(UINT event_id, Params... params)
{
#ifdef LTK_C_API
	auto iter = m_mapCallbacks.find(event_id);
	if (iter == m_mapCallbacks.end()) {
		return 0;
	}
	const auto& list = iter->second;
	for (size_t i = list.size(); i > 0; i--) {
		int ret = (int)((CB)list[i - 1].callback)(
			list[i - 1].userdata, std::forward<Params>(params)...);
		if (!ret)
			return ret;
	}
#endif
	return 0;
}

} // namespace ltk

