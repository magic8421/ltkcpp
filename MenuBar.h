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
class PopupMenu;

struct MenuItem
{
public:
	MenuItem() {}

	enum Type {Normal, Icon, Check, Radio};

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

class PopupMenu : public Sprite
{
public:
	PopupMenu();
	virtual ~PopupMenu();

	void AddItem(LPCWSTR text);
	UINT GetChildCount();
	
	void SetWidth(float);
	float GetWidth();

	void SetSubMenu(UINT idx, PopupMenu *popup);
	void SetMenuBar(MenuBar*);

	void Show(Window *wnd, const RectF &rc);
	void Hide();
	void HideAll();
	void TrackPopupMenu(UINT idx);

	ImmutableString TextColor;
	ImmutableString HoverColor;
	ImmutableString TextFormat;
	ImmutableString Background;

	virtual bool OnPaint(PaintEvent *ev) override;
	virtual bool OnKillFocus(FocusEvent* ev) override;
	virtual bool OnLBtnDown(MouseEvent* ev) override;
	virtual bool OnMouseMove(MouseEvent* ev) override;
	virtual bool OnMouseLeave(MouseEvent* ev) override;
	virtual void OnThemeChanged() override;
	virtual void OnParentChanged(Sprite* old, Sprite* new_) override;

private:
	std::vector<MenuItem *> m_vecItems;
	PopupMenu* m_parent = nullptr;
	MenuBar* m_menuBar = nullptr;
	float m_width = 100.0f;
	int m_hoverIdx = -1;
	int m_trackingIdx = -1;
	bool m_bTrackingPopup = false; // TODO 这个能和m_trackingIdx合并吗？
	UINT m_hoverTimer = 0;
	bool m_bHiding = false;

	enum State {sHide, sSlideIn, sShow};
	State m_state = sHide;
	float m_aniProgress = 0.f;
	const float AniDelta = 1.f / 300.f;
	DWORD m_lastTick = 0;

	IDWriteTextFormat* m_format = nullptr;
	D2D1_COLOR_F m_textColor;
	D2D1_COLOR_F m_hoverColor;
	AbstractBackground* m_background = nullptr;
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
	void OnMenuHide();

protected:
	virtual bool OnSize(SizeEvent *ev) override;
	virtual void OnThemeChanged() override;

	void OnMenuBtnClicked(UINT idx);
	void OnButtonMouseEvent(Button* btn, MouseEvent* ev, bool& bHandled);

private:
	std::vector<MenuButtonParam> m_vecMenuItems;
	int m_trackingIdx = -1;
};

} // namespace
