//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScrollBar.h"
#include "ScrollBar_p.h"
#include "Button.h"
#include "StyleManager.h"
#include "Window.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

const float ScrollBar::SLIDER_MIN = 10.0f;

ScrollBar::ScrollBar(Mode mode) : Sprite(new ScrollBarPrivate(this, mode))
{
	LTK_D(ScrollBar);
	d->Init();
}

ScrollBar::ScrollBar(ScrollBarPrivate *d) : Sprite(d)
{
	d->Init();
}


void ScrollBarPrivate::Init()
{
	LTK_Q(ScrollBar);
	auto d = this;
	d->slider = new Button;
	d->slider->EnableCapture(false);
	d->slider->AttachMouseEventDelegate(MakeDelegate(
		d, &ScrollBarPrivate::OnSilderEvent));

	if (mode == Horizontal) {
		d->slider->SetBackground("scrollbar_h");
	}
	else {
		d->slider->SetBackground("scrollbar_v");
	}
	q->AddChild(d->slider);
}

ScrollBarPrivate::ScrollBarPrivate(ScrollBar *q, ScrollBar::Mode m) : 
	SpritePrivate(q),
	mode(m)
{
}

ScrollBar::~ScrollBar()
{
}

float ScrollBar::GetValue()
{
	LTK_D(ScrollBar);
	return d->position;
}

void ScrollBar::SetContentSize(float size)
{
	LTK_D(ScrollBar);
	d->contentSize = size;
}

void ScrollBar::SetPosition(float pos)
{
	LTK_D(ScrollBar);
	d->position = pos;
}

void ScrollBar::Update()
{
	LTK_D(ScrollBar);
	auto rc = this->GetRect();

    float view_size;
	if (d->mode == Horizontal) {
        view_size = rc.Width;
    }
    else {
        view_size = rc.Height;
    }
	if (d->contentSize <= view_size) {
		d->slider->SetVisible(false);
        return;
    }
    else {
		d->slider->SetVisible(true);
    }
	float slider_size = view_size / d->contentSize * view_size;
    slider_size = max(SLIDER_MIN, slider_size);
	float slider_pos = d->position / (d->contentSize - view_size)
        * (view_size - slider_size);

    RectF rc2;
	if (d->mode == Horizontal) {
        rc2.X = slider_pos;
        rc2.Y = 1;
        rc2.Width = slider_size;
        rc2.Height = rc.Height - 1;
        /*
        LTK_LOG("hsb: %.1f %.1f %.1f %.1f | %.1f %.1f %.1f %.1f",
            rc2.X, rc2.Y, rc2.Width, rc2.Height,
            rc.X, rc.Y, rc.Width, rc.Height);
        */
    }
    else {
        rc2.X = 1;
        rc2.Y = slider_pos;
        rc2.Width = rc.Width - 1;
        rc2.Height = slider_size;
    }
	d->slider->SetRect(rc2);
}

bool ScrollBar::OnPaint(PaintEvent *ev)
{
	LTK_D(ScrollBar);
	if (d->mode != Horizontal) {
    }
	if (!d->bDrag) {
        Update();
    }
    return true;
}

bool ScrollBar::OnMouseMove(MouseEvent *ev)
{
	LTK_D(ScrollBar);
	//LTK_LOG("OnMouseMove %f %f", ev->x, ev->y);
	if (d->bDrag) {
		float x = ev->x - d->deltaX;
		float y = ev->y - d->deltaY;
		auto rcSlider = d->slider->GetRect();
        auto rcRoot = this->GetRect();

		if (d->mode == Horizontal) {
            x = max(x, 0.0f);
            x = min(x, rcRoot.Width - rcSlider.Width);
            y = max(y, 0.0f);
            //LTK_LOG("d->slider->SetRect %f 1.0", x);
			d->slider->SetRect(RectF(x, 1.0f, rcSlider.Width, rcSlider.Height));
			d->position = x / (rcRoot.Width - rcSlider.Width) * (d->contentSize - rcRoot.Width);
        }
        else {
            x = max(x, 0.0f);
            y = max(y, 0.0f);
            y = min(y, rcRoot.Height - rcSlider.Height);
            //LTK_LOG("d->slider->SetRect 1.0 %f", y);
			d->slider->SetRect(RectF(1.0f, y, rcSlider.Width, rcSlider.Height));
			d->position = y / (rcRoot.Height - rcSlider.Height) * (d->contentSize - rcRoot.Height);
        }
		this->OnValueChanged(d->position);
    }
    return true;
}

bool ScrollBar::OnLBtnUp(MouseEvent *ev)
{
	LTK_D(ScrollBar);
	LTK_LOG("OnLBtnUp");
	d->bDrag = false;
    this->ReleaseCapture();
	d->slider->OnLBtnUp(ev);
    this->Invalidate();
    return true;
}

bool ScrollBar::OnValueChanged(float pos)
{
	LTK_D(ScrollBar);
	this->ValueChangedEvent.Invoke(pos);
    return false;
}

void ScrollBar::OnRecreateResouce(ID2D1RenderTarget *target)
{
}

void ScrollBarPrivate::OnSilderEvent(MouseEvent *ev, bool &bHandled)
{
	LTK_Q(ScrollBar);
	switch (ev->id) {
    case eLBtnDown:
        LTK_LOG("eLBtnDown %f %f", ev->x, ev->y);
		this->bDrag = true;
		this->deltaX = ev->x;
		this->deltaY = ev->y;
        LTK_ASSERT(::GetForegroundWindow() == q->GetWindow()->Handle());
        q->SetCapture();
        break;
    case eLBtnUp:

        break;
    case eMouseMove:
        //LTK_LOG("eMouseMove %f %f", ev->x, ev->y);
		if (this->bDrag) {
            int i = 1;
            i = 1;
        }
        break;
    }
    bHandled = false;
}

}