#include "stdafx.h"
#include "AbstractButton.h"
#include "AbstractButton_p.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

static const int AniDuration = 200;

AbstractButton::AbstractButton() : Sprite(new AbstractButtonPrivate(this))
{
}

AbstractButton::AbstractButton(AbstractButtonPrivate *pd) : Sprite(pd)
{
}

AbstractButton::~AbstractButton()
{
    this->EndAnimation();
}

AbstractButton::State AbstractButton::GetState()
{
	LTK_D(AbstractButton);
    return d->state;
}

void AbstractButton::EnableCapture(bool v)
{
	LTK_D(AbstractButton);
	d->bCaptureMouse = v;
}

void AbstractButton::Update()
{
	LTK_D(AbstractButton);
	DWORD timeDiff = ltk::TickCount() - d->lastTick;
    //LTK_LOG("timeDiff: %d", timeDiff);
    d->lastTick = ltk::TickCount();
    if (d->state == State::Normal2Hover) {
        d->aniCounter += timeDiff;
        if (d->aniCounter >= AniDuration) {
            d->aniCounter = AniDuration;
            d->state = State::Hover;
            this->EndAnimation();
        }
    } else if (d->state == State::Hover2Normal) {
        d->aniCounter -= timeDiff;
        if (d->aniCounter <= 0) {
            d->aniCounter = 0;
            d->state = State::Normal;
            this->EndAnimation();
        }
    } else {
        this->EndAnimation();
    }
}

float AbstractButton::GetBlend()
{
	LTK_D(AbstractButton);
	return (float)d->aniCounter / AniDuration;
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

bool AbstractButton::OnMouseMove(MouseEvent *ev)
{
	LTK_D(AbstractButton);
	if (!d->bMouseIn) {
		//LTK_LOG("OnMouseMove");
		d->bMouseIn = true;
		this->BeginAnimation();
		this->TrackMouseLeave();
		d->state = State::Normal2Hover;
		d->lastTick = ltk::TickCount();
	}
    return true;
}

bool AbstractButton::OnMouseLeave(MouseEvent *ev)
{
	LTK_D(AbstractButton);
	//LTK_LOG("OnMouseLeave");
	d->bMouseIn = false;
    this->BeginAnimation();
    d->state = State::Hover2Normal;
    d->lastTick = ltk::TickCount();
    return true;
}

bool AbstractButton::OnLBtnDown(MouseEvent *ev)
{
	LTK_D(AbstractButton);
    d->state = State::Pressed;
    if (d->bCaptureMouse) {
        this->SetCapture();
    }
    this->Invalidate();
    return true;
}

bool AbstractButton::OnLBtnUp(MouseEvent *ev)
{
	LTK_D(AbstractButton);
	d->state = State::Hover;
    if (d->bCaptureMouse) {
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
