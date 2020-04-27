//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "TimerManager.h"

namespace ltk {

class LTK_CPP_API ScrollAnimation : public Object
{
public:
    ScrollAnimation();
    ~ScrollAnimation() {}

    enum State
    {
        stStop, stScrollUp, stScrollDown
    };

    void BeginScroll(float delta); // OnMouseWheel
    void Stop();
    // return true if you have to call EndAnimation();
    bool UpdateScroll(float height); // OnPaint
    //
    float GetScroll() { return m_scroll; }
    void SetScroll(float pos);
    State GetState() { return m_state; }

    bool IsRunning();

private:
    float m_scroll = 0.0f;
    float m_target = 0.0f;
    DWORD m_lastTick = 0;
    State m_state = stStop;

    const float ItemHeight = 80.0f;
    const float AniDuration = 700.0f;

};

} // namespace ltk
