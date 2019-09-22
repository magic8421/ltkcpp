//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "Event.h"
#include "Delegate.h"
#include "Object.h"

namespace ltk {

class Sprite;
class MenuBar;
class WindowPrivate;

class LTK_API Window : public Object
{
public:
	RTTI_DECLARATIONS(Window, Object);

    Window(void);
    virtual ~Window(void);

    void Create(Window *parent, RectF rc);
	void Create(Window *parent, SizeF size);

    void SetRect(RectF rc);
    RectF GetRect();

    SizeF GetClientSize();

    void SetCaption(LPCWSTR text);
	void EnableShadow(bool);

	static void RegisterWndClass();

    LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	void OnPaint( HWND hwnd );

	HWND Handle();

	void OnImeInput(LPCTSTR text);
	void SetImePosition( float x, float y );

    Sprite *GetRootSprite();
    Sprite *SetClientSprite(Sprite *sp);

	MenuBar *SetMenu(MenuBar *);
	MenuBar *GetMenu();

	void SetFocusSprite(Sprite *sp);
	Sprite *GetFocusSprite();
	void DisableFocusChange();

	void SetCaretHeight(float);

	void ShowCaret();
	void HideCaret();

	void SetCapture(Sprite *sp);
	void ReleaseCapture();
    bool IsCapturing(Sprite *sp);

    void TrackMouseLeave(Sprite *sp);

    void BeginAnimation(Sprite *sp);
    void EndAnimation(Sprite *sp);

    void Minimize();
    void Maximize();
    void CloseWindow();

    virtual bool OnSize(float cx, float cy, DWORD flag);
    virtual bool OnClose(bool &proceed);
	virtual void OnDestroy();

    ID2D1Bitmap *GetAtlasBitmap();
    ID2D1SolidColorBrush *GetStockBrush();
    void SetBackground(LPCSTR style);
    
    void UpdateTheme();
    virtual void OnThemeChanged() {}

public:
	Cookie AttachCloseDelegate(const std::function<void(bool &)> &cb);
	void RemoveCloseDelegate(Cookie c);

private:
	LTK_DECLARE_PRIVATE(Window);
};

} // namespace ltk