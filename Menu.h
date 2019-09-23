//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "Sprite.h"
#include "Delegate.h"
#include "ltk.h"
#include "ImmutableString.h"

namespace ltk {

class Button;
class AbstractBackground;
class MenuBar;

class PopupMenuPrivate;

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

	void SetTextColor(LPCSTR);
	void SetHoverColor(LPCSTR);
	void SetTextFormat(LPCSTR);
	void SetBackground(LPCSTR);

	virtual bool OnPaint(PaintEvent *ev) override;
	virtual bool OnKillFocus(FocusEvent* ev) override;
	virtual bool OnLBtnDown(MouseEvent* ev) override;
	virtual bool OnMouseMove(MouseEvent* ev) override;
	virtual bool OnMouseLeave(MouseEvent* ev) override;
	virtual void OnThemeChanged() override;
	virtual void OnParentChanged(Sprite* old, Sprite* new_) override;

private:
	LTK_DECLARE_PRIVATE(PopupMenu);

protected:
	PopupMenu(PopupMenuPrivate *d);
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
