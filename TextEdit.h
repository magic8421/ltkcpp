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
    virtual bool OnLBtnDown(MouseEvent *ev) override;
    virtual void RecreateResouce(ID2D1RenderTarget *target) override;

    void RecreateLayout();



private:
    int m_cursorPos = 0;
    wstring m_text;
    IDWriteTextFormat *m_format = nullptr;
    IDWriteTextLayout *m_layout = nullptr;
    ID2D1SolidColorBrush *m_brush = nullptr;
};

} // namespace ltk