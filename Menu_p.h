#pragma once
#include "Sprite_p.h"

namespace ltk {

struct MenuItem
{
public:
	MenuItem() {}

	enum Type { Normal, Icon, Check, Radio };

	Type type = Normal;
	std::wstring text;
	bool bChecked = false;
	bool bNewRadioGroup = false;
	ltk::IconInfo icon;
	PopupMenu* sub_menu = nullptr;

	Delegate<void()> ClickedEvent;

private:
	DISALLOW_COPY_AND_ASSIGN(MenuItem)
};

class PopupMenuPrivate : public SpritePrivate
{
public:
	LTK_DECLARE_PUBLIC(PopupMenu);

	PopupMenuPrivate(PopupMenu *q);
	virtual ~PopupMenuPrivate();

	std::vector<MenuItem *> vecItems;
	PopupMenu* parent = nullptr;
	MenuBar* menuBar = nullptr;
	float width = 100.0f;
	int hoverIdx = -1;
	int trackingIdx = -1;
	bool bTrackingPopup = false; // TODO 这个能和trackingIdx合并吗？
	UINT hoverTimer = 0;
	bool bHiding = false;

	enum State { sHide, sSlideIn, sShow };
	State state = sHide;
	float aniProgress = 0.f;
	DWORD lastTick = 0;

	IDWriteTextFormat* format = nullptr;
	D2D1_COLOR_F textColor;
	D2D1_COLOR_F hoverColor;
	AbstractBackground* background = nullptr;

	ImmutableString TextColor;
	ImmutableString HoverColor;
	ImmutableString TextFormat;
	ImmutableString Background;
};


} // namespace
