#pragma once
#include "Object.h"

namespace ltk {

class ObjectPrivate
{
public:
	ObjectPrivate();
	ObjectPrivate(Object *q);
	virtual ~ObjectPrivate() {}

protected:
	Object *q_ptr = nullptr;
	bool isSpriteType = false;

private:
	DWORD thread_id = 0;
	Object *parent = nullptr;
	std::vector<Object *> children;


private:
	friend class Object;
	DISALLOW_COPY_AND_ASSIGN(ObjectPrivate);
};

} // namespace ltk