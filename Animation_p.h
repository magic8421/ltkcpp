#pragma once
#include "Object_p.h"

namespace ltk {

class ScrollAnimationPrivate : public ObjectPrivate
{
public:
	ScrollAnimationPrivate(ScrollAnimation *q);
	virtual ~ScrollAnimationPrivate();

	void Init();
	void OnNoInputTimer();

	float scroll = 0.0f;
	float velocity = 0.0f;
	DWORD lastTick = 0;
	ScrollAnimation::State state = ScrollAnimation::stStop;
	bool bInput = false;
	Timer *timer = nullptr;

	LTK_DECLARE_PUBLIC(ScrollAnimation);
};

} // namespace ltk

