#include "stdafx.h"
#include "TextEdit.h"
#include "StyleManager.h"

namespace ltk {


TextEdit::TextEdit()
{
    m_text = L"test... test ...";
    m_cursorPos = m_text.size();

    HRESULT hr = GetDWriteFactory()->CreateTextFormat(L"Î¢ÈíÑÅºÚ", NULL, DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 16.0f, L"zh-cn",
        &m_format);
    LTK_ASSERT(SUCCEEDED(hr));

    this->EnableFocus(true);
    this->EnableClipChildren(true);
}

TextEdit::~TextEdit()
{
    SAFE_RELEASE(m_format);
    SAFE_RELEASE(m_layout);
    SAFE_RELEASE(m_brush);
}

bool TextEdit::OnPaint(PaintEvent *ev)
{
    auto target = ev->target;
    auto rc = this->GetClientRect();
    rc.X = 0.5f;
    rc.Y = 0.5f;
    rc.Width -= 0.5f;
    rc.Height -= 0.5f;
    target->DrawRectangle(D2D1RectF(rc), m_brush);

    target->DrawTextLayout(D2D_POINT_2F{ 0.0f, 0.0f }, m_layout, m_brush);
    return false;
}

bool TextEdit::OnChar(KeyEvent *ev)
{
    wchar_t ch = (wchar_t)ev->keyCode;
    m_text.insert(m_text.begin() + m_cursorPos, ch);
    m_cursorPos++;
    this->RecreateLayout();
    return true;
}

bool TextEdit::OnImeInput(ImeEvent *ev)
{
    m_text.insert(m_cursorPos, ev->text);
    m_cursorPos += wcslen(ev->text);
    this->RecreateLayout();
    return true;
}

void TextEdit::RecreateLayout()
{
    SAFE_RELEASE(m_layout);
    auto rc = this->GetClientRect();
    HRESULT hr = GetDWriteFactory()->CreateTextLayout(
        m_text.c_str(), m_text.size(), m_format,
        rc.Width, rc.Height, &m_layout);
    LTK_ASSERT(SUCCEEDED(hr));

    this->Invalidate();
}

bool TextEdit::OnSize(SizeEvent *ev)
{
    this->RecreateLayout();
    return false;
}

void TextEdit::RecreateResouce(ID2D1RenderTarget *target)
{
    SAFE_RELEASE(m_brush);
    auto textColor = StyleManager::Instance()->GetColor(StyleManager::clrTextNormal);
    HRESULT hr = target->CreateSolidColorBrush(textColor, &m_brush);
    LTK_ASSERT(SUCCEEDED(hr));
}

//////////////////////////////////////////////////////////////////////////
// TextRenderer
//////////////////////////////////////////////////////////////////////////

STDOVERRIDEMETHODIMP TextRenderer::DrawGlyphRun(_In_opt_ void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, DWRITE_MEASURING_MODE measuringMode, _In_ DWRITE_GLYPH_RUN const* glyphRun, _In_ DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription, _In_opt_ IUnknown* clientDrawingEffect)
{
    throw std::logic_error("The method or operation is not implemented.");
}

STDOVERRIDEMETHODIMP TextRenderer::DrawUnderline(_In_opt_ void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, _In_ DWRITE_UNDERLINE const* underline, _In_opt_ IUnknown* clientDrawingEffect)
{
    throw std::logic_error("The method or operation is not implemented.");
}

STDOVERRIDEMETHODIMP TextRenderer::DrawStrikethrough(_In_opt_ void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, _In_ DWRITE_STRIKETHROUGH const* strikethrough, _In_opt_ IUnknown* clientDrawingEffect)
{
    throw std::logic_error("The method or operation is not implemented.");
}

STDOVERRIDEMETHODIMP TextRenderer::DrawInlineObject(_In_opt_ void* clientDrawingContext, FLOAT originX, FLOAT originY, _In_ IDWriteInlineObject* inlineObject, BOOL isSideways, BOOL isRightToLeft, _In_opt_ IUnknown* clientDrawingEffect)
{
    throw std::logic_error("The method or operation is not implemented.");
}

HRESULT STDMETHODCALLTYPE TextRenderer::QueryInterface(REFIID riid, void **ppvObject)
{
    throw std::logic_error("The method or operation is not implemented.");
}

ULONG STDMETHODCALLTYPE TextRenderer::AddRef(void)
{
    throw std::logic_error("The method or operation is not implemented.");
}

ULONG STDMETHODCALLTYPE TextRenderer::Release(void)
{
    throw std::logic_error("The method or operation is not implemented.");
}

} // namespace ltk