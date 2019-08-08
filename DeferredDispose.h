#pragma once
#include "stdafx.h"

namespace ltk {

class RefCounted;

class DeferredDispose
{
protected:
	DeferredDispose();
	~DeferredDispose();

	typedef void(*DisposeCallback)(void *);

	struct DisposeParam
	{
		void *udata;
		DisposeCallback callback;
	};

public:
    static DeferredDispose *Instance();
    static void Free();

    static void Collect();
	static void AutoRelease(RefCounted *rc);
    static void AutoFree(void *p);
    static void AutoDispose(void *p, DisposeCallback cb);

private:
	static void DisposeRefCounted(void *);

private:
	std::vector<DisposeParam> m_vecDeferred;
	__declspec(thread) static DeferredDispose *m_sInstance;
};

} // namespace ltk
