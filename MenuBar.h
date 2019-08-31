//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "Sprite.h"
#include "Delegate.h"

namespace ltk {

class Button;
class AbstractBackground;

struct MenuItem
{
public:
	MenuItem() {}

	std::wstring text;
	bool bChecked = false;
	bool bNewRadioGroup = false;
	bool bHasIcon = false;
	ltk::IconInfo icon;

	Delegate<void()> ClickedEvent;

private:
	DISALLOW_COPY_AND_ASSIGN(MenuItem)
};

class PopupMenu : public Sprite
{
public:
	PopupMenu();
	virtual ~PopupMenu();

	void AddMenu(LPCWSTR text, const std::function<void()> &cb);

	ImmutableString TextColor;
	ImmutableString HoverColor;
	ImmutableString TextFormat;
	ImmutableString Background;

private:
	std::vector<MenuItem *> m_vecItems;
	IDWriteTextFormat *m_format = nullptr;
	D2D1_COLOR_F m_textColor;
	AbstractBackground *m_background = nullptr;

protected:
	virtual bool OnPaint(PaintEvent *ev) override;
	virtual void OnThemeChanged() override;

};

struct MenuButtonParam {
	Button *button = nullptr;
	void *sub_menu = nullptr;
};

class MenuBar : public Sprite
{
public:
	MenuBar();
	virtual ~MenuBar();

	void AddItem(LPCWSTR text);
	UINT GetItemCount();
	void DoLayout();

protected:
	virtual bool OnSize(SizeEvent *ev) override;
	virtual void OnThemeChanged() override;

private:
	std::vector<MenuButtonParam> m_vecMenuItems;
};

} // namespace
