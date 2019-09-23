#pragma once
#include "RTTI.h"

namespace ltk {

class ObjectPrivate;

class LTK_API Object : public RTTI
{
public:
	RTTI_DECLARATIONS(Object, RTTI);

	Object();
	virtual ~Object();

	void SetInvalid();

	void CheckThread();
	void MoveToThread(DWORD id);

	virtual void OnDeleted(Object *sender) {}
	virtual void OnDeleted(UINT member_id) {}

private:
	DISALLOW_COPY_AND_ASSIGN(Object);

protected:
	ObjectPrivate *d_ptr = nullptr;
	Object(ObjectPrivate *pp);
};

} // namespace ltk

