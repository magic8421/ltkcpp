//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "Common.h"
#include "Event.h"
#include "Object.h"

namespace ltk {

class Window;

struct LayoutParam
{
    SizeF preferred_ize;
    PointF grow_factor;
};

typedef void (CALLBACK *ActionCallback)(void* userdata, LPCSTR name, BOOL* pbHandled);

class LTK_CPP_API Widget : public Object
{
public:
	RTTI_DECLARATIONS(Widget, Object);

    Widget(void);
    virtual ~Widget(void);

    RectF GetRect();
	RectF GetClientRect();
	RectF GetAbsRect();
    float GetX();
    float GetY();
    float GetWidth();
    float GetHeight();

    void SetRect(RectF rect);
    void SetRect(float x, float y, float width, float height);
    void SetPosition(float x, float y);
    void SetSize(float width, float height);

	void Invalidate();

	void SetWindow( Window *wnd );
	Window *GetWindow();

	void SetVisible( bool );
	bool GetVisible();

	void SetCapture();
	void ReleaseCapture();
    bool IsCapturing();

    void BringToFront();

	void EnableClipChildren(bool bClip);
	bool GetClipChildren();

	void TrackMouseLeave();

    Widget *GetParentWidget();
	Widget *GetAncestor();

    void ShowCaret();
    void SetCaretPos(RectF rc);
    void HideCaret();

    void BeginAnimation();
    void EndAnimation();

	Widget *SetFocus();
	void KillFocus();

    bool DispatchMouseEvent(MouseEvent *ev);
    void HandlePaint( ID2D1RenderTarget *target );
    void HandleKeyEvent( UINT message, DWORD keyCode, DWORD flag );
    void HandleImeInput( LPCTSTR text );
    void HandleRecreateResouce( ID2D1RenderTarget *target );
    void HandleThemeChange();

    virtual void SetAttribute(LPCSTR name, LPCSTR value) override;

    virtual bool OnEvent(Event *ev) override;

protected:
    virtual bool OnPaint            (PaintEvent *ev) { return false; }
    virtual bool OnLBtnDown         (MouseEvent *ev) { return false; }
    virtual bool OnLBtnUp           (MouseEvent *ev) { return false; }
    virtual bool OnLBtnDbClk        (MouseEvent *ev) { return false; }
    virtual bool OnRBtnDown         (MouseEvent *ev) { return false; }
    virtual bool OnRBtnUp           (MouseEvent *ev) { return false; }
    virtual bool OnMouseMove        (MouseEvent *ev) { return false; }
    virtual bool OnMouseLeave       (MouseEvent *ev) { return false; }
    virtual bool OnMouseWheel       (MouseEvent *ev) { return false; }
    virtual bool OnKeyDown          (KeyEvent *ev)   { return false; }
    virtual bool OnKeyUp            (KeyEvent *ev)   { return false; }
    virtual bool OnChar             (KeyEvent *ev)   { return false; }
    virtual bool OnSize             (SizeEvent *ev)  { return false; }
    virtual bool OnImeInput         (ImeEvent *ev)   { return false; }
    virtual bool OnSetFocus         (FocusEvent *ev) { return false; }
    virtual bool OnKillFocus        (FocusEvent *ev) { return false; }

    virtual void RecreateResouce(ID2D1RenderTarget *target){}
    virtual void OnParentChanged(Widget *old, Widget *new_){}
    virtual void OnThemeChanged() {}

private:
	bool m_bVisible;
	bool m_bClipChildren; // TODO this one is useless

    RectF m_rect;
    Window *m_window = nullptr;

	DISALLOW_COPY_AND_ASSIGN(Widget);
};

} // namespace ltk