#pragma once
#include "Object_p.h"
#include "Window.h"
#include "WindowLayout.h"
#include "ShadowFrame.h"
#include "ImeInput.h"

namespace ltk {

class AbstractBackground;
class WindowLayout;

class WindowPrivate : public ObjectPrivate {
public:
	WindowPrivate(Window *wnd);
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
