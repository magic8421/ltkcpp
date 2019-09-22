#pragma once
#include "Splitter.h"
#include "Sprite_p.h"

namespace ltk {

struct SplitterItem
{
	float size = 10.f;
	bool bFixed = false;
	Sprite* client = nullptr;
};

class SplitterPrivate : public SpritePrivate
{
public:
	SplitterPrivate(Splitter *q, Splitter::Mode m) :
		SpritePrivate(q), mode(m) {}
	virtual ~SplitterPrivate() {}

	std::vector<SplitterItem> vecItems;
	Splitter::Mode mode;
	bool bCapture = false;
	float dragPos = 0.f;
	int dragIdx = -1;

	int HitTest(float x, float y);
	float PosFromIdx(UINT idx);

	LTK_DECLARE_PUBLIC(Splitter);
};

} // namespace ltk
