#pragma once

#include "Sprite.h"

namespace ltk {

class Button;

class ScrollBar : public Sprite
{
public:
    RTTI_DECLARATIONS(ScrollBar, Sprite)

    virtual bool OnEvent(Event *ev) override;

    enum Mode {
        Horizontal, Vertical
    };

    explicit ScrollBar(Mode mode);
    ~ScrollBar();

    float GetValue() { return m_position; }

    void SetContentSize(float size);

    void SetPosition(float pos);

    void Update();

    void OnSilderEvent(MouseEvent *ev, bool &bHandled);

    static const float SLIDER_MIN;

    virtual bool OnPaint(PaintEvent *ev) override;
    virtual bool OnMouseMove(MouseEvent *ev) override;

    virtual bool OnLBtnUp(MouseEvent *ev) override;

    virtual bool OnThumbDragging(float pos);

    virtual void RecreateResouce(ID2D1RenderTarget *target);

private:
    Mode m_mode;
    Button *m_slider = nullptr;
    float m_contentSize = 100.0f;
    float m_position = 0.0f;
    bool m_bDrag = false;
    float m_deltaX = 0.0f;
    float m_deltaY = 0.0f;
    ID2D1SolidColorBrush *m_brush = nullptr;
};

}