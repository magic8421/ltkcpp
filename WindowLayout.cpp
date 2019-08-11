#include "stdafx.h"
#include "Button.h"
#include "Label.h"
#include "Window.h"
#include "WindowLayout.h"
#include "StyleManager.h"

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
    m_caption = new Label;
    m_caption->SetTextAlign(DWRITE_TEXT_ALIGNMENT_LEADING);
    m_caption->SetParagraphAlign(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    m_caption->SetTextColor(
        StyleManager::Instance()->GetColor(StyleManager::clrTextCaption));
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
    float btn_w = sm->GetMeasurement(StyleManager::mSysButtonWidth);
    float btn_h = sm->GetMeasurement(StyleManager::mSysButtonHeight);
    float caption_h = sm->GetMeasurement(StyleManager::mCaptionHeight);
    float margin = 5;

    RectF rc = this->GetClientRect();
    m_closeBtn->SetRect(RectF(rc.Width - btn_w, 0, btn_w, btn_h));
    m_maxBtn->SetRect(RectF(rc.Width - btn_w * 2, 0, btn_w, btn_h));
    m_minBtn->SetRect(RectF(rc.Width - btn_w * 3, 0, btn_w, btn_h));

    m_caption->SetRect(RectF(margin, margin, 
        rc.Width - btn_w * 3 - margin * 3, caption_h));

    if (m_client) {
        m_client->SetRect(RectF(margin, caption_h + margin,
            rc.Width - margin * 2, rc.Height - margin * 2 - caption_h));
    }
}

void WindowLayout::SetWindow( Window *wnd )
{
    Sprite::SetWindow(wnd);

    m_minBtn->ClickedEvent.Remove(m_minTrack);
    m_minTrack = m_minBtn->ClickedEvent.Attach([=]() {
        wnd->Minimize();
    });

    m_closeBtn->ClickedEvent.Remove(m_closeTrack);
    m_closeTrack = m_closeBtn->ClickedEvent.Attach([=]() {
        wnd->CloseWindow();
    });
}

bool WindowLayout::OnSize(SizeEvent *ev)
{
    this->DoLayout();
    return false;
}

} // namespace ltk
