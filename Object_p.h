#pragma once
#include "Object.h"

namespace ltk {

class ObjectPrivate
{
public:
	ObjectPrivate();
	ObjectPrivate(Object *q);
	virtual ~ObjectPrivate() {}

	enum type { tObject, tSprite, tTreeNode, tLast };
	void SetSpriteType();
	void SetTreeNodeType();

protected:
	Object *q_ptr = nullptr;

private:
	DWORD thread_id = 0;
	DWORD type = 0;
	Object *parent = nullptr;
	std::vector<Object *> children;


private:
	friend class Object;
	DISALLOW_COPY_AND_ASSIGN(ObjectPrivate);
};

} // namespace ltk