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

PopupMenu::PopupMenu() :
	TextColor("item_text_clr"),
	TextFormat("item_text_fmt")
	HoverColor("item_hover_clr"),
	Background("menu_bg")
{
}

PopupMenu::~PopupMenu()
{
	for (auto item : m_vecItems) {
		delete item;
	}
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



//////////////////////////////////////////////////////////////////////////

MenuBar::MenuBar()
{
}

MenuBar::~MenuBar()
{
}


void MenuBar::AddItem(LPCWSTR text)
{
	Button *btn = new Button;
	btn->SetText(text);
	btn->Background = "menu_bg";
	//btn->ObjectName = "menu_btn";
	this->AddChild(btn);
	MenuButtonParam param;
	param.button = btn;
	m_vecMenuItems.push_back(param);
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
	
}



} // namespace

