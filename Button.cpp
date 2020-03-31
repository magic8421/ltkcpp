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
#include "Window.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

Button::Button() : 
	m_szBackground("default_btn_bg"),
	m_szTextFormat("default_btn_fmt"),
	m_szTextColor("default_btn_clr"),
	m_textColor({ 1.0f, 0.0f, 1.0f, 0.5f })
{
}

Button::~Button()
{
}

void Button::OnThemeChanged()
{
	SAFE_RELEASE(m_layout);
	auto sm = StyleManager::Instance();

	m_background = sm->GetBackground(m_szBackground);
	m_format = sm->GetTextFormat(m_szTextFormat);
	m_textColor = sm->GetColor(m_szTextColor);
}

bool Button::OnPaint(PaintEvent *ev)
{
    Update();
    //LTK_LOG("OnPaint target: 0x%08x", ev->target);

    Window *wnd = this->GetWindow();
    auto rc = this->GetClientRect();
    if (m_background) {
        float blend;
        switch (GetState()) {
        case AbstractBackground::Normal2Hover:
        case AbstractBackground::Hover2Normal:
            blend = GetBlend();
            break;
        default:
            blend = 1.0f;
        }
        m_background->Draw(wnd, ev->target, rc, GetState(), blend);
    }
    if (m_text.size() > 0) {
        auto brush = wnd->GetStockBrush();
        brush->SetColor(m_textColor);
        //ev->target->DrawText(m_text.c_str(), m_text.size(), m_format, ltk::D2D1RectF(rc),
        //    brush);
		if (!m_layout) {
			this->RecreateLayout();
		}
		DWRITE_TEXT_METRICS dtm = { 0 };
		m_layout->GetMetrics(&dtm);
		ev->target->DrawTextLayout(D2D1::Point2F(
			(GetWidth() - dtm.width) / 2.f, (GetHeight() - dtm.height) / 2.f),
			m_layout, brush);

		// test
		GetPreferredSize();
    }
    return true;
}

void Button::SetText(LPCWSTR text)
{
    m_text = text;
	SAFE_RELEASE(m_layout);
    this->Invalidate();
}

void Button::RecreateLayout()
{
	static int cnt = 0;
	LTK_LOG("Button::RecreateLayout() %d", cnt++);

	SAFE_RELEASE(m_layout);
	if (!m_format) {
		this->OnThemeChanged();
	}
	LTK_ASSERT(m_format->GetTextAlignment() == DWRITE_TEXT_ALIGNMENT_LEADING);
	LTK_ASSERT(m_format->GetParagraphAlignment() == DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	HRESULT hr = GetDWriteFactory()->CreateTextLayout(
		m_text.c_str(), m_text.size(), m_format,
		9999, 9999, &m_layout);
	LTK_ASSERT(SUCCEEDED(hr));
}

SizeF Button::GetPreferredSize()
{
	if (!m_layout) {
		this->RecreateLayout();
	}
	DWRITE_TEXT_METRICS dtm = { 0 };
	m_layout->GetMetrics(&dtm);

	const float padding = 8;
	return SizeF(dtm.width + padding * 2, dtm.height + padding * 2);
}

void Button::SetBackground(LPCSTR style)
{
	this->m_szBackground = ltk::InternString(style);
}

void Button::SetTextFormat(LPCSTR style)
{
	this->m_szTextFormat = ltk::InternString(style);
}

void Button::SetTextColor(LPCSTR style)
{
	this->m_szTextColor = ltk::InternString(style);
}

bool Button::OnSize(SizeEvent *ev)
{
	//SAFE_RELEASE(m_layout);
	return false;
}

} // namespace ltk

