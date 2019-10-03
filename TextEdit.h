//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Sprite.h"
#include "Animation.h"

namespace ltk {

class TextEditPrivate;

class TextEdit : public Sprite
{
public:
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
    virtual void OnRecreateResouce(ID2D1RenderTarget *target) override;

    void RecreateLayout();
    void UpdateCursor(bool bEnsureVisible);

private:
	LTK_DECLARE_PRIVATE(TextEdit);

protected:
	TextEdit(TextEditPrivate *d);
};

} // namespace ltk