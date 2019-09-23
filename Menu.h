//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "Sprite.h"

namespace ltk {

class PopupMenuPrivate;

class LTK_API PopupMenu : public Sprite
{
public:
	PopupMenu();
	virtual ~PopupMenu();

	void AddItem(LPCWSTR text);
	UINT GetChildCount();
	
	void SetWidth(float);
	float GetWidth();

	void SetSubMenu(UINT idx, PopupMenu *popup);

	void Show(Window *wnd, const RectF &rc);

	void SetTextColor(LPCSTR);
	void SetHoverColor(LPCSTR);
	void SetTextFormat(LPCSTR);
	void SetBackground(LPCSTR);

protected:
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
	friend class MenuBar;
};

class MenuBarPrivate;

class LTK_API MenuBar : public Sprite
{
public:
	MenuBar();
	virtual ~MenuBar();

	void AddItem(LPCWSTR text);
	void SetPopupMenu(UINT idx, PopupMenu *menu);
	void DoLayout();

protected:
	virtual bool OnSize(SizeEvent *ev) override;
	virtual void OnThemeChanged() override;

private:
	LTK_DECLARE_PRIVATE(MenuBar);

protected:
	MenuBar(MenuBarPrivate *d);
	friend class PopupMenu;
	friend class PopupMenuPrivate;
};

} // namespace
