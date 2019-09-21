#pragma once
#include "AbstractButton.h"
#include "Sprite_p.h"
#include "Delegate.h"

namespace ltk {

class AbstractButtonPrivate : public SpritePrivate
{
public:
	AbstractButtonPrivate(AbstractButton *q) : q_ptr(q), SpritePrivate(q) {}
	virtual ~AbstractButtonPrivate() {}

	AbstractButton *q_ptr = nullptr;

	bool bCaptureMouse = true;
	bool bMouseIn = false;

	AbstractBackground::State state = AbstractBackground::Normal;
	int aniCounter = 0;
	DWORD lastTick = 0;

	Delegate<void()> clickedDelegate;
	Delegate<void(MouseEvent*, bool&)> mouseEventDelegate;
};

}

