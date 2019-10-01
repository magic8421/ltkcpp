//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "Object.h"

namespace ltk {

class Timer;
class ScrollAnimationPrivate;

class ScrollAnimation : public Object
{
public:
    ScrollAnimation();
    ~ScrollAnimation();

    enum State {
        stStop, stScrollUp, stScrollDown
    };

    void BeginScroll(float delta);
    void Stop();
    void UpdateScroll(float height);
    float GetScroll();
    void SetScroll(float pos);
    State GetState();

    bool IsRunning();


protected:
	ScrollAnimation(ScrollAnimationPrivate *d);

private:
	LTK_DECLARE_PRIVATE(ScrollAnimation);
};

} // namespace ltk
