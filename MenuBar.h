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
class MenuBar;

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

	void AddItem(LPCWSTR text);
	UINT GetChildCount();
	
	void SetWidth(float);
	float GetWidth();

	ImmutableString TextColor;
	ImmutableString HoverColor;
	ImmutableString TextFormat;
	ImmutableString Background;

	virtual bool OnPaint(PaintEvent *ev) override;
	virtual bool OnKillFocus(FocusEvent* ev) override;
	virtual bool OnMouseMove(MouseEvent* ev) override;
	virtual bool OnMouseLeave(MouseEvent* ev) override;
	virtual void OnThemeChanged() override;
	virtual void OnParentChanged(Sprite* old, Sprite* new_) override;

private:
	std::vector<MenuItem *> m_vecItems;
	IDWriteTextFormat *m_format = nullptr;
	D2D1_COLOR_F m_textColor;
	D2D1_COLOR_F m_hoverColor;
	AbstractBackground *m_background = nullptr;
	PopupMenu* m_parent = nullptr;
	float m_width = 100.0f;
	int m_hoverIdx = -1;
};

struct MenuButtonParam {
	Button *button = nullptr;
	PopupMenu *sub_menu = nullptr;
};

class MenuBar : public Sprite
{
public:
	MenuBar();
	virtual ~MenuBar();

	void AddItem(LPCWSTR text);
	void SetPopupMenu(UINT idx, PopupMenu *menu);
	UINT GetItemCount();
	void DoLayout();

protected:
	virtual bool OnSize(SizeEvent *ev) override;
	virtual void OnThemeChanged() override;

	void OnMenuBtnClicked(UINT idx);

private:
	std::vector<MenuButtonParam> m_vecMenuItems;
};

} // namespace
