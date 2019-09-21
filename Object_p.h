#pragma once
#include "Object.h"

namespace ltk {

struct ObjectPrivate 
{
	virtual ~ObjectPrivate() {}
	DWORD thread_id = 0; // TODO check gui thread
};

} // namespace ltk