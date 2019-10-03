#pragma once
#include "ListView.h"
#include "ImmutableString.h"
#include "Sprite_p.h"

namespace ltk {

class ScrollBar;
class HeaderCtrl;

struct LineData
{
	std::vector<std::wstring> cells;
};

class ListViewPrivate : public SpritePrivate
{
public:
	ListViewPrivate(ListView *q);
	virtual ~ListViewPrivate();

	void Init();
	void HandleVScrollBar(float pos);
	void HandleHScrollBar(float pos);
	void OnHeaderDelete();
	void RemoveHeaderDelegates();

private:
	ImmutableString TextColor;
	ImmutableString HoverColor;
	ImmutableString SelectedColor;
	ImmutableString SelectedTextColor;
	ImmutableString TextFormat;

	enum State
	{
		stStop, stScrollUp, stScrollDown
	};
	std::vector<LineData> vecData;
	ID2D1SolidColorBrush *brush = nullptr;
	IDWriteTextFormat *textFormat = nullptr;
	ScrollAnimation scroll;
	ScrollBar *vsb = nullptr;
	ScrollBar *hsb = nullptr;
	HeaderCtrl *header = nullptr;
	int hoverItem = -1;
	int selectedItem = -1;
	std::vector<float> vecColumns;
	float hscroll = 0.0f;
	float accumulateVScroll = 0.f; // for high precision trackpad

protected:
	D2D1_COLOR_F textColor;
	D2D1_COLOR_F hoverColor;
	D2D1_COLOR_F selectedColor;
	D2D1_COLOR_F selectedTextColor;

public:
	LTK_DECLARE_PUBLIC(ListView);
};

} // namespace ltk
