#include "stdafx.h"
#include "Label.h"
#include "ltk.h"
#include "StyleManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

Label::Label()
{
    m_textColor = StyleManager::Instance()->GetColor(StyleManager::clrTextNormal);
}

Label::~Label()
{
    if (m_brush) {
        m_brush->Release();
    }
    m_brush = INVALID_POINTER(ID2D1SolidColorBrush);

    if (m_textFormat) {
        m_textFormat->Release();
    }
    m_textFormat = INVALID_POINTER(IDWriteTextFormat);
}

bool Label::OnPaint(PaintEvent *ev)
{
    if (m_text.length() == 0) {
        return true;
    }
    HRESULT hr = E_FAIL;
    RectF rc = this->GetClientRect();
    if (!m_textFormat) {
        hr = GetDWriteFactory()->CreateTextFormat(
            m_fontName.c_str(),
            NULL,
            m_fontWeight,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            m_fontSize,
            L"zh-cn",
            &m_textFormat
            );
        assert(SUCCEEDED(hr));
        LTK_LOG("recreate m_textFormat");
    }
    m_textFormat->SetTextAlignment(m_textAlign);
    m_textFormat->SetParagraphAlignment(m_paragraphAlign);
    m_brush->SetColor(m_textColor);
    ev->target->DrawText(m_text.c_str(), (UINT32)m_text.length(), m_textFormat, ltk::D2D1RectF(rc), m_brush);
    return true;
}

void Label::RecreateResouce(ID2D1RenderTarget *target)
{
    LTK_LOG("RecreateResouce");
    HRESULT hr = E_FAIL;
    SAFE_RELEASE(m_brush);
    hr = target->CreateSolidColorBrush(m_textColor, &m_brush);
    assert(SUCCEEDED(hr));
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

void Label::SetTextAlign(DWRITE_TEXT_ALIGNMENT a)
{
    m_textAlign = a;
    this->Invalidate();
}

void Label::SetParagraphAlign(DWRITE_PARAGRAPH_ALIGNMENT a)
{
    m_paragraphAlign = a;
    this->Invalidate();
}

void Label::SetTextColor(D2D1_COLOR_F clr)
{
    m_textColor = clr;
    this->Invalidate();
}

void Label::SetFontSize(float size)
{
    m_fontSize = size;
    SAFE_RELEASE(m_textFormat);
    this->Invalidate();
}

void Label::SetFontWeight(DWRITE_FONT_WEIGHT weight)
{
    m_fontWeight = weight;
    SAFE_RELEASE(m_textFormat);
    this->Invalidate();
}

void Label::SetFontName(const wchar_t *name)
{
    m_fontName = name;
    SAFE_RELEASE(m_textFormat);
    this->Invalidate();
}

} // namespace ltk
