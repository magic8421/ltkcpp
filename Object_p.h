#pragma once
#include "Object.h"

namespace ltk {

struct ObjectPrivate 
{
	virtual ~ObjectPrivate() {}
	int dummy = 0;
};

} // namespace ltk