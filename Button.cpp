//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ltk.h"
#include "StyleManager.h"
#include "Button.h"
#include "Button_p.h"
#include "Window.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

Button::Button() : AbstractButton(new ButtonPrivate(this))
{
}

Button::Button(ButtonPrivate *pd) : AbstractButton(pd)
{
}

Button::~Button()
{
}

ButtonPrivate::ButtonPrivate(Button *pq) :
	Background("default_btn_bg"),
	TextFormat("default_btn_fmt"),
	TextColor("default_btn_clr"),
	AbstractButtonPrivate(pq)
{
}

ButtonPrivate::~ButtonPrivate()
{
	SAFE_RELEASE(layout);
}

void Button::OnThemeChanged()
{
	LTK_PUBLIC_DQ(Button);
	SAFE_RELEASE(d->layout);
	auto sm = StyleManager::Instance();

	d->background = sm->GetBackground(d->Background);
	d->format = sm->GetTextFormat(d->TextFormat);
	d->textColor = sm->GetColor(d->TextColor);
}

bool Button::OnPaint(PaintEvent *ev)
{
	LTK_PUBLIC_DQ(Button);
	Update();
    //LTK_LOG("OnPaint target: 0x%08x", ev->target);

    Window *wnd = this->GetWindow();
    auto rc = this->GetClientRect();
    if (d->background) {
        float blend;
        switch (GetState()) {
        case AbstractBackground::Normal2Hover:
        case AbstractBackground::Hover2Normal:
            blend = GetBlend();
            break;
        default:
            blend = 1.0f;
        }
        d->background->Draw(wnd, ev->target, rc, GetState(), blend);
    }
    if (d->text.size() > 0) {
        auto brush = wnd->GetStockBrush();
        brush->SetColor(d->textColor);
        //ev->target->DrawText(d->text.c_str(), d->text.size(), d->format, ltk::D2D1RectF(rc),
        //    brush);
		if (!d->layout) {
			this->RecreateLayout();
		}
		DWRITE_TEXT_METRICS dtm = { 0 };
		d->layout->GetMetrics(&dtm);
		ev->target->DrawTextLayout(D2D1::Point2F(
			(GetWidth() - dtm.width) / 2.f, (GetHeight() - dtm.height) / 2.f),
			d->layout, brush);

		// test
		GetPreferredSize();
    }
    return true;
}

void Button::SetText(LPCWSTR text)
{
	LTK_CHECK_THREAD;
	LTK_PUBLIC_DQ(Button);
	d->text = text;
	SAFE_RELEASE(d->layout);
    this->Invalidate();
}

void Button::RecreateLayout()
{
	LTK_PUBLIC_DQ(Button);
	static int cnt = 0;
	//LTK_LOG("Button::RecreateLayout() %d", cnt++);

	SAFE_RELEASE(d->layout);
	auto rc = this->GetClientRect();
	if (!d->format) {
		this->OnThemeChanged();
	}
	LTK_ASSERT(d->format->GetTextAlignment() == DWRITE_TEXT_ALIGNMENT_LEADING);
	LTK_ASSERT(d->format->GetParagraphAlignment() == DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	HRESULT hr = GetDWriteFactory()->CreateTextLayout(
		d->text.c_str(), d->text.size(), d->format,
		999, 999, &d->layout);
	LTK_ASSERT(SUCCEEDED(hr));
}

SizeF Button::GetPreferredSize()
{
	LTK_CHECK_THREAD;
	LTK_PUBLIC_DQ(Button);
	if (!d->layout) {
		this->RecreateLayout();
	}
	DWRITE_TEXT_METRICS dtm = { 0 };
	d->layout->GetMetrics(&dtm);

	const float padding = 8;
	return SizeF(dtm.width + padding * 2, dtm.height + padding * 2);
}

bool Button::OnSize(SizeEvent *ev)
{
	//SAFE_RELEASE(d->layout);
	return false;
}

void Button::SetBackground(LPCSTR name)
{
	LTK_CHECK_THREAD;
	LTK_PUBLIC_DQ(Button);
	d->Background = name;
}

void Button::SetTextFormat(LPCSTR name)
{
	LTK_CHECK_THREAD;
	LTK_PUBLIC_DQ(Button);
	d->TextFormat = name;
}

void Button::SetTextColor(LPCSTR name)
{
	LTK_CHECK_THREAD;
	LTK_PUBLIC_DQ(Button);
	d->TextColor = name;
}

} // namespace ltk

