#pragma once
#include "Label.h"
#include "Sprite_p.h"

namespace ltk {

class LabelPrivate : public SpritePrivate
{
public:
	LTK_DECLARE_PUBLIC(Label);

	LabelPrivate(Label *q);
	virtual ~LabelPrivate() {}

private:
	std::wstring text;
	IDWriteTextFormat *textFormat = nullptr;
	D2D1_COLOR_F color;

	ImmutableString TextFormat;
	ImmutableString TextColor;
};

} // namespace ltk
