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

Button::Button()
{
    this->SetStyle("default_button");
}

Button::~Button()
{
}

void Button::SetStyle(LPCSTR name)
{
    SetStyleName(name);
    OnThemeChanged();
}

void Button::OnThemeChanged()
{
    m_style = StyleManager::Instance()->GetButtonStyle(GetStyleName());
    m_background = StyleManager::Instance()->GetBackground(m_style->BackgroundStyle.c_str());
    m_format = StyleManager::Instance()->GetTextFormat(m_style->TextFormat.c_str());    
}

void Button::SetBackgroundStyle(LPCSTR style)
{
    m_background = StyleManager::Instance()->GetBackground(style);
}

void Button::SetTextFormat(LPCSTR name)
{
    m_format = StyleManager::Instance()->GetTextFormat(name);
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
        brush->SetColor(m_style->TextColor);
        ev->target->DrawText(m_text.c_str(), m_text.size(), m_format, ltk::D2D1RectF(rc),
            brush);
    }
    return true;
}

void Button::SetText(LPCWSTR text)
{
    m_text = text;
    this->Invalidate();
}



} // namespace ltk

