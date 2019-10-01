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

	void Delete();

	static void SetDelegeteInvoker(RTTI *sender);
	static RTTI *GetDelegateInvoker();

private:
	DISALLOW_COPY_AND_ASSIGN(Object);

protected:
	ObjectPrivate *d_ptr = nullptr;
	Object(ObjectPrivate *d);
};

} // namespace ltk

