#pragma once

#include "Sprite.h"

namespace ltk {

class TextEdit : public Sprite
{
public:
    TextEdit();
    ~TextEdit();

    virtual bool OnPaint(PaintEvent *ev) override;
    virtual bool OnChar(KeyEvent *ev) override;
    virtual bool OnSize(SizeEvent *ev) override;
    virtual bool OnImeInput(ImeEvent *ev) override;
    virtual void RecreateResouce(ID2D1RenderTarget *target) override;

    void RecreateLayout();

private:
    int m_cursorPos = 0;
    wstring m_text;
    IDWriteTextFormat *m_format = nullptr;
    IDWriteTextLayout *m_layout = nullptr;
    ID2D1SolidColorBrush *m_brush = nullptr;
};

class TextRenderer : public IDWriteTextRenderer
{
public:
    STDOVERRIDEMETHODIMP DrawGlyphRun(_In_opt_ void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, DWRITE_MEASURING_MODE measuringMode, _In_ DWRITE_GLYPH_RUN const* glyphRun, _In_ DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription, _In_opt_ IUnknown* clientDrawingEffect);
    STDOVERRIDEMETHODIMP DrawUnderline(_In_opt_ void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, _In_ DWRITE_UNDERLINE const* underline, _In_opt_ IUnknown* clientDrawingEffect);
    STDOVERRIDEMETHODIMP DrawStrikethrough(_In_opt_ void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, _In_ DWRITE_STRIKETHROUGH const* strikethrough, _In_opt_ IUnknown* clientDrawingEffect);
    STDOVERRIDEMETHODIMP DrawInlineObject(_In_opt_ void* clientDrawingContext, FLOAT originX, FLOAT originY, _In_ IDWriteInlineObject* inlineObject, BOOL isSideways, BOOL isRightToLeft, _In_opt_ IUnknown* clientDrawingEffect);

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef(void);
    virtual ULONG STDMETHODCALLTYPE Release(void);

};

} // namespace ltk