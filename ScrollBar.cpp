//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScrollBar.h"
#include "Button.h"
#include "StyleManager.h"
#include "Window.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

const float ScrollBar::SLIDER_MIN = 10.0f;

ScrollBar::ScrollBar(Orientation mode) : m_mode(mode)
{
    m_slider = new Button;
    m_slider->EnableCapture(false);
	m_slider->MouseEventDelegate += MakeDelegate(this, &ScrollBar::OnSilderEvent);

    if (m_mode == Horizontal) {
        m_slider->SetBackground("scrollbar_h");
    } else {
		m_slider->SetBackground("scrollbar_v");
    }
    this->AddChild(m_slider);
}

ScrollBar::~ScrollBar()
{
    SAFE_RELEASE(m_slider);
    RELEASE_AND_INVALIDATE(ID2D1SolidColorBrush, m_brush);
}

void ScrollBar::SetContentSize(float size)
{
    m_contentSize = size;
}

void ScrollBar::SetPosition(float pos)
{
    m_position = pos;
}

void ScrollBar::Update()
{
    auto rc = this->GetRect();

    float view_size;
    if (m_mode == Horizontal) {
        view_size = rc.Width;
    }
    else {
        view_size = rc.Height;
    }
    if (m_contentSize <= view_size) {
        m_slider->SetVisible(false);
        return;
    }
    else {
        m_slider->SetVisible(true);
    }
    float slider_size = view_size / m_contentSize * view_size;
    slider_size = max(SLIDER_MIN, slider_size);
    float slider_pos = m_position / (m_contentSize - view_size)
        * (view_size - slider_size);

    LtkRect rc2;
    if (m_mode == Horizontal) {
        rc2.x = slider_pos;
        rc2.y = 1;
        rc2.width = slider_size;
        rc2.height = rc.Height - 1;
        /*
        LTK_LOG("hsb: %.1f %.1f %.1f %.1f | %.1f %.1f %.1f %.1f",
            rc2.X, rc2.Y, rc2.Width, rc2.Height,
            rc.X, rc.Y, rc.Width, rc.Height);
        */
    }
    else {
        rc2.x = 1;
        rc2.y = slider_pos;
        rc2.width = rc.Width - 1;
        rc2.height = slider_size;
    }
    m_slider->SetRect(&rc2);
}

bool ScrollBar::OnPaint(PaintEvent *ev)
{
    if (m_mode != Horizontal) {
        //LTK_LOG("m_position %f m_mode: %d", m_position, m_mode);
    }
    if (!m_bDrag) {
        Update();
    }
    //DrawRectSnapped(ev->target, this->GetClientRect(), m_brush);
    return true;
}

bool ScrollBar::OnMouseMove(MouseEvent *ev)
{
    //LTK_LOG("OnMouseMove %f %f", ev->x, ev->y);
    if (m_bDrag) {
        float x = ev->x - m_deltaX;
        float y = ev->y - m_deltaY;
        auto rcSlider = m_slider->GetRect();
        auto rcRoot = this->GetRect();

        if (m_mode == Horizontal) {
            x = max(x, 0.0f);
            x = min(x, rcRoot.Width - rcSlider.Width);
            y = max(y, 0.0f);
            //LTK_LOG("m_slider->SetRect %f 1.0", x);
            m_slider->SetRect(x, 1.0f, rcSlider.Width, rcSlider.Height);
            m_position = x / (rcRoot.Width - rcSlider.Width) * (m_contentSize - rcRoot.Width);
        }
        else {
            x = max(x, 0.0f);
            y = max(y, 0.0f);
            y = min(y, rcRoot.Height - rcSlider.Height);
            //LTK_LOG("m_slider->SetRect 1.0 %f", y);
            m_slider->SetRect(1.0f, y, rcSlider.Width, rcSlider.Height);
            m_position = y / (rcRoot.Height - rcSlider.Height) * (m_contentSize - rcRoot.Height);
        }
        this->OnValueChanged(m_position);
    }
    return true;
}

bool ScrollBar::OnLBtnUp(MouseEvent *ev)
{
    LTK_LOG("OnLBtnUp");
    m_bDrag = false;
    this->ReleaseCapture();
    m_slider->OnLBtnUp(ev);
    this->Invalidate();
    return true;
}

bool ScrollBar::OnValueChanged(float pos)
{
	SetDelegateInvoker(this);
	this->ValueChangedDelegate(pos);
    return false;
}

void ScrollBar::RecreateResouce(ID2D1RenderTarget *target)
{
    HRESULT hr = E_FAIL;
    SAFE_RELEASE(m_brush);
    // TODO get color from StyleManager
    hr = target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkGray), &m_brush);
    assert(SUCCEEDED(hr));
}

void ScrollBar::OnSilderEvent(MouseEvent *ev, bool &bHandled)
{
    switch (ev->id) {
    case eLBtnDown:
        LTK_LOG("eLBtnDown %f %f", ev->x, ev->y);
        m_bDrag = true;
        m_deltaX = ev->x;
        m_deltaY = ev->y;
        LTK_ASSERT(::GetForegroundWindow() == this->GetWindow()->GetHWND());
        this->SetCapture();
        break;
    case eLBtnUp:

        break;
    case eMouseMove:
        //LTK_LOG("eMouseMove %f %f", ev->x, ev->y);
        break;
    }
    bHandled = false;
}

}