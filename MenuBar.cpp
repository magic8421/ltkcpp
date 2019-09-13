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

void PopupMenu::Show(Window* wnd, const RectF& rc)
{
	if (!wnd) {
		return;
	}
	auto root = wnd->GetRootSprite();
	root->AddChild(this);
	this->SetRect(rc);
	wnd->SetFocusSprite(this);
	m_bTrackingPopup = false;
}

void PopupMenu::Hide()
{
	m_bHiding = true;
	if (GetParent()) {
		GetParent()->RemoveChild(this);
		// Invalidate(); // because GetWindow() will return null, this does not work.
	}
	if (m_parent) {
		m_parent->SetFocus();
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
	return false;
}

bool PopupMenu::OnKillFocus(FocusEvent* ev)
{
	if (!m_bTrackingPopup && !m_bHiding) {
		this->Hide();
		auto menu = m_parent;
		while (menu) {
			menu->Hide();
			menu = menu->m_parent;
		}
	}
	return false;
}

void PopupMenu::TrackPopupMenu(UINT idx)
{
	auto menu = m_vecItems[idx]->sub_menu;
	if (menu) {
		m_trackingIdx = idx;
		m_bTrackingPopup = true;
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
		m_hoverTimer = ltk::SetOnceTimer(400, m_hoverTimer, [this, hover]() {
			LTK_LOG("hover %d", hover);
			if (m_trackingIdx >= 0 && m_trackingIdx != hover) {
				auto sub_menu = m_vecItems[m_trackingIdx]->sub_menu;
				sub_menu->m_bTrackingPopup = true;
				sub_menu->Hide();
				m_trackingIdx = -1;
				m_bTrackingPopup = false;
				this->Invalidate();
			}
			this->TrackPopupMenu(hover);
			m_hoverTimer = 0;
		});
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
	btn->ClickedEvent.Attach([this, idx]() {
		this->OnMenuBtnClicked(idx);
	});
}

void MenuBar::SetPopupMenu(UINT idx, PopupMenu *menu)
{
	LTK_ASSERT(m_vecMenuItems[idx].sub_menu == nullptr);
	menu->OnThemeChanged();
	m_vecMenuItems[idx].sub_menu = menu;
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

