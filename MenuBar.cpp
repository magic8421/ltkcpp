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

namespace ltk {

const static float ITEM_HEIGHT = 30.f;
const static float MENU_WIDTH = 300.f;
const static float ICON_WIDTH = 16.f;
const static float PADDING = 5.f;

PopupMenu::PopupMenu() :
	TextColor("item_text_clr"),
	TextFormat("popup_menu_fmt"),
	HoverColor("menu_hover_clr"),
	Background("popup_menu_bg"),
	m_textColor(D2D1::ColorF(D2D1::ColorF::Cyan)),
	m_hoverColor(D2D1::ColorF(D2D1::ColorF::Cyan))
{
}

PopupMenu::~PopupMenu()
{
	ltk::KillTimer(m_hoverTimer);

	for (auto item : m_vecItems) {
		delete item->sub_menu;
		delete item;
	}
}

void PopupMenu::AddItem(LPCWSTR text)
{
	auto item = new MenuItem;
	item->text = text;
	m_vecItems.push_back(item);
}

UINT PopupMenu::GetChildCount()
{
	return m_vecItems.size();
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
	auto root = wnd->GetRootSprite();
	root->AddChild(this);
	this->SetRect(rc);
	wnd->SetFocusSprite(this);
	m_trackingIdx = -1;
	
	m_state = sSlideIn;
	m_aniProgress = 0.f;
	m_lastTick = TickCount();
	BeginAnimation();
}

void PopupMenu::Hide()
{
	m_state = sHide;
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
	m_state = sHide;
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
	m_format = sm->GetTextFormat(this->TextFormat);
	m_textColor = sm->GetColor(this->TextColor);
	m_hoverColor = sm->GetColor(this->HoverColor);
	m_background = sm->GetBackground(this->Background);

	for (auto item : m_vecItems) {
		if (item->sub_menu) {
			item->sub_menu->OnThemeChanged();
		}
	}
}

void PopupMenu::OnParentChanged(Sprite* old, Sprite* new_)
{
	m_hoverIdx = -1;
}

bool PopupMenu::OnPaint(PaintEvent *ev)
{
	float slide_h = 0.f;
	if (m_state == sSlideIn) {
		m_aniProgress += (TickCount() - m_lastTick) * AniDelta;
		m_aniProgress = min(1.0f, m_aniProgress);
		slide_h = -this->GetHeight() + this->GetHeight() * m_aniProgress;
		ev->target->PushAxisAlignedClip(ltk::D2D1RectF(GetClientRect()),
			D2D1_ANTIALIAS_MODE_ALIASED);
		ltk::TranslateTransform(ev->target, 0.f, slide_h);
	}

	m_background->Draw(GetWindow(), ev->target,
		this->GetClientRect(), 
		AbstractBackground::Normal, 1.f);
	float y = 0;
	auto brush = GetWindow()->GetStockBrush();
	if (m_hoverIdx >= 0) {
		brush->SetColor(m_hoverColor);
		ev->target->FillRectangle(ltk::D2D1RectF(RectF(0.f, m_hoverIdx * ITEM_HEIGHT,
			this->GetWidth(), ITEM_HEIGHT)), brush);
	}
	brush->SetColor(m_textColor);
	for (auto item : m_vecItems) {
		ev->target->DrawText(item->text.c_str(), item->text.size(), m_format,
			D2D1::RectF(PADDING + ICON_WIDTH, y, this->GetWidth(), y + ITEM_HEIGHT),
			brush);
		y += ITEM_HEIGHT;
	}

	if (m_state == sSlideIn) {
		ltk::TranslateTransform(ev->target, 0.f, -slide_h);
		ev->target->PopAxisAlignedClip();
		m_lastTick = TickCount();
		//LTK_LOG("m_aniProgress: %.2f", m_aniProgress);
		if (m_aniProgress >= 1.f) {
			EndAnimation();
			m_state = sShow;
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

bool PopupMenu::OnLBtnDown(MouseEvent* ev)
{
	auto wnd = GetWindow();
	wnd->DisableFocusChange();
	int hit = (int)(ev->y / ITEM_HEIGHT);
	if (hit < 0 || hit >= (int)m_vecItems.size()) {
		return false;
	}
	if (hit == m_trackingIdx) {
		return false;
	}
	auto item = m_vecItems[hit];
	if (!item->sub_menu) {
		item->ClickedEvent.Invoke();
	}
	int tracking = m_trackingIdx;
	PopupMenu* menu = this;
	while (tracking >= 0) {
		menu = menu->m_vecItems[tracking]->sub_menu;
		tracking = menu->m_trackingIdx;
	}
	menu->HideAll();
	::InvalidateRect(wnd->Handle(), NULL, FALSE);
	return true;
}

void PopupMenu::TrackPopupMenu(UINT idx)
{
	auto menu = m_vecItems[idx]->sub_menu;
	if (menu) {
		m_trackingIdx = idx;
		auto arc = this->GetAbsRect();
		menu->Show(GetWindow(), RectF(
			arc.X + this->GetWidth(), arc.Y + idx * ITEM_HEIGHT,
			menu->GetWidth(), menu->GetChildCount() * ITEM_HEIGHT));
		Invalidate();
	}
}

bool PopupMenu::OnMouseMove(MouseEvent* ev)
{
	TrackMouseLeave();
	int hover = (int)(ev->y / ITEM_HEIGHT);
	if (hover != m_hoverIdx) {
		m_hoverIdx = hover;
		Invalidate();
		m_hoverTimer = ltk::SetOnceTimer(0, m_hoverTimer, [this, hover]() {
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
			m_hoverTimer = 0;
		});
		if (m_parent) {
			ltk::KillTimer(m_parent->m_hoverTimer);
			m_parent->m_hoverTimer = 0;
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
	btn->Background = "menu_bar_btn_bg";
	//btn->ObjectName = "menu_btn";
	this->AddChild(btn);
	MenuButtonParam param;
	param.button = btn;
	m_vecMenuItems.push_back(param);
	UINT idx = m_vecMenuItems.size() - 1;
	btn->ClickedEvent.Attach([this, idx]() { // TODO  包btn比较好 插入删除不会错
		this->OnMenuBtnClicked(idx);
	});
	btn->DelegateMouseEvent.Attach([this, btn](MouseEvent *ev, bool &bHandled) {
		this->OnButtonMouseEvent(btn, ev, bHandled);
	});
}

void MenuBar::SetPopupMenu(UINT idx, PopupMenu *menu)
{
	LTK_ASSERT(m_vecMenuItems[idx].sub_menu == nullptr);
	menu->OnThemeChanged();
	m_vecMenuItems[idx].sub_menu = menu;
	menu->SetMenuBar(this);
}

void MenuBar::OnMenuBtnClicked(UINT idx)
{
	LTK_ASSERT(idx < m_vecMenuItems.size());
	auto menu = m_vecMenuItems[idx].sub_menu;
	if (!menu) {
		return;
	}
	auto arc = m_vecMenuItems[idx].button->GetAbsRect();

	menu->Show(GetWindow(), RectF(arc.X, arc.Y + arc.Height,
		m_vecMenuItems[idx].sub_menu->GetWidth(), menu->GetChildCount() * ITEM_HEIGHT));
	m_trackingIdx = idx;
}

void MenuBar::OnButtonMouseEvent(Button* btn, MouseEvent* ev, bool& bHandled)
{
	if (ev->id == eMouseMove && m_trackingIdx >= 0) {
		size_t idx = 0;
		for (; idx < m_vecMenuItems.size(); idx++) {
			if (m_vecMenuItems[idx].button == btn) {
				break;
			}
		}
		if (m_trackingIdx != (int)idx) {
			OnMenuBtnClicked(idx);
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

