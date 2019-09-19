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

class Sprite : public boost::enable_shared_from_this<Sprite>
{
public:
    Sprite(void);
    virtual ~Sprite(void);

	ImmutableString ObjectName;

    RectF GetRect();
	RectF GetClientRect();
	void SetRect( RectF rect );
	RectF GetAbsRect();

    float GetWidth();
    float GetHeight();

	void Invalidate();

	void SetWindow( shared_ptr<Window> wnd );
	shared_ptr<Window> GetWindow();

	void SetVisible( bool );
	bool GetVisible();

	void AddChild( shared_ptr<Sprite> sp );
	void RemoveChild( shared_ptr<Sprite> sp );

    void HandlePaint( ID2D1RenderTarget *target );

	void HandleKeyEvent( UINT message, DWORD keyCode, DWORD flag );

	void HandleImeInput( LPCTSTR text );

    void HandleRecreateResouce( ID2D1RenderTarget *target );

    void HandleThemeChange();

	void SetCapture();
	void ReleaseCapture();
    bool IsCapturing();

    void BringToFront();

	void EnableClipChildren(bool bClip);
	bool GetClipChildren();

	void TrackMouseLeave();

	bool DispatchMouseEvent(MouseEvent *ev);

	shared_ptr<Sprite> GetAncestor();
	shared_ptr<Sprite> GetParent();

    void ShowCaret();
    void SetCaretPos(RectF rc);
    void HideCaret();

    void BeginAnimation();
    void EndAnimation();

	Sprite *SetFocus();
	void KillFocus();

    virtual bool OnEvent(Event *ev);

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
    virtual void OnParentChanged(Sprite *old, Sprite *new_){}
    virtual void OnThemeChanged() {}

private:
	bool m_bVisible;
	bool m_bClipChildren; // TODO this one is useless

    RectF m_rect;
    weak_ptr<Window> m_window;

    std::vector<shared_ptr<Sprite>> m_children;
    weak_ptr<Sprite> m_parent;

	DISALLOW_COPY_AND_ASSIGN(Sprite);
};

} // namespace ltk