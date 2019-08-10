/*
** Copyright (C) 2015-2016 Chen Shi. See Copyright Notice in ltk.h
*/
#pragma once
#include "stdafx.h"
#include "ShadowFrame.h"
#include "StyleManager.h"
#include "Event.h"
#include "ImeInput.h"
#include "RefCounted.h"
#include "Delegate.h"
#include "BoxLayout.h"

namespace ltk {

class Button;
class Label;

class Window : public RTTI
{
public:
    RTTI_DECLARATIONS(Window, RTTI);

    Window(void);
    virtual ~Window(void);

    virtual bool OnEvent(Event *ev) override;

    void Create(Window *parent, RectF rc);

    void SetRect(RectF rc);

    RectF GetRect();

    SizeF GetClientSize();

    void SetTitle(const wchar_t *title);

	static void RegisterWndClass();

    LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	void OnPaint( HWND hwnd );

	HWND Handle();

	void OnImeInput(LPCTSTR text);

	void SetImePosition( float x, float y );

    Sprite *GetRootSprite() { return m_sprite; }

	void SetFocusSprite(Sprite *sp);

	Sprite *GetFocusSprite();

	void SetCaretHeight(float);

	void ShowCaret();

	void HideCaret();

	void SetCapture(Sprite *sp);

	void ReleaseCapture();

	void TrackMouseLeave(Sprite *sp);

    void BeginAnimation(Sprite *sp);
    void EndAnimation(Sprite *sp);

    void Minimize();
    void Maximize();
    void CloseWindow();

    virtual bool OnSize(float cx, float cy, DWORD flag);
    virtual bool OnClose(bool &proceed);
    virtual bool OnDestroy() { return false; }

    void OnBtnCloseClicked();
    void OnBtnMinimizeClicked();
    void OnBtnMaximizeClicked();

    ID2D1Bitmap *GetAtlasBitmap();
    void SetBackground(LPCSTR style);

public:
    Delegate<void(bool &)> CloseEvent;

private:
	void HandleMouseMessage(UINT message, WPARAM wparam, LPARAM lparam);
    void HandleMouseLeave();
    void DrawNonClient();
    void RecreateResouce();
    void UpdateShadowFrame(bool bRedraw);

    LRESULT OnImeEvent(UINT message, WPARAM wparam, LPARAM lparam);
    LRESULT HandleNcHitTest(const POINT &pt);
    enum { TIMER_ANIMATION = 100 };
    static const wchar_t * ClsName;

private:
	HWND m_hwnd = NULL;

	ImeInput m_ime;
	RECT m_rectComposition;
	int m_caretHeight;

	BoxLayout *m_sprite = nullptr; // owner
    Sprite *m_spFocus = nullptr; // TODO change to strong ref
    Sprite *m_spCapture = nullptr; // TODO change to strong ref
    Sprite *m_spHover = nullptr;
	std::unordered_set<Sprite *> m_setTrackMouseLeave;
    std::unordered_set<Sprite *> m_setAnimation;

    Label *m_labelTitle = nullptr; // owner

    ID2D1HwndRenderTarget *m_target = nullptr; // owner
    ID2D1SolidColorBrush *m_brush = nullptr; // owner

    ShadowFrame m_shadowLeft;
    ShadowFrame m_shadowTop;
    ShadowFrame m_shadowRight;
    ShadowFrame m_shadowBottom;

    ID2D1Bitmap *m_atlas = nullptr; // owner TODO share across multiple Window
    AbstractBackground *m_background = nullptr;
};

} // namespace ltk