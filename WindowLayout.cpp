//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Button.h"
#include "Label.h"
#include "Window.h"
#include "WindowLayout.h"
#include "StyleManager.h"
#include "MenuBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk
{

WindowLayout::WindowLayout()
{
    m_minBtn = new Button;
    m_minBtn->SetBackground("min_btn_bg");
    this->AddChild(m_minBtn);
    m_maxBtn = new Button;
	m_maxBtn->SetBackground("max_btn_bg");
    this->AddChild(m_maxBtn);
    m_closeBtn = new Button;
	m_closeBtn->SetBackground("close_btn_bg");
    this->AddChild(m_closeBtn);
    m_caption = new Label;
	m_caption->SetTextFormat("caption_fmt");
	m_caption->SetTextColor("caption_clr");
    this->AddChild(m_caption);
}

WindowLayout::~WindowLayout()
{
}

Sprite *WindowLayout::SetClientSprite(Sprite *sp)
{
    if (m_client) {
        this->RemoveChild(m_client);
    }
    auto old = m_client;
    m_client = sp;
    this->AddChild(sp);
    this->DoLayout();
    return old;
}

Sprite *WindowLayout::GetClientSprite()
{
    return m_client;
}

MenuBar *WindowLayout::SetMenuBar(MenuBar *menu)
{
	auto old = m_menu;
	if (old) {
		this->RemoveChild(old);
	}
	m_menu = menu;
	this->AddChild(m_menu);
	return old;
}

MenuBar *WindowLayout::GetMenuBar()
{
	return m_menu;
}

RectF WindowLayout::GetCaptionRect()
{
    return m_caption->GetAbsRect();
}

LPCWSTR WindowLayout::GetCaptionText()
{
    return m_caption->GetText();
}

void WindowLayout::SetCaptionText(LPCWSTR text)
{
    m_caption->SetText(text);
}

void WindowLayout::DoLayout()
{
    auto sm = StyleManager::Instance();
	float btn_w = 22;
	float btn_h = 20;
	float caption_h = 30;
    float margin = 5;
	float sys_btn_margin = 3;
	float menu_h = 30;

    RectF rc = this->GetClientRect();
    m_closeBtn->SetRect(RectF(
		rc.Width - btn_w - sys_btn_margin, sys_btn_margin, btn_w, btn_h));
    m_maxBtn->SetRect(RectF(
		rc.Width - btn_w * 2 - sys_btn_margin, sys_btn_margin, btn_w, btn_h));
    m_minBtn->SetRect(RectF(
		rc.Width - btn_w * 3 - sys_btn_margin, sys_btn_margin, btn_w, btn_h));

    m_caption->SetRect(RectF(margin, margin, 
        rc.Width - btn_w * 3 - margin * 3, caption_h));

	if (m_menu) {
		m_menu->SetRect(RectF(margin, caption_h,
			rc.Width - margin * 2, menu_h));
		if (m_client) {
			m_client->SetRect(RectF(margin, caption_h + menu_h + margin,
				rc.Width - margin * 2, rc.Height - margin * 2 - caption_h - menu_h));
		}
	} else {
		if (m_client) {
			m_client->SetRect(RectF(margin, caption_h + margin,
				rc.Width - margin * 2, rc.Height - margin * 2 - caption_h));
		}
	}

}

void WindowLayout::SetWindow( Window *wnd )
{
    Sprite::SetWindow(wnd);

    m_minBtn->RemoveClickedDelegate(m_minTrack);
    m_minTrack = m_minBtn->AttachClickedDelegate([=]() {
        wnd->Minimize();
    });

	m_maxBtn->RemoveClickedDelegate(m_maxTrack);
	m_maxTrack = m_maxBtn->AttachClickedDelegate([=]() {
        wnd->Maximize();
    });

	m_closeBtn->RemoveClickedDelegate(m_closeTrack);
	m_closeTrack = m_closeBtn->AttachClickedDelegate([=]() {
        wnd->CloseWindow();
    });
}

bool WindowLayout::OnSize(SizeEvent *ev)
{
    this->DoLayout();
    return false;
}

} // namespace ltk
