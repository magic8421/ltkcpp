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

	void AddChild(Object *child);
	void RemoveChild(Object *child);
	void SetParent(Object *parent);
	Object *GetParent();
	UINT GetChildCount();
	Object *GetChildAt(UINT idx);

	bool IsSpriteType();
	bool IsTreeNodeType();
	void SetTypeTag(DWORD type);
	DWORD GetTypeTag();

private:
	DISALLOW_COPY_AND_ASSIGN(Object);

protected:
	ObjectPrivate *d_ptr = nullptr;
	Object(ObjectPrivate *d);
	//friend class Sprite;
};

} // namespace ltk

