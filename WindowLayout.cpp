#include "stdafx.h"
#include "Button.h"
#include "Label.h"
#include "Window.h"
#include "WindowLayout.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk
{

WindowLayout::WindowLayout()
{
    m_minBtn = new Button;
    m_minBtn->SetBackgroundStyle("min_button");
    this->AddChild(m_minBtn);
    m_maxBtn = new Button;
    m_maxBtn->SetBackgroundStyle("max_button");
    this->AddChild(m_maxBtn);
    m_closeBtn = new Button;
    m_closeBtn->SetBackgroundStyle("close_button");
    this->AddChild(m_closeBtn);
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

void WindowLayout::DoLayout()
{
    float btn_w = 22; // TODO load from StyleManager
    float btn_h = 20;
    float caption_h = 35;
    float margin = 5;

    RectF rc = this->GetClientRect();
    m_closeBtn->SetRect(RectF(rc.Width - btn_w, 0, btn_w, btn_h));
    m_maxBtn->SetRect(RectF(rc.Width - btn_w * 2, 0, btn_w, btn_h));
    m_minBtn->SetRect(RectF(rc.Width - btn_w * 3, 0, btn_w, btn_h));

    if (m_client) {
        m_client->SetRect(RectF(margin, caption_h + margin,
            rc.Width - margin * 2, rc.Height - margin * 2 - caption_h));
    }
}

void WindowLayout::UpdateEventHandler()
{
    auto wnd = this->GetWindow();
    m_minTrack.Disconnect();
    m_minBtn->ClickedEvent.Attach([=]() {
        wnd->Minimize();
    });

    m_closeTrack.Disconnect();
    m_closeBtn->ClickedEvent.Attach([=]() {
        wnd->CloseWindow();
    });
}

bool WindowLayout::OnSize(SizeEvent *ev)
{
    this->DoLayout();
    return false;
}

} // namespace ltk
