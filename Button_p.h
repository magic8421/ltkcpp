#pragma once
#include "AbstractButton_p.h"

namespace ltk {

struct ButtonPrivate : public AbstractButtonPrivate
{
	ButtonPrivate(Button *pq);
	virtual ~ButtonPrivate();

	Button *q_ptr = nullptr;

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
