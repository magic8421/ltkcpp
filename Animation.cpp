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
}

void ScrollAnimation::BeginScroll(float delta)
{
	//if (fabs(fabs(delta) - 1.f) > 0.001) {
	//	m_scroll -= delta * 120.f;
	//	m_scroll = max(0.f, m_scroll);
	//	return;
	//}

    State new_state = stStop;
    if (delta > 0.0f) {
        new_state = stScrollUp;
    } else {
        new_state = stScrollDown;
    }

    if (new_state != m_state) {
        m_target = m_scroll;
    }
    m_target = m_target + -delta * ItemHeight;

    m_state = new_state;
    m_lastTick = ltk::TickCount();
}

void ScrollAnimation::Stop()
{
    m_target = m_scroll;
    m_state = stStop;
}

bool ScrollAnimation::UpdateScroll(float height)
{
    if (height < 0.0f) {
        return false;
    }
    if (m_state == stStop) {
        return false;
    }
    DWORD now = ltk::TickCount();
    if (now - m_lastTick > AniDuration) {
        m_state = stStop;
        m_target = m_scroll;
        return true;
    }
    m_scroll += (m_target - m_scroll) * (now - m_lastTick) / AniDuration;

    if (m_scroll < 0.0f) {
        m_scroll = 0.0f;
        m_target = m_scroll;
        m_state = stStop;
        return true;
    } else if (m_scroll > height) {
        m_scroll = height;// this->GetTotalHeight() - rcWidget.Height;
        m_target = m_scroll;
		m_state = stStop;
        return true;
    }
    return false;
}

void ScrollAnimation::SetScroll(float pos)
{
    m_state = stStop;
    m_scroll = pos;
    m_target = m_scroll;
}

bool ScrollAnimation::IsRunning()
{
    return m_state != stStop;
}

} // namespace ltk
