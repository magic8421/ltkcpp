#pragma once
#include "Object_p.h"
#include "Window.h"
#include "WindowLayout.h"

namespace ltk {

struct WindowPrivate : public ObjectPrivate {
	WindowPrivate(Window *wnd) :
		shadowLeft(ShadowFrame::eLeft),
		shadowTop(ShadowFrame::eTop),
		shadowRight(ShadowFrame::eRight),
		shadowBottom(ShadowFrame::eBottom) {

		rectComposition.left = 0;
		rectComposition.top = 0;
		rectComposition.right = 5;
		rectComposition.bottom = 20;

		sprite = new WindowLayout;
		sprite->SetWindow(wnd);

		caretHeight = 20;
	}
	~WindowPrivate() {}

	HWND hwnd = NULL;

	ImeInput ime;
	RECT rectComposition;
	int caretHeight;

	WindowLayout *sprite = nullptr; // owner

	bool bEnableFocusChange = true;
	Sprite *spFocus = nullptr;
	Sprite *spCapture = nullptr;
	Sprite *spHover = nullptr;
	std::unordered_set<Sprite *> setTrackMouseLeave;
	std::unordered_set<Sprite *> setAnimation;

	ID2D1HwndRenderTarget *target = nullptr; // owner
	ID2D1SolidColorBrush *brush = nullptr; // owner

	ShadowFrame shadowLeft;
	ShadowFrame shadowTop;
	ShadowFrame shadowRight;
	ShadowFrame shadowBottom;

	ID2D1Bitmap *atlas = nullptr; // owner TODO share across multiple Window
	AbstractBackground *background = nullptr;
	std::string styleName;

	Delegate<void(bool &)> CloseDelegate;
};

} // namespace ltk
