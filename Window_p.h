#pragma once
#include "Object_p.h"
#include "Window.h"
#include "WindowLayout.h"
#include "ShadowFrame.h"
#include "ImeInput.h"
#include "Delegate/MulticastDelegate.h"

namespace ltk {

class AbstractBackground;
class WindowLayout;

class WindowPrivate : public ObjectPrivate {
public:
	WindowPrivate(Window *wnd);
	~WindowPrivate() {}

	LTK_DECLARE_PUBLIC(Window);

private:
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
	bool bShadow = true;


	ID2D1Bitmap *atlas = nullptr; // owner TODO share across multiple Window
	AbstractBackground *background = nullptr;
	std::string styleName;

	//Delegate<void(bool &)> CloseDelegate;
	MulticastDelegate1<bool &> CloseDelegate;

public:
	void HandleMouseMessage(UINT message, WPARAM wparam, LPARAM lparam);
	void HandleMouseLeave();
	void DrawNonClient();
	void RecreateResouce();
	void UpdateShadowFrame(bool bRedraw);
	Events TranslateMessageCode(UINT message);

	LRESULT OnImeEvent(UINT message, WPARAM wparam, LPARAM lparam); // TODO change name
	LRESULT HandleNcHitTest(const POINT &pt);
	void ShowShadowFrame(bool);
};

} // namespace ltk
