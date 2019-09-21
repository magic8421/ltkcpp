#pragma once
#include "Object.h"

namespace ltk {

class ObjectPrivate
{
public:
	virtual ~ObjectPrivate() {}
	DWORD thread_id = 0; // TODO check gui thread
};

} // namespace ltk