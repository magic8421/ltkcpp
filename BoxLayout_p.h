#pragma once
#include "Splitter_p.h"

namespace ltk {

struct BoxLayoutParam {
	Sprite *item = nullptr;
	float size = 20.0f;
	float growFactor = 0.0f;
};

class BoxLayoutPrivate : public SpritePrivate
{
public:
	LTK_DECLARE_PUBLIC(BoxLayout);

	BoxLayoutPrivate(BoxLayout *q, BoxLayout::Mode m);
	virtual ~BoxLayoutPrivate() {}

	std::vector<BoxLayoutParam> params; // item: owner
	BoxLayout::Mode mode;
	float spacing = 5.0f;
};

} // namespace ltk
