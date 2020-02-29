//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Label.h"
#include "ltk.h"
#include "StyleManager.h"
#include "Window.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

Label::Label() :
    m_szTextColor("default_label_clr"),
    m_szTextFormat("default_label_fmt"),
    m_color({ 1.0f, 0.0f, 1.0f, 0.5f })
{}

Label::~Label()
{
}

bool Label::OnPaint(PaintEvent *ev)
{
    if (m_text.length() == 0) {
        return true;
    }
    RectF rc = this->GetClientRect();

    auto brush = this->GetWindow()->GetStockBrush();
    brush->SetColor(m_color);
    ev->target->DrawText(m_text.c_str(), (UINT32)m_text.length(), m_textFormat,
        ltk::D2D1RectF(rc), brush);
    return true;
}

LPCWSTR Label::GetText()
{
    return m_text.c_str();
}

void Label::SetText(LPCWSTR t)
{
    m_text = t;
    this->Invalidate();
}

void Label::SetTextFormat(LPCSTR style)
{
    this->m_szTextFormat = StyleManager::Instance()->InternString(style);
}

void Label::SetTextColor(LPCSTR style)
{
    this->m_szTextColor = StyleManager::Instance()->InternString(style);
}

void Label::OnThemeChanged()
{
	auto sm = StyleManager::Instance();
    m_textFormat = sm->GetTextFormat(m_szTextFormat);
	m_color = sm->GetColor(m_szTextColor);
}

} // namespace ltk
