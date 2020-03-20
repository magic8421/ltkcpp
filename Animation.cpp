//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Animation.h"
#include "TimerManager.h"
#include "ltk.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk
{

ScrollAnimation::ScrollAnimation()
{
	m_timer.SetInterval(350);
	m_timer.TimeoutDelegate += MakeDelegate(this, &ScrollAnimation::OnNoInputTimer);
}

void ScrollAnimation::OnNoInputTimer()
{
	m_bInput = false;
}

void ScrollAnimation::BeginScroll(float delta)
{
	if (fabs(fabs(delta) - 1.f) > 0.001) {
		m_scroll -= delta * 120.f;
		m_scroll = max(0.f, m_scroll);
		return;
	}

	m_bInput = true;
	
	m_timer.StartOnce();

    State new_state = stStop;
    if (delta > 0.0f) {
        new_state = stScrollUp;
    } else {
        new_state = stScrollDown;
    }
    m_velocity += ScrollVelocity;
    if (m_velocity > ScrollVelocity * 6) {
        m_velocity = ScrollVelocity * 6;
    }
    if (m_state != stStop && new_state != m_state) {
        m_velocity = 0.0f;
    }
    m_state = new_state;
    m_lastTick = ltk::TickCount();
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
    DWORD now = ltk::TickCount();
    if (m_state == stScrollUp) {
        m_scroll -= m_velocity * (now - m_lastTick);
    } else if (m_state == stScrollDown) {
        m_scroll += m_velocity * (now - m_lastTick);
    }
	if (m_bInput) {
		m_velocity -= ScrollVelocity / 500.0f * (now - m_lastTick);
	} else {
		if (m_velocity > ScrollVelocity * 3) {
			m_velocity = ScrollVelocity * 3;
		}
		m_velocity -= ScrollVelocity / 100.0f * (now - m_lastTick);
	}
    m_lastTick = now;
    if (m_scroll < 0.0f) {
        m_scroll = 0.0f;
        m_velocity = 0.0f;
        m_state = stStop;
        return true;
    } else if (m_scroll > height) {
        m_scroll = height;// this->GetTotalHeight() - rcWidget.Height;
		m_velocity = 0.0f;
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
