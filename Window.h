//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "stdafx.h"
#include "ShadowFrame.h"
#include "Event.h"
#include "ImeInput.h"
#include "Delegate.h"
#include "Object.h"
#include "LtkInterface.h"

namespace ltk {

class Button;
class Label;
class WindowLayout;
class AbstractBackground;
class MenuBar;

class Window : public Object
{
public:
	RTTI_DECLARATIONS(Window, Object);

    Window(void);
    virtual ~Window(void);

    void Create(Window *parent, RectF rc);

    void SetRect(RectF rc);
    RectF GetRect();

    SizeF GetClientSize();

    void SetCaption(LPCWSTR text);

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
    Delegate<void(bool &)> CloseEvent;

private:
	void HandleMouseMessage(UINT message, WPARAM wparam, LPARAM lparam);
    void HandleMouseLeave();
    void DrawNonClient();
    void RecreateResouce();
    void UpdateShadowFrame(bool bRedraw);
	Events TranslateMessageCode(UINT message);

    LRESULT OnImeEvent(UINT message, WPARAM wparam, LPARAM lparam);
    LRESULT HandleNcHitTest(const POINT &pt);
    enum { TIMER_ANIMATION = 100 };
    static const wchar_t * ClsName;

private:
	HWND m_hwnd = NULL;

	ImeInput m_ime;
	RECT m_rectComposition;
	int m_caretHeight;

	WindowLayout *m_sprite = nullptr; // owner

	bool m_bEnableFocusChange = true;
    Sprite *m_spFocus = nullptr;
    Sprite *m_spCapture = nullptr;
    Sprite *m_spHover = nullptr;
	std::unordered_set<Sprite *> m_setTrackMouseLeave;
    std::unordered_set<Sprite *> m_setAnimation;

    ID2D1HwndRenderTarget *m_target = nullptr; // owner
    ID2D1SolidColorBrush *m_brush = nullptr; // owner

    ShadowFrame m_shadowLeft;
    ShadowFrame m_shadowTop;
    ShadowFrame m_shadowRight;
    ShadowFrame m_shadowBottom;

    ID2D1Bitmap *m_atlas = nullptr; // owner TODO share across multiple Window
    AbstractBackground *m_background = nullptr;
    std::string m_styleName;
};

} // namespace ltk