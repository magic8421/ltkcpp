#pragma once
#include "Sprite.h"

namespace ltk {

class Label : public Sprite
{
public:
    Label();
    virtual ~Label();
    virtual bool OnPaint(PaintEvent *ev);
    virtual void RecreateResouce(ID2D1RenderTarget *target);

    LPCWSTR GetText();
    void SetText(LPCWSTR text);

    void SetTextAlign(DWRITE_TEXT_ALIGNMENT);
    void SetParagraphAlign(DWRITE_PARAGRAPH_ALIGNMENT);
    void SetTextColor(D2D1_COLOR_F);
    void SetFontSize(float size);
    void SetFontWeight(DWRITE_FONT_WEIGHT);
    void SetFontName(const wchar_t *name);

private:
    ID2D1SolidColorBrush *m_brush = nullptr; // owner
    IDWriteTextFormat *m_textFormat = nullptr; // owner
    std::wstring m_text;
    DWRITE_TEXT_ALIGNMENT m_textAlign = DWRITE_TEXT_ALIGNMENT_CENTER;
    DWRITE_PARAGRAPH_ALIGNMENT m_paragraphAlign = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
    D2D1_COLOR_F m_textColor;
    float m_fontSize = 12.0f;
    std::wstring m_fontName;
    DWRITE_FONT_WEIGHT m_fontWeight = DWRITE_FONT_WEIGHT_REGULAR;
};

} // namespace ltk
