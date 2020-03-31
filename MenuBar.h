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

class MenuItem : public Object
{
public:
	MenuItem() {}

	enum class Type {Normal, Icon, Check, Radio};

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

class PopupMenu : public ILtkPopupMenu, public Widget
{
public:
	PopupMenu();
	virtual ~PopupMenu();

	LTK_OBJECT_IMPL()
	LTK_WIDGET_IMPL()
	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject) override { return E_NOTIMPL; }

	STDMETHOD_(void, AddItem)(LPCWSTR text, LPCSTR name) override;
	STDMETHOD_(void, AddSeparator)() override;

	UINT GetMenuItemCount();
	MenuItem *GetMenuItemAt(UINT idx);

	MenuItem *FindChildItem(LPCSTR name);
	MenuItem *FindChildItemInterned(LPCSTR name); // name必须内部化

	float GetHeight();
	
	STDMETHOD_(void, SetWidth)(float) override;
	float GetWidth();
	void CalcWidth();

	STDMETHOD_(void,  SetSubMenu)(UINT idx, ILtkPopupMenu *popup) override;
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

	static HRESULT GetTextExtent(LPCWSTR str, IDWriteTextFormat *format, LtkSize *size);

private:
	int IndexFromPos(float y);
	RectF RectFromIndex(int idx);

private:
	std::vector<MenuItem *> m_vecItems;
	PopupMenu* m_parent = nullptr;
	MenuBar* m_menuBar = nullptr;

	int m_hoverIdx = -1;
	int m_trackingIdx = -1;
	bool m_bTrackingPopup = false;
	bool m_bHiding = false;

	enum class State {sHide, sSlideIn, sShow};
	State m_state = State::sHide;
	float m_aniProgress = 0.f;
	const float AniDelta = 1.f / 300.f;
	DWORD m_lastTick = 0;

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
	Button *button = nullptr;
	PopupMenu *sub_menu = nullptr;
};

class MenuBar : public ILtkMenuBar, public Widget
{
public:
	MenuBar();
	virtual ~MenuBar();

	LTK_OBJECT_IMPL()
	LTK_WIDGET_IMPL()
	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject) override { return E_NOTIMPL; }

	STDMETHOD_(void, AddItem)(LPCWSTR text) override;
	STDMETHOD_(void, SetPopupMenu)(UINT idx, ILtkPopupMenu *menu) override;
	//void AddItem(LPCWSTR text);
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
