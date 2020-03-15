#pragma once
#include "RTTI.h"
#include "Common.h"
#include "LtkInterface.h"

namespace ltk {


class LTK_CPP_API Object : public RTTI, public ILtkObject
{
public:
	RTTI_DECLARATIONS(Object, RTTI);

	Object();
	virtual ~Object();

	void Delete() override;

	static void Free();

	static Object *GetDelegateInvoker();
	static void SetDelegateInvoker(Object *);

	void SetSourceLine(LPCSTR source, int line);
	static bool CheckValid(Object* o);
	static void DumpObjectLeaks();

	void SetName(LPCSTR name);
	LPCSTR GetName();

private:
	LPCSTR m_name = nullptr;

	const char* m_source = nullptr; // 好像没必要 外部使用者应该用umdh来查内存泄漏
	int m_line = -1;

	DISALLOW_COPY_AND_ASSIGN(Object);
};

} // namespace ltk

