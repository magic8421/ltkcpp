//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Button.h"
#include "Label.h"
#include "ltk.h"
#include "StyleManager.h"
#include "ImageSprite.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

Button::Button() : BoxLayout(BoxLayout::Horizontal)
{
    m_background = StyleManager::Instance()->GetBackground("default_button");
}

Button::~Button()
{
    this->EndAnimation();
}

void Button::SetBackgroundStyle(const char *style)
{
    m_background = StyleManager::Instance()->GetBackground(style);
}

bool Button::OnPaint(PaintEvent *ev)
{
    Update();
    //LTK_LOG("OnPaint target: 0x%08x", ev->target);

    Window *wnd = this->GetWindow();
    auto rc = this->GetClientRect();
    if (m_background) {
        float blend;
        switch (m_state) {
        case AbstractBackground::Normal2Hover:
        case AbstractBackground::Hover2Normal:
            blend = (float)m_aniCounter / AniDuration;
            break;
        default:
            blend = 1.0f;
        }
        m_background->Draw(wnd, ev->target, rc, m_state, blend);
    }
    return true;
}

void Button::Update()
{
    DWORD timeDiff = ::GetTickCount() - m_lastTick;
    //LTK_LOG("timeDiff: %d", timeDiff);
    m_lastTick = ::GetTickCount();
    if (m_state == State::Normal2Hover)
    {
        m_aniCounter += timeDiff;
        if (m_aniCounter >= AniDuration)
        {
            m_aniCounter = AniDuration;
            m_state = State::Hover;
            this->EndAnimation();
        }
    }
    else if (m_state == State::Hover2Normal)
    {
        m_aniCounter -= timeDiff;
        if (m_aniCounter <= 0)
        {
            m_aniCounter = 0;
            m_state = State::Normal;
            this->EndAnimation();
        }
    }
    else
    {
        this->EndAnimation();
    }
}

bool Button::OnEvent(Event *ev)
{
    bool bHandled = false;
    if (ev->id > eMouseFirst && ev->id < eMouseLast) {
        this->DelegateMouseEvent.Invoke(
            static_cast<MouseEvent *>(ev), std::ref(bHandled));
    }
    if (!bHandled) {
        return Sprite::OnEvent(ev);
    }
    return bHandled;
}

bool Button::OnMouseEnter(MouseEvent *ev)
{
    //LTK_LOG("OnMouseEnter");
    this->BeginAnimation();
    this->TrackMouseLeave();
    m_state = State::Normal2Hover;
    m_lastTick = ::GetTickCount();
    return true;
}

bool Button::OnMouseLeave(MouseEvent *ev)
{
    //LTK_LOG("OnMouseLeave");
    this->BeginAnimation();
    m_state = State::Hover2Normal;
    m_lastTick = ::GetTickCount();
    return true;
}

bool Button::OnLBtnDown(MouseEvent *ev)
{
    m_state = State::Pressed;
    if (m_bCaptureMouse) {
        this->SetCapture();
    }
    this->Invalidate();
    return true;
}

bool Button::OnLBtnUp(MouseEvent *ev)
{
    m_state = State::Normal;
    if (m_bCaptureMouse){
        this->ReleaseCapture();
    }
    auto rc = this->GetClientRect();
    if (rc.Contains(Gdiplus::PointF(ev->x, ev->y))) {
        this->OnClicked();
    }
    else {
        this->OnMouseLeave(ev);
    }
    this->Invalidate();
    return true;
}

void Button::RecreateResouce(ID2D1RenderTarget *target)
{
}

void Button::OnClicked()
{
    this->ClickedEvent.Invoke();
}

void Button::SetText(LPCWSTR text)
{
    if (!m_label) {
        m_label = new Label;
        this->AddLayoutItem(m_label, 0.0f, 1.0f);
    }
    m_label->SetText(text);
}

Label *Button::GetLabel()
{
    return m_label;
}

void Button::EnableCapture(bool v)
{
    m_bCaptureMouse = v;
}

void Button::SetIcon(const RectF &rc, float scale, UINT idx)
{
    if (!m_image) {
        m_image = new IconSprite();
        m_image->SetIcon(rc, scale, idx);
        this->InsertLayoutItem(0, m_image, rc.Width * scale, 0.0f);
    }
    m_image->SetIcon(rc, scale, idx);
    this->Invalidate();
}

} // namespace
