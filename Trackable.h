#pragma once

#include "RTTI.h"

namespace ltk {

class Trackable : public RTTI
{
public:
	RTTI_DECLARATIONS(Trackable, RTTI);

	Trackable();
	~Trackable();

	static void DumpLeaks();

private:
	Trackable* m_prev = nullptr;
	Trackable* m_next = nullptr;
	LPCSTR m_alloc_info = nullptr;
};

} // namespace ltk
