//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "Sprite.h"
#include "StyleManager.h"
#include "Delegate.h"

namespace ltk {

class AbstractButtonPrivate;

class LTK_API AbstractButton : public Sprite
{
public:
    AbstractButton();
    virtual ~AbstractButton();

    typedef AbstractBackground::State State;

	void EnableCapture(bool);

	Cookie AttachClickedDelegate(const std::function<void()> &cb);
	void RemoveClickedDelegate(Cookie c);

	Cookie AttachMouseEventDelegate(const std::function<void(MouseEvent*, bool&)> &cb);
	void RemoveMouseEventDelegate(Cookie c);

    virtual bool OnEvent(Event *ev) override;
    virtual bool OnMouseMove(MouseEvent *ev) override;
    virtual bool OnMouseLeave(MouseEvent *ev) override;
    virtual bool OnLBtnDown(MouseEvent *ev) override;
    virtual bool OnLBtnUp(MouseEvent *ev) override;
    virtual void OnClicked();

protected:
	AbstractButton(AbstractButtonPrivate *pd);

	State GetState();
	void Update();
	float GetBlend();

private:
	LTK_DECLARE_PRIVATE(AbstractButton);
};



} // namespace ltk

