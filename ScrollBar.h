//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Sprite.h"
#include "Delegate.h"

namespace ltk {

class Button;
class ScrollBarPrivate;

class ScrollBar : public Sprite
{
public:
    enum Mode {
        Horizontal, Vertical
    };

    explicit ScrollBar(Mode mode);
    ~ScrollBar();

    float GetValue();

    void SetContentSize(float size);

    void SetPosition(float pos);

    void Update();

    static const float SLIDER_MIN;

    virtual bool OnPaint(PaintEvent *ev) override;
    virtual bool OnMouseMove(MouseEvent *ev) override;

    virtual bool OnLBtnUp(MouseEvent *ev) override;

    virtual bool OnValueChanged(float pos);

    virtual void OnRecreateResouce(ID2D1RenderTarget *target);

    Delegate<void(float)> ValueChangedEvent;

private:
	LTK_DECLARE_PRIVATE(ScrollBar);

protected:
	ScrollBar(ScrollBarPrivate *d);
};

}