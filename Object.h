#pragma once
#include "LtkInterface.h"
#include "RTTI.h"

namespace ltk {


struct CallbackInfo
{
	LtkCallback callback = nullptr;
	void* userdata = nullptr;
};

class Object : public RTTI
{
public:
	RTTI_DECLARATIONS(Object, RTTI);

	Object() {}
	virtual ~Object() {}

	void RegisterCallback(UINT event_id, LtkCallback cb, void* userdata);
	void InvokeCallback(UINT event_id, ...);

	virtual BOOL DoInvokeCallback(UINT event_id, LtkCallback cb,
		void* userdata, va_list args) { return 1; }

	static Object *GetEventSender();

private:
	std::map<UINT, std::vector<CallbackInfo>> m_mapCallbacks;
	static Object *m_staticSender;
};

} // namespace ltk

