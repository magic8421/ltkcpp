#pragma once
#include "AbstractButton.h"
#include "Sprite_p.h"

namespace ltk {

struct AbstractButtonPrivate : public SpritePrivate
{
	AbstractButtonPrivate(AbstractButton *q) : q_ptr(q), SpritePrivate(q) {}
	virtual ~AbstractButtonPrivate() {}

	AbstractButton *q_ptr;

	bool bCaptureMouse = true;
	bool bMouseIn = false;

	AbstractBackground::State state = AbstractBackground::Normal;
	int aniCounter = 0;
	DWORD lastTick = 0;
};

}

