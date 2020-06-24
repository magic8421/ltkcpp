//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "Sprite.h"
#include "StyleManager.h"
#include "MulticastDelegate.h"
#include "Delegate2.h"

namespace ltk {

class LTK_CPP_API AbstractButton : public Widget
{
public:
	RTTI_DECLARATIONS(AbstractButton, Widget);

    AbstractButton() {}
    virtual ~AbstractButton();

    typedef AbstractBackground::State State;

    State GetState();
    void EnableCapture(bool);
    void Update();
    float GetBlend();

	MulticastDelegate0 ClickedDelegate;
	delegate2::MuticastDelegate<MouseEvent*, bool&> MouseEventDelegate;

    virtual bool OnEvent(Event *ev) override;

    virtual bool OnMouseMove(MouseEvent *ev) override;
    virtual bool OnMouseLeave(MouseEvent *ev) override;
    virtual bool OnLBtnDown(MouseEvent *ev) override;
    virtual bool OnLBtnUp(MouseEvent *ev) override;

    virtual void OnClicked();

private:
    bool m_bCaptureMouse = true;
	bool m_bMouseIn = false;

    static const int AniDuration = 200;

    State m_state = State::Normal;
    int m_aniCounter = 0;
    DWORD m_lastTick = 0;
};



} // namespace ltk

