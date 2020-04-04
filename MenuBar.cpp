//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "MenuBar.h"
#include "Button.h"
#include "Window.h"
#include "TimerManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

const static float ITEM_HEIGHT = 30.f;
const static float SEPARATOR_HEIGHT = 10.f;
const static float MENU_WIDTH = 300.f;
const static float ICON_WIDTH = 0.f;
const static float PADDING_V = 4.f;
const static float PADDING_H = 15.f;
const static float SHADOW_SIZE = 7.F;

PopupMenu::PopupMenu() :
	m_szTextColor("item_text_clr"),
	m_szTextFormat("popup_menu_fmt"),
	m_szHoverColor("menu_hover_clr"),
	m_szBackground("popup_menu_bg"),
	m_textColor(D2D1::ColorF(D2D1::ColorF::Cyan)),
	m_hoverColor(D2D1::ColorF(D2D1::ColorF::Cyan))
{
}

PopupMenu::~PopupMenu()
{
	for (auto item : m_vecItems) {
		if (item) {
			delete item->sub_menu;
			delete item;
		}
	}
}

void PopupMenu::AddItem(LPCWSTR text, LPCSTR name)
{
	auto item = new MenuItem;
	item->text = text;
	item->SetName(name);
	m_vecItems.push_back(item);
}

void PopupMenu::AddSeparator()
{
	m_vecItems.push_back(nullptr);
}

UINT PopupMenu::GetMenuItemCount()
{
	return m_vecItems.size();
}

MenuItem * PopupMenu::GetMenuItemAt(UINT idx)
{
	return m_vecItems[idx];
}

MenuItem* PopupMenu::FindChildItem(LPCSTR name)
{
	auto i_name = ltk::InternString(name);
	return FindChildItemInterned(i_name);
}

MenuItem* PopupMenu::FindChildItemInterned(LPCSTR name)
{
	for (auto item : m_vecItems) {
		if (item->GetName() == name) {
			return item;
		}
		if (item->sub_menu) {
			auto found = item->sub_menu->FindChildItem(name);
			if (found) {
				return found;
			}
		}
	}
	return nullptr;
}

float PopupMenu::GetHeight()
{
	float h = SHADOW_SIZE;
	for (auto item : m_vecItems) {
		if (item) {
			h += PADDING_V * 2;
			h += m_text_h;
		}
		else {
			h += SEPARATOR_HEIGHT;
		}
	}
	h += SHADOW_SIZE;
	return h;
}

void PopupMenu::SetWidth(float w)
{
	m_width = w;
}

float PopupMenu::GetWidth()
{
	return m_width;
}

void PopupMenu::SetSubMenu(UINT idx, PopupMenu *popup)
{
	LTK_ASSERT(m_vecItems[idx]->sub_menu == nullptr);
	popup->m_parent = this;
	m_vecItems[idx]->sub_menu = popup;
}

void PopupMenu::SetMenuBar(MenuBar *b)
{
	m_menuBar = b;
}

void PopupMenu::Show(Window* wnd, const RectF& rc)
{
	if (!wnd) {
		return;
	}
	auto root = wnd->GetRootWidget();
	root->AddChild(this);
	this->SetRect(rc);
	wnd->SetFocusWidget(this);
	m_trackingIdx = -1;
	
	m_state = State::sSlideIn;
	m_aniProgress = 0.f;
	m_lastTick = TickCount();
	BeginAnimation();
}

void PopupMenu::Hide()
{
	m_state = State::sHide;
	m_bHiding = true;
	if (GetParent()) {
		GetParent()->RemoveChild(this);
		// Invalidate(); // because GetWindow() will return null, this does not work.
	}
	if (m_parent) {
		m_parent->SetFocus();
	}
	if (m_menuBar) {
		m_menuBar->OnMenuHide();
	}
	m_bHiding = false;
}

void PopupMenu::HideAll()
{
	m_state = State::sHide;
	m_bHiding = true;
	if (GetParent()) {
		GetParent()->RemoveChild(this);
		// Invalidate(); // because GetWindow() will return null, this does not work.
	}
	if (m_parent) {
		m_parent->HideAll();
	}
	if (m_menuBar) {
		m_menuBar->OnMenuHide();
	}
	m_bHiding = false;
}

void PopupMenu::OnThemeChanged()
{
	auto sm = StyleManager::Instance();
	m_format = sm->GetTextFormat(this->m_szTextFormat);
	m_textColor = sm->GetColor(this->m_szTextColor);
	m_hoverColor = sm->GetColor(this->m_szHoverColor);
	m_background = sm->GetBackground(this->m_szBackground);

	for (auto item : m_vecItems) {
		if (item && item->sub_menu) {
			item->sub_menu->OnThemeChanged();
		}
	}
}

void PopupMenu::OnParentChanged(Widget* old, Widget* new_)
{
	m_hoverIdx = -1;
}

void PopupMenu::SetTextColor(LPCSTR style)
{
	this->m_szTextColor = ltk::InternString(style);
}

void PopupMenu::SetHoverColor(LPCSTR style)
{
	this->m_szHoverColor = ltk::InternString(style);
}

void PopupMenu::SetTextFormat(LPCSTR style)
{
	this->m_szTextFormat = ltk::InternString(style);
}

void PopupMenu::SetBackground(LPCSTR style)
{
	this->m_szBackground = ltk::InternString(style);
}

HRESULT PopupMenu::GetTextExtent(LPCWSTR str, IDWriteTextFormat *format, SizeF &size)
{
	IDWriteTextLayout *layout = nullptr;
	if (format->GetTextAlignment() != DWRITE_TEXT_ALIGNMENT_LEADING) 
		return E_INVALIDARG;
	if (format->GetParagraphAlignment() != DWRITE_PARAGRAPH_ALIGNMENT_NEAR)
		return E_INVALIDARG;

	HRESULT hr = GetDWriteFactory()->CreateTextLayout(
		str, wcslen(str), format, 9999, 9999, &layout);
	if (FAILED(hr))
		return hr;

	DWRITE_TEXT_METRICS dtm = { 0 };
	layout->GetMetrics(&dtm);
	size.Width = dtm.width;
	size.Height = dtm.height;

	return S_OK;
}

void PopupMenu::CalcWidth()
{
	SizeF size;
	float max_w = 10.f;
	float max_h = 10.f;
	for (auto item : m_vecItems) {
		if (item) {
			auto hr = GetTextExtent(item->text.c_str(), m_format, size);
			LTK_ASSERT(SUCCEEDED(hr));
			if (size.Width > max_w) {
				max_w = size.Width;
			}
			if (size.Height > max_h) {
				max_h = size.Height;
			}
		}
	}
	m_width = max_w + SHADOW_SIZE * 2 + PADDING_H * 2 + ICON_WIDTH;
	m_text_h = max_h;
}

bool PopupMenu::OnPaint(PaintEvent *ev)
{
	auto rcbg = this->GetClientRect();
	rcbg.Inflate(SHADOW_SIZE, SHADOW_SIZE);

	float slide_h = 0.f;
	if (m_state == State::sSlideIn) {
		m_aniProgress += (TickCount() - m_lastTick) * AniDelta;
		m_aniProgress = min(1.0f, m_aniProgress);
		slide_h = -this->GetHeight() + this->GetHeight() * m_aniProgress;
		ev->target->PushAxisAlignedClip(ltk::D2D1RectF(rcbg),
			D2D1_ANTIALIAS_MODE_ALIASED);
		ltk::TranslateTransform(ev->target, 0.f, slide_h);
	}

	m_background->Draw(GetWindow(), ev->target,
		rcbg, 
		AbstractBackground::Normal, 1.f);

	auto brush = GetWindow()->GetStockBrush();
	if (m_hoverIdx >= 0 && m_vecItems[m_hoverIdx]) {
		brush->SetColor(m_hoverColor);
		RectF rcHover = RectFromIndex(m_hoverIdx);
		ev->target->FillRectangle(ltk::D2D1RectF(rcHover), brush);
	}
	brush->SetColor(m_textColor);
	int idx = 0;
	for (auto item : m_vecItems) {
		if (item) {
			//ev->target->DrawText(item->text.c_str(), item->text.size(), m_format,
			//	D2D1::RectF(PADDING + ICON_WIDTH, y, this->GetWidth(), y + ITEM_HEIGHT),
			//	brush);
			auto rc = RectFromIndex(idx);
			rc.X += PADDING_H + ICON_WIDTH;
			rc.Width -= PADDING_H * 2 + ICON_WIDTH;
			rc.Y += PADDING_V;
			ev->target->DrawText(item->text.c_str(), item->text.size(), m_format,
				ltk::D2D1RectF(rc), brush);
		}
		else {
			auto rc = RectFromIndex(idx);
			auto old_clr = brush->GetColor();
			brush->SetColor(D2D1::ColorF(D2D1::ColorF::Gray));
			ev->target->DrawLine(D2D1::Point2F(rc.X + PADDING_H, rc.Y + rc.Height / 2.f),
				D2D1::Point2F(rc.Width - PADDING_H, rc.Y + rc.Height / 2.f), brush);
			brush->SetColor(old_clr);
		}
		idx++;
	}

	if (m_state == State::sSlideIn) {
		ltk::TranslateTransform(ev->target, 0.f, -slide_h);
		ev->target->PopAxisAlignedClip();
		m_lastTick = TickCount();
		//LTK_LOG("m_aniProgress: %.2f", m_aniProgress);
		if (m_aniProgress >= 1.f) {
			EndAnimation();
			m_state = State::sShow;
		}
	}
	return false;
}

bool PopupMenu::OnKillFocus(FocusEvent* ev)
{
	Invalidate();
	if (m_trackingIdx < 0 && !m_bHiding) {
		this->Hide();
		auto menu = m_parent;
		while (menu) {
			menu->Hide();
			menu = menu->m_parent;
		}
	}
	return false;
}

int PopupMenu::IndexFromPos(float y)
{
	float pos = SHADOW_SIZE;
	if (y < pos)
		return -1;
	int idx = 0;
	for (auto item : m_vecItems) {
		if (item) {
			pos += PADDING_V * 2 + m_text_h;
		}
		else {
			pos += SEPARATOR_HEIGHT;
		}
		if (y < pos) {
			return idx;
		}
		idx++;
	}
	return -1;
}

RectF PopupMenu::RectFromIndex(int idx)
{
	LTK_ASSERT(idx >= 0);
	RectF rc;
	rc.Y = SHADOW_SIZE;
	rc.X = SHADOW_SIZE;
	rc.Width = this->GetWidth() - SHADOW_SIZE * 2;

	for (int i = 0; i < idx; i ++) {
		auto item = m_vecItems[i];
		if (item) {
			rc.Y += PADDING_V * 2;
			rc.Y += m_text_h;
		}
		else {
			rc.Y += SEPARATOR_HEIGHT;
		}
	}
	auto item = m_vecItems[idx];

	if (item) {
		rc.Height = m_text_h + PADDING_V * 2;
	}
	else {
		rc.Height = SEPARATOR_HEIGHT;
	}
	return rc;
}

bool PopupMenu::OnLBtnDown(MouseEvent* ev)
{
	auto wnd = GetWindow();
	wnd->DisableFocusChange();
	int hit = IndexFromPos(ev->y);
	if (hit < 0 || hit >= (int)m_vecItems.size()) {
		return false;
	}
	if (hit == m_trackingIdx) {
		return false;
	}
	auto item = m_vecItems[hit];
	if (!item) {
		return true;
	}
	if (!item->sub_menu) {
		// TODO 这里不能传item因为要和button共用一个回调
		// 实际上应该弄个 IAction接口 C API拿到 Object之后 dynamic_cast到 IAction
		SetDelegateInvoker(wnd); 
		BOOL bhandled = FALSE;
		wnd->InvokeCallbacks<ActionCallback>(LTK_ACTION, item->GetName(), &bhandled);
		//item->ClickedDelegate();
	}
	int tracking = m_trackingIdx;
	PopupMenu* menu = this;
	while (tracking >= 0) {
		menu = menu->m_vecItems[tracking]->sub_menu;
		tracking = menu->m_trackingIdx;
	}
	menu->HideAll();
	::InvalidateRect(wnd->GetHWND(), NULL, FALSE);
	return true;
}

void PopupMenu::TrackPopupMenu(int idx)
{
	if (idx < 0 || !m_vecItems[idx])
		return;
	auto menu = m_vecItems[idx]->sub_menu;
	if (menu) {
		m_trackingIdx = idx;
		auto arc = this->GetAbsRect();
		auto mrc = this->RectFromIndex(idx);
		menu->CalcWidth();
		menu->Show(GetWindow(), RectF(
			arc.X + this->GetWidth(), arc.Y + mrc.Y, menu->GetWidth(), menu->GetHeight()));
		Invalidate();
	}
}

bool PopupMenu::OnMouseMove(MouseEvent* ev)
{
	TrackMouseLeave();
	int hover = IndexFromPos(ev->y);
	if (hover != m_hoverIdx) {
		m_hoverIdx = hover;
		Invalidate();
		
		//LTK_LOG("hover %d", hover);
		if (m_trackingIdx >= 0 && m_trackingIdx != hover) {
			auto sub_menu = m_vecItems[m_trackingIdx]->sub_menu;
			sub_menu->Hide();
			m_trackingIdx = -1;
			this->TrackPopupMenu(hover);
			this->Invalidate();
		} else if (m_trackingIdx < 0){
			this->TrackPopupMenu(hover);
		}
	}
	return true;
}

bool PopupMenu::OnMouseLeave(MouseEvent* ev)
{
	m_hoverIdx = -1;
	Invalidate();
	return false;
}



//////////////////////////////////////////////////////////////////////////

MenuBar::MenuBar()
{
}

MenuBar::~MenuBar()
{
	for (auto &item : m_vecMenuItems) {
		delete item.sub_menu;
	}
}


void MenuBar::AddItem(LPCWSTR text)
{
	Button *btn = new Button;
	btn->SetText(text);
	btn->SetBackground("menu_bar_btn_bg");
	//btn->ObjectName = "menu_btn";
	Widget::AddChild(btn);
	MenuButtonParam param;
	param.button = btn;
	m_vecMenuItems.push_back(param);
	btn->ClickedDelegate += MakeDelegate(this, &MenuBar::OnMenuBtnClicked);
	btn->MouseEventDelegate += MakeDelegate(this, &MenuBar::OnButtonMouseEvent);
}

void MenuBar::SetPopupMenu(UINT idx, PopupMenu *menu)
{
	LTK_ASSERT(m_vecMenuItems[idx].sub_menu == nullptr);
	menu->OnThemeChanged();
	m_vecMenuItems[idx].sub_menu = menu;
	menu->SetMenuBar(this);
}

void MenuBar::OnMenuBtnClicked()
{
	Button* btn = Object::GetDelegateInvoker()->As<Button>();
	int idx = FindMenuButtonIdx(btn);
	LTK_ASSERT(idx < (int)m_vecMenuItems.size());
	PopupMenu *menu = m_vecMenuItems[idx].sub_menu;
	if (!menu) {
		return;
	}
	auto arc = m_vecMenuItems[idx].button->GetAbsRect();

	menu->CalcWidth();
	menu->Show(this->GetWindow(), RectF(arc.X, arc.Y + arc.Height,
		m_vecMenuItems[idx].sub_menu->GetWidth(), menu->GetHeight()));
	m_trackingIdx = idx;
}


int MenuBar::FindMenuButtonIdx(Button *btn)
{
	int idx = -1;
	for (size_t i = 0; i < m_vecMenuItems.size(); i++) {
		if (m_vecMenuItems[i].button == btn) {
			idx = i;
			break;
		}
	}
	return idx;
}

void MenuBar::OnButtonMouseEvent(MouseEvent* ev, bool& bHandled)
{
	Button* btn = Object::GetDelegateInvoker()->As<Button>();

	if (ev->id == eMouseMove && m_trackingIdx >= 0) {
		int idx = FindMenuButtonIdx(btn);
		if (m_trackingIdx != (int)idx) {
			OnMenuBtnClicked();
		}
	}
}

void MenuBar::DoLayout()
{
	float x = 0;
	for (UINT i = 0; i < m_vecMenuItems.size(); i ++) {
		auto param = m_vecMenuItems[i];
		SizeF size = param.button->GetPreferredSize();
		RectF rc(x, 0.f, size.Width, this->GetHeight());
		//LTK_LOG("menu_btn: %.1f,%.1f,%.1f,%.1f", rc.X, rc.Y, rc.Width, rc.Height);
		param.button->SetRect(rc);
		x += size.Width;
	}
}

void MenuBar::OnMenuHide()
{
	m_trackingIdx = -1;
}

bool MenuBar::OnSize(SizeEvent *ev)
{
	this->DoLayout();
	return false;
}

void MenuBar::OnThemeChanged()
{
	for (auto &item : m_vecMenuItems) {
		if (item.sub_menu) {
			item.sub_menu->OnThemeChanged();
		}
	}
}

} // namespace

