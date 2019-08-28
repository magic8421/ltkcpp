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
	Background("default_btn_bg"),
	TextFormat("default_btn_fmt"),
	TextColor("default_btn_clr")
{
}

Button::~Button()
{
}

void Button::OnThemeChanged()
{
	//if (GetName() && strcmp(GetName(), "min_btn") == 0) {
	//	LTK_ASSERT(false);
	//}
	auto sm = StyleManager::Instance();

	//if (this->ObjectName == "menu_btn") {
	//	LTK_ASSERT(false);
	//}
	m_background = sm->GetBackground(Background);
	m_format = sm->GetTextFormat(TextFormat);
	m_textColor = sm->GetColor(TextColor);
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
		ev->target->DrawTextLayout(D2D1::Point2F(), m_layout, brush);

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
	SAFE_RELEASE(m_layout);
	auto rc = this->GetClientRect();
	if (!m_format) {
		this->OnThemeChanged();
	}
	HRESULT hr = GetDWriteFactory()->CreateTextLayout(
		m_text.c_str(), m_text.size(), m_format,
		rc.Width, rc.Height, &m_layout);
	LTK_ASSERT(SUCCEEDED(hr));
}

SizeF Button::GetPreferredSize()
{
	if (!m_layout) {
		this->RecreateLayout();
	}
	DWRITE_TEXT_METRICS dtm = { 0 };
	m_layout->GetMetrics(&dtm);

	const float padding = 6;
	return SizeF(dtm.width + padding * 2, dtm.height + padding * 2);
}

bool Button::OnSize(SizeEvent *ev)
{
	SAFE_RELEASE(m_layout);
	return false;
}

} // namespace ltk

