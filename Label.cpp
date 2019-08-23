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

Label::Label()
{
    m_style = StyleManager::Instance()->LabelStyleMap.Get("default");
}

Label::~Label()
{
}

bool Label::OnPaint(PaintEvent *ev)
{
    if (m_text.length() == 0) {
        return true;
    }
    HRESULT hr = E_FAIL;
    RectF rc = this->GetClientRect();

    auto brush = this->GetWindow()->GetStockBrush();
    brush->SetColor(m_style->TextColor);
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

void Label::OnThemeChanged()
{
    m_style = StyleManager::Instance()->LabelStyleMap.Get(this->GetStyleName());
    m_textFormat = StyleManager::Instance()->GetTextFormat(m_style->TextFormat.c_str());
}

} // namespace ltk
