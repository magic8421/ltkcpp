//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Animation.h"
#include "Animation_p.h"
#include "TimerManager.h"
#include "ltk.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk
{

const float ItemHeight = 35.0f;
const float ScrollVelocity = ItemHeight * 3 / 500.0f;

ScrollAnimation::ScrollAnimation() : Object(new ScrollAnimationPrivate(this))
{
}

ScrollAnimation::ScrollAnimation(ScrollAnimationPrivate *d) : Object(d)
{
}

ScrollAnimationPrivate::ScrollAnimationPrivate(ScrollAnimation *q) 
	: ObjectPrivate(q)
{
	auto d = this;
	d->timer = new Timer;
	d->timer->SetInterval(350);
	d->timer->AttatchTimeoutDelegate(MakeDelegate(d, &ScrollAnimationPrivate::OnNoInputTimer));
}

ScrollAnimationPrivate::~ScrollAnimationPrivate()
{
	auto d = this;
	delete d->timer;
}


ScrollAnimation::~ScrollAnimation()
{
}

void ScrollAnimationPrivate::OnNoInputTimer()
{
	//LTK_LOG("OnNoInputTimer");
	this->bInput = false;
}

// TODO enum Direction { Up, Down }
void ScrollAnimation::BeginScroll(float delta)
{
	LTK_D(ScrollAnimation);
	d->bInput = true;
	d->timer->StartOnce();
    State new_state = stStop;
    if (delta > 0.0f) {
        new_state = stScrollUp;
    } else {
        new_state = stScrollDown;
    }
	d->velocity += ScrollVelocity;
	if (d->velocity > ScrollVelocity * 6) {
		d->velocity = ScrollVelocity * 6;
    }
	if (d->state != stStop && new_state != d->state) {
		d->velocity = 0.0f;
    }
	d->state = new_state;
	d->lastTick = ltk::TickCount();
}

void ScrollAnimation::Stop()
{
	LTK_D(ScrollAnimation);
	d->velocity = 0.0f;
	d->state = stStop;
}

void ScrollAnimation::UpdateScroll(float height)
{
	LTK_D(ScrollAnimation);
	if (height < 0.0f) {
        return;
    }
    DWORD now = ltk::TickCount();
	if (d->state == stScrollUp) {
		d->scroll -= d->velocity * (now - d->lastTick);
	}
	else if (d->state == stScrollDown) {
		d->scroll += d->velocity * (now - d->lastTick);
    }
	if (d->bInput) {
		d->velocity -= ScrollVelocity / 500.0f * (now - d->lastTick);
	} else {
		if (d->velocity > ScrollVelocity * 3) {
			d->velocity = ScrollVelocity * 3;
		}
		d->velocity -= ScrollVelocity / 100.0f * (now - d->lastTick);
	}
	d->lastTick = now;
	if (d->scroll < 0.0f) {
		d->scroll = 0.0f;
		d->velocity = 0.0f;
		d->state = stStop;
        return;
	}
	else if (d->scroll > height) {
		d->scroll = height;// this->GetTotalHeight() - rcSprite.Height;
		d->velocity = 0.0f;
		d->state = stStop;
        return;
    }
	if (d->velocity < 0.0f) {
		d->velocity = 0.0f;
		d->state = stStop;
        return ;
    }
}

float ScrollAnimation::GetScroll()
{
	LTK_D(ScrollAnimation);
	return d->scroll;
}

void ScrollAnimation::SetScroll(float pos)
{
	LTK_D(ScrollAnimation);
	d->state = stStop;
	d->velocity = 0.0f;
	d->scroll = pos;
}

bool ScrollAnimation::IsRunning()
{
	LTK_D(ScrollAnimation);
	return d->state != stStop;
}

ltk::ScrollAnimation::State ScrollAnimation::GetState()
{
	LTK_D(ScrollAnimation);
	return d->state;
}

} // namespace ltk
