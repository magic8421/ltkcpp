#pragma once
#include "AbstractButton.h"
#include "Sprite_p.h"
#include "Delegate/MulticastDelegate.h"

namespace ltk {

class AbstractButtonPrivate : public SpritePrivate
{
public:
	AbstractButtonPrivate(AbstractButton *q) : SpritePrivate(q) {}
	virtual ~AbstractButtonPrivate() {}

	LTK_DECLARE_PUBLIC(AbstractButton);

private:
	bool bCaptureMouse = true;
	bool bMouseIn = false;

	AbstractBackground::State state = AbstractBackground::Normal;
	int aniCounter = 0;
	DWORD lastTick = 0;

	MulticastDelegate0 clickedDelegate;
	MulticastDelegate2<MouseEvent*, bool&> mouseEventDelegate;
};

}

