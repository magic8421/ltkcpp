#pragma once
#include "Sprite.h"

namespace ltk {

class Splitter : public Sprite
{
public:
	Splitter() {}
	virtual ~Splitter() {}

private:
	struct SplitterItem
	{
		float size = 10.f;
		Sprite* child_view = nullptr;
	};
};

} // namespace ltk
