#pragma once
#include "Sprite_p.h"
#include "Delegate\MulticastDelegate.h"

namespace ltk {

class ScrollBarPrivate : public SpritePrivate
{
public:
	ScrollBarPrivate(ScrollBar *q, ScrollBar::Mode m);
	virtual ~ScrollBarPrivate() {}

	ScrollBar::Mode mode;
	Button *slider = nullptr;
	float contentSize = 100.0f;
	float position = 0.0f;
	bool bDrag = false;
	float deltaX = 0.0f;
	float deltaY = 0.0f;

	MulticastDelegate1<float> valueChangedDelegate;

	void Init();
	void OnSilderEvent(MouseEvent *ev, bool &bHandled);

	LTK_DECLARE_PUBLIC(ScrollBar);
};

} // namespace ltk

