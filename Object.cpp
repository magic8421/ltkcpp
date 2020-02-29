#include "stdafx.h"
#include "Object.h"
#include "Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

static __declspec(thread) Object *sDelegateInvoker = nullptr;


Object * Object::GetDelegateInvoker()
{
	return sDelegateInvoker;
}

void Object::SetDelegateInvoker(Object *sender)
{
	sDelegateInvoker = sender;
}

} // namespace ltk
