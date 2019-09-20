#pragma once
#include "Object_p.h"

namespace ltk {

struct SpritePrivate : public ObjectPrivate
{
	SpritePrivate(Sprite *q) : q_ptr(q) {}
	virtual ~SpritePrivate() {}
	Sprite *q_ptr = nullptr;

	bool bVisible;
	bool bClipChildren; // TODO this one is useless

	RectF rect;
	Window *window = nullptr;

	std::vector<Sprite *> children;
	Sprite *parent = nullptr;
};

} // namespace ltk