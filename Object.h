#pragma once
#include "RTTI.h"
#include "Common.h"
#include "LtkInterface.h"

namespace ltk {


class LTK_CPP_API Object : public RTTI
{
public:
	RTTI_DECLARATIONS(Object, RTTI);

	Object();
	virtual ~Object();
	static void Free();

	static Object *GetDelegateInvoker();
	static void SetDelegateInvoker(Object *);

	void AddListener(void* userdata, LtkCallback callback);
	void RemoveListener(void* userdata, LtkCallback callback);
	BOOL FireEvent(LtkEvent* ev);
	BOOL CallNextEventHandler(LtkEvent* ev);

	void SetSourceLine(LPCSTR source, int line);
	static bool CheckValid(Object* o);
	static void DumpObjectLeaks();

	void SetName(LPCSTR name);
	LPCSTR GetName();

private:
	struct CallbackInfo {
		void* userdata = nullptr;
		LtkCallback callback = nullptr;
	};
	std::vector<CallbackInfo> m_vecCallback;
	int m_currentCallback = -1;

	LPCSTR m_name = nullptr;

	const char* m_source = nullptr; // 好像没必要 外部使用者应该用umdh来查内存泄漏
	int m_line = -1;

	DISALLOW_COPY_AND_ASSIGN(Object);
};

} // namespace ltk

