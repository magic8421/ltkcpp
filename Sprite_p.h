#pragma once
#include "Object_p.h"
#include "ImmutableString.h"

namespace ltk {

class SpritePrivate : public ObjectPrivate
{
public:
	SpritePrivate(Sprite *q);
	virtual ~SpritePrivate();

	LTK_DECLARE_PUBLIC(Sprite);

	bool DispatchMouseEvent(MouseEvent *ev);
	void HandlePaint(ID2D1RenderTarget *target);
	void HandleKeyEvent(UINT message, DWORD keyCode, DWORD flag);
	void HandleImeInput(LPCTSTR text);
	void HandleRecreateResouce(ID2D1RenderTarget *target);
	void HandleThemeChange();

	bool bVisible;
	bool bClipChildren; // TODO this one is useless

	RectF rect;
	Window *window = nullptr;

	std::vector<Sprite *> children;
	Sprite *parent = nullptr;
	ImmutableString name;
};

} // namespace ltk