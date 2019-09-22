#pragma once
#include "Object.h"

namespace ltk {

class ObjectPrivate
{
public:
	ObjectPrivate();
	ObjectPrivate(Object *q) : q_ptr(q) {}
	virtual ~ObjectPrivate() {}

	DWORD thread_id = 0;
	Object *q_ptr = nullptr;
};

} // namespace ltk