/*
** Copyright (C) 2015-2016 Chen Shi. See Copyright Notice in ltk.h
*/
#pragma once
#include "Common.h"
#include "Event.h"
#include "RefCounted.h"

namespace ltk {

class Window;

class Sprite : public RefCounted
{
protected:
    virtual ~Sprite(void);

public:
    RTTI_DECLARATIONS(Sprite, RefCounted);

    Sprite(void);

	RectF GetRect();

	RectF GetClientRect();

	void SetRect( RectF rect );

	RectF GetAbsRect();

	void Invalidate();

	void SetWindow( Window *wnd );
    void SetWindowRecursive( Window *wnd );
	Window *GetWindow();

	void SetVisible( bool );

	bool GetVisible();

	void AddChild( Sprite *sp );

    void HandlePaint( ID2D1RenderTarget *target );

    void TranslateMouseEvent(MouseEvent *ev);

	void HandleCapturedMouseEvent( MouseEvent *ev );

	void HandleKeyEvent( UINT message, DWORD keyCode, DWORD flag );

	void HandleImeInput( LPCTSTR text );

    void HandleRecreateResouce( ID2D1RenderTarget *target );

	void EnableFocus(bool enable);

	void SetCapture();

	void ReleaseCapture();

	void BringToFront();

	void SetClipChildren(bool bClip);

	bool GetClipChildren();

	void TrackMouseLeave();

	void DispatchMouseEvent(MouseEvent *ev);

    Sprite *DispatchMouseEvent2(MouseEvent *ev);

	void RemoveChild(Sprite *sp);

	Sprite *GetAncestor();

	Sprite *GetParent();

	Sprite *GetNextSprite();

	Sprite *GetPrevSprite();

	Sprite *GetFirstChild();

	Sprite *GetLastChild();

    void ShowCaret();
    void SetCaretPos(RectF rc);
    void HideCaret();

    void BeginAnimation();
    void EndAnimation();

    virtual bool OnEvent(Event *ev) override;

    virtual bool OnPaint            (PaintEvent *ev) { return false; }
    virtual bool OnLBtnDown         (MouseEvent *ev) { return false; }
    virtual bool OnLBtnUp           (MouseEvent *ev) { return false; }
    virtual bool OnLBtnDbClk        (MouseEvent *ev) { return false; }
    virtual bool OnRBtnDown         (MouseEvent *ev) { return false; }
    virtual bool OnRBtnUp           (MouseEvent *ev) { return false; }
    virtual bool OnMouseMove        (MouseEvent *ev) { return false; }
    virtual bool OnMouseEnter       (MouseEvent *ev) { return false; }
    virtual bool OnMouseLeave       (MouseEvent *ev) { return false; }
    virtual bool OnMouseWheel       (MouseEvent *ev) { return false; }
    virtual bool OnKeyDown          (KeyEvent *ev)   { return false; }
    virtual bool OnKeyUp            (KeyEvent *ev)   { return false; }
    virtual bool OnChar             (KeyEvent *ev)   { return false; }
    virtual bool OnSize             (SizeEvent *ev)  { return false; }
    virtual bool OnImeInput         (ImeEvent *ev)   { return false; }
    virtual bool OnSetFocus         (FocusEvent *ev) { return false; }
    virtual bool OnKillFocus        (FocusEvent *ev) { return false; }

    void SetDelegate(RTTI *dlgt);
    void DelegateEvent(Event *ev);

    virtual void RecreateResouce(ID2D1RenderTarget *target){}

    void SetName(const char *name);
    const char *GetName();

private:
	bool m_bVisible; // 编译器会自动优化成1字节
	bool m_bMouseIn;
	bool m_enableFocus;
	bool m_bClipChildren;
    bool m_bShowCaret;

    RectF m_rect;
    Window *m_window = nullptr;

    ArrayList<Sprite*> m_children;
    Sprite *m_parent = nullptr;

    const char *m_name = nullptr;
    RTTI *m_delegate = nullptr;

	DISALLOW_COPY_AND_ASSIGN(Sprite);
};

} // namespace ltk