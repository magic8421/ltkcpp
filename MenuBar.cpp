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

namespace ltk {

const static float ITEM_HEIGHT = 30.f;
const static float MENU_WIDTH = 300.f;

PopupMenu::PopupMenu() :
	TextColor("item_text_clr"),
	TextFormat("popup_menu_fmt"),
	HoverColor("item_hover_clr"),
	Background("popup_menu_bg"),
	m_textColor(D2D1::ColorF(D2D1::ColorF::Cyan))
{
}

PopupMenu::~PopupMenu()
{
	for (auto item : m_vecItems) {
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

void PopupMenu::OnThemeChanged()
{
	auto sm = StyleManager::Instance();
	m_format = sm->GetTextFormat(this->TextFormat);
	m_textColor = sm->GetColor(this->TextColor);
	m_background = sm->GetBackground(this->Background);
}

bool PopupMenu::OnPaint(PaintEvent *ev)
{
	m_background->Draw(GetWindow(), ev->target,
		this->GetClientRect(), 
		AbstractBackground::Normal, 1.f);
	float y = 0;
	auto brush = GetWindow()->GetStockBrush();
	brush->SetColor(m_textColor);
	for (auto item : m_vecItems) {
		ev->target->DrawText(item->text.c_str(), item->text.size(), m_format,
			D2D1::RectF(0.f, y, this->GetWidth(), y + ITEM_HEIGHT), brush);
		y += ITEM_HEIGHT;
	}
	return false;
}

bool PopupMenu::OnKillFocus(FocusEvent* ev)
{
	if (GetParent()) {
		GetParent()->RemoveChild(this);
		Invalidate();
	}
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
	auto root = GetWindow()->GetRootSprite();
	root->AddChild(menu);
	menu->SetRect(RectF(arc.X, arc.Y + arc.Height,
		m_vecMenuItems[idx].sub_menu->GetWidth(), menu->GetChildCount() * ITEM_HEIGHT));
	GetWindow()->SetFocusSprite(menu);
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

