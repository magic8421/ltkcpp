#pragma once
#include "Trackable.h"
#include "Common.h"
#include "Container.h"
#include "LtkInterface.h"
#include "MulticastDelegate.h"

namespace ltk {

struct CallbackInfo {
	LtkCallback callback = nullptr;
	void* userdata = nullptr;
};

class LTK_CPP_API Object : public RTTI
{
public:
	RTTI_DECLARATIONS(Object, RTTI);

	static void Init();
	static void Free();

	Object();
	virtual ~Object();

	void DeleteLater();

	static Object *GetDelegateInvoker();
	static void SetDelegateInvoker(Object *);

	void SetName(LPCSTR name);
	LPCSTR GetName() { return m_name; }

	MulticastDelegate0 DeleteDelegate;

	void RegisterCallback(UINT event_id, LtkCallback cb, void* userdata);

	template<typename CB, typename... Params>
	int InvokeCallbacks(UINT event_id, Params... params);

	static LPCSTR InternString(LPCSTR str);

	static void Track(Object* obj);
	static bool CheckValid(Object* o);

private:
	LPCSTR m_name = nullptr; // Move to Widget
	bool m_bTracking = false;

#ifdef LTK_C_API

	struct CallbackNode
	{
		UINT code;
		std::vector<CallbackInfo> list;
	};
	std::vector<CallbackNode> m_vecCallbacks;

	const char* m_source = nullptr; // 好像没必要 外部使用者应该用umdh来查内存泄漏
	int m_line = -1;
#endif

	DISALLOW_COPY_AND_ASSIGN(Object);
};


template<typename CB, typename... Params>
int Object::InvokeCallbacks(UINT event_id, Params... params)
{
#ifdef LTK_C_API
	size_t idx = (size_t)-1;
	for (size_t i = 0; i < m_vecCallbacks.size(); ++i) {
		if (m_vecCallbacks[i].code == event_id) {
			idx = i;
			break;
		}
	}
	if (idx != (size_t)-1) {
		const auto& node = m_vecCallbacks[idx];
		for (size_t i = 0; i < node.list.size(); ++i) {
			int ret = (int)((CB)node.list[i].callback)(
				node.list[i].userdata, std::forward<Params>(params)...);
			if (ret)
				return ret;
		}
	}
#endif
	return 0;
}

} // namespace ltk

