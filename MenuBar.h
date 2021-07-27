//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "ltk.h"
#include "Sprite.h"
#include "MulticastDelegate.h"

namespace ltk {

class Button;
class AbstractBackground;
class MenuBar;
class PopupMenu;

struct LTK_CPP_API MenuItem : public Object
{
public:
	RTTI_DECLARATIONS(MenuItem, Object);
	MenuItem() {}

	enum class Type {Normal, Icon, Check, Radio};

	MulticastDelegate0 ClickedDelegate;

private:
	Type type = Type::Normal;
	std::wstring text;
	bool bChecked = false;
	bool bNewRadioGroup = false;
	ltk::IconInfo icon;
	PopupMenu* sub_menu = nullptr;


private:
	friend class PopupMenu;
	DISALLOW_COPY_AND_ASSIGN(MenuItem)
};

class LTK_CPP_API PopupMenu : public Widget
{
public:
	RTTI_DECLARATIONS(PopupMenu, Widget);

	PopupMenu();
	virtual ~PopupMenu();

	void AddItem(LPCWSTR text, LPCSTR name);
	void AddSeparator();

	UINT GetMenuItemCount();
	Ptr<MenuItem> GetMenuItemAt(UINT idx);

	Ptr<MenuItem> FindChildItem(LPCSTR name);
	Ptr<MenuItem> FindChildItemInterned(LPCSTR name); // name必须内部化

	float GetHeight();
	
	void SetWidth(float);
	float GetWidth();
	void CalcWidth();

	void SetSubMenu(UINT idx, PopupMenu *popup);
	void SetMenuBar(MenuBar*);

	void Show(Window *wnd, const RectF &rc);
	void Hide();
	void HideAll();
	void TrackPopupMenu(int idx);


	virtual bool OnPaint(PaintEvent *ev) override;
	virtual bool OnKillFocus(FocusEvent* ev) override;
	virtual bool OnLBtnDown(MouseEvent* ev) override;
	virtual bool OnMouseMove(MouseEvent* ev) override;
	virtual bool OnMouseLeave(MouseEvent* ev) override;
	virtual void OnThemeChanged() override;
	virtual void OnParentChanged(Widget* old, Widget* new_) override;

	void SetTextColor(LPCSTR style);
	void SetHoverColor(LPCSTR style);
	void SetTextFormat(LPCSTR style);
	void SetBackground(LPCSTR style);

	static HRESULT GetTextExtent(LPCWSTR str, IDWriteTextFormat *format, SizeF &size);

private:
	int IndexFromPos(float y);
	RectF RectFromIndex(int idx);

private:
	std::vector<Ptr<MenuItem>> m_vecItems;
	PopupMenu* m_parent = nullptr;
	MenuBar* m_menuBar = nullptr;

	int m_hoverIdx = -1;
	int m_trackingIdx = -1;
	bool m_bTrackingPopup = false;
	bool m_bHiding = false;

	enum class State {sHide, sSlideIn, sShow};
	State m_state = State::sHide;
	float m_aniProgress = 0.f;
	const float AniDuration = 500.f;
	DWORD m_lastTick = 0;
	float m_aniSlide = 0.f;

	float m_width = 100.0f;
	float m_text_h = 10.f;

	IDWriteTextFormat* m_format = nullptr;
	D2D1_COLOR_F m_textColor;
	D2D1_COLOR_F m_hoverColor;
	AbstractBackground* m_background = nullptr;

	LPCSTR m_szTextColor = nullptr;
	LPCSTR m_szHoverColor = nullptr;
	LPCSTR m_szTextFormat = nullptr;
	LPCSTR m_szBackground = nullptr;
};

struct MenuButtonParam {
	Ptr<Button> button;
	Ptr<PopupMenu> sub_menu;
};

class LTK_CPP_API MenuBar : public Widget
{
public:
	RTTI_DECLARATIONS(MenuBar, Widget);

	MenuBar();
	virtual ~MenuBar();

	UINT AddItem(LPCWSTR text);
	void SetPopupMenu(UINT idx, PopupMenu *menu);
	UINT GetItemCount();
	void DoLayout();
	void OnMenuHide();

protected:
	virtual bool OnSize(SizeEvent *ev) override;
	virtual void OnThemeChanged() override;

	void OnMenuBtnClicked();
	void OnButtonMouseEvent(MouseEvent* ev, bool& bHandled);

	int FindMenuButtonIdx(Button *btn);

private:
	std::vector<MenuButtonParam> m_vecMenuItems;
	int m_trackingIdx = -1;
};

} // namespace
