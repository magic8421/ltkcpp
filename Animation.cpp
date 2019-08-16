//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Animation.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk
{

void ScrollAnimation::BeginScroll(float delta)
{
    State new_state = stStop;
    if (delta > 0.0f) {
        new_state = stScrollUp;
    } else {
        new_state = stScrollDown;
    }
    m_velocity += ScrollVelocity;
    if (m_velocity > ScrollVelocity * 5) {
        m_velocity = ScrollVelocity * 5;
    }
    if (m_state != stStop && new_state != m_state) {
        m_velocity = 0.0f;
    }
    m_state = new_state;
    m_lastTick = ::GetTickCount();
}

void ScrollAnimation::Stop()
{
    m_velocity = 0.0f;
    m_state = stStop;
}

bool ScrollAnimation::UpdateScroll(float height)
{
    if (height < 0.0f) {
        return false;
    }
    DWORD now = ::GetTickCount();
    if (m_state == stScrollUp) {
        m_scroll -= m_velocity * (now - m_lastTick);
    } else if (m_state == stScrollDown) {
        m_scroll += m_velocity * (now - m_lastTick);
    }
    m_velocity -= ScrollVelocity / 500.0f * (now - m_lastTick);
    m_lastTick = now;
    if (m_scroll < 0.0f) {
        m_scroll = 0.0f;
        m_velocity = 0.0f;
        m_state = stStop;
        return true;
    } else if (m_scroll > height) {
        m_scroll = height;// this->GetTotalHeight() - rcSprite.Height;
        m_state = stStop;
        return true;
    }
    if (m_velocity < 0.0f) {
        m_velocity = 0.0f;
        m_state = stStop;
        return true;
    }
    return false;
}

void ScrollAnimation::SetScroll(float pos)
{
    m_state = stStop;
    m_velocity = 0.0f;
    m_scroll = pos;
}

bool ScrollAnimation::IsRunning()
{
    return m_state != stStop;
}

} // namespace ltk
