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

struct AbstractButtonPrivate;

class AbstractButton : public Sprite
{
public:
    AbstractButton();
    virtual ~AbstractButton();

    typedef AbstractBackground::State State;

    State GetState();
    void EnableCapture(bool);
    void Update();
    float GetBlend();

    Delegate<void()> ClickedEvent;
    Delegate<void(MouseEvent*, bool&)> DelegateMouseEvent;

    virtual bool OnEvent(Event *ev) override;

    virtual bool OnMouseMove(MouseEvent *ev) override;
    virtual bool OnMouseLeave(MouseEvent *ev) override;
    virtual bool OnLBtnDown(MouseEvent *ev) override;
    virtual bool OnLBtnUp(MouseEvent *ev) override;

    virtual void OnClicked();

protected:
	AbstractButton(AbstractButtonPrivate *pd);

private:
	LTK_DECLARE_PRIVATE(AbstractButton);
};



} // namespace ltk

