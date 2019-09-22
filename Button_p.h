#pragma once
#include "AbstractButton_p.h"
#include "ImmutableString.h"

namespace ltk {

class ButtonPrivate : public AbstractButtonPrivate
{
public:
	ButtonPrivate(Button *pq);
	virtual ~ButtonPrivate();

	LTK_DECLARE_PUBLIC(Button);

	std::wstring text;
	AbstractBackground *background = nullptr;
	IDWriteTextFormat *format = nullptr;
	IDWriteTextLayout *layout = nullptr;
	D2D1_COLOR_F textColor;

	ImmutableString Background;
	ImmutableString TextFormat;
	ImmutableString TextColor;
};
} // namespace
