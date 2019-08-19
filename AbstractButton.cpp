#include "stdafx.h"
#include "AbstractButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

AbstractButton::~AbstractButton()
{
    this->EndAnimation();
}

AbstractButton::State AbstractButton::GetState()
{
    return m_state;
}

void AbstractButton::EnableCapture(bool v)
{
    m_bCaptureMouse = v;
}

void AbstractButton::Update()
{
    DWORD timeDiff = ::GetTickCount() - m_lastTick;
    //LTK_LOG("timeDiff: %d", timeDiff);
    m_lastTick = ::GetTickCount();
    if (m_state == State::Normal2Hover) {
        m_aniCounter += timeDiff;
        if (m_aniCounter >= AniDuration) {
            m_aniCounter = AniDuration;
            m_state = State::Hover;
            this->EndAnimation();
        }
    } else if (m_state == State::Hover2Normal) {
        m_aniCounter -= timeDiff;
        if (m_aniCounter <= 0) {
            m_aniCounter = 0;
            m_state = State::Normal;
            this->EndAnimation();
        }
    } else {
        this->EndAnimation();
    }
}

float AbstractButton::GetBlend()
{
    return (float)m_aniCounter / AniDuration;
}

bool AbstractButton::OnEvent(Event *ev)
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

bool AbstractButton::OnMouseEnter(MouseEvent *ev)
{
    //LTK_LOG("OnMouseEnter");
    this->BeginAnimation();
    this->TrackMouseLeave();
    m_state = State::Normal2Hover;
    m_lastTick = ::GetTickCount();
    return true;
}

bool AbstractButton::OnMouseLeave(MouseEvent *ev)
{
    //LTK_LOG("OnMouseLeave");
    this->BeginAnimation();
    m_state = State::Hover2Normal;
    m_lastTick = ::GetTickCount();
    return true;
}

bool AbstractButton::OnLBtnDown(MouseEvent *ev)
{
    m_state = State::Pressed;
    if (m_bCaptureMouse) {
        this->SetCapture();
    }
    this->Invalidate();
    return true;
}

bool AbstractButton::OnLBtnUp(MouseEvent *ev)
{
    m_state = State::Normal;
    if (m_bCaptureMouse) {
        this->ReleaseCapture();
    }
    auto rc = this->GetClientRect();
    if (rc.Contains(Gdiplus::PointF(ev->x, ev->y))) {
        this->OnClicked();
    } else {
        this->OnMouseLeave(ev);
    }
    this->Invalidate();
    return true;
}

void AbstractButton::OnClicked()
{
    this->ClickedEvent.Invoke();
}


} // namespace ltk
