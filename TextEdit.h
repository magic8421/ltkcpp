//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Sprite.h"
#include "Animation.h"

namespace ltk {

class ScrollBar;

class LTK_CPP_API TextEdit : public Sprite
{
public:
	RTTI_DECLARATIONS(TextEdit, Sprite);

    TextEdit();
    ~TextEdit();

    void SetText(LPCWSTR);
    LPCWSTR GetText();

protected:
    virtual bool OnChar(KeyEvent *ev) override;
    virtual bool OnSize(SizeEvent *ev) override;
    virtual bool OnPaint(PaintEvent *ev) override;
    virtual bool OnKeyDown(KeyEvent *ev) override;
    virtual bool OnLBtnUp(MouseEvent *ev) override;
    virtual bool OnImeInput(ImeEvent *ev) override;
    virtual bool OnLBtnDown(MouseEvent *ev) override;
    virtual bool OnMouseMove(MouseEvent *ev) override;
    virtual bool OnMouseWheel(MouseEvent *ev) override;
    virtual void RecreateResouce(ID2D1RenderTarget *target) override;

    void RecreateLayout();
    void UpdateCursor(bool bEnsureVisible);

private:
    void DeleteSelected();
    int HitTest(float x, float y);

private:
    int m_cursorPos = 0;
    int m_selection = -1;
    int m_prevSelection = -1;

    wstring m_text;
    IDWriteTextFormat *m_format = nullptr;
    IDWriteTextLayout *m_layout = nullptr;
    ID2D1SolidColorBrush *m_brushSelectedText = nullptr;

    ScrollBar *m_vsb = nullptr;
    ScrollAnimation m_scrollAni;
    bool m_isInside = true;
    bool m_bCapture = false;

    float m_padding = 10.0f;
};

} // namespace ltk