//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "Sprite.h"
#include "StyleManager.h"
#include "Delegate.h"

namespace ltk {

class AbstractButton : public Sprite
{
public:
    AbstractButton() {}
    virtual ~AbstractButton();

    typedef AbstractBackground::State State;

    State GetState();
    void EnableCapture(bool);
    void Update();
    float GetBlend();

    Delegate<void()> ClickedEvent;
    Delegate<void(MouseEvent*, bool&)> DelegateMouseEvent;

    virtual bool OnEvent(Event *ev) override;

    virtual bool OnMouseEnter(MouseEvent *ev) override;
    virtual bool OnMouseLeave(MouseEvent *ev) override;
    virtual bool OnLBtnDown(MouseEvent *ev) override;
    virtual bool OnLBtnUp(MouseEvent *ev) override;

    virtual void OnClicked();

private:
    bool m_bCaptureMouse = true;

    static const int AniDuration = 200;

    State m_state = State::Normal;
    int m_aniCounter = 0;
    DWORD m_lastTick = 0;
};



} // namespace ltk

