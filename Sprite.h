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
class SpritePrivate;

class LTK_API Sprite : public Object
{
public:
	RTTI_DECLARATIONS(Sprite, Object);

    Sprite(void);
    virtual ~Sprite(void);

	LPCSTR GetName();
	void SetName(LPCSTR);

    RectF GetRect();
	RectF GetClientRect();
	void SetRect( RectF rect );
	RectF GetAbsRect();

    float GetWidth();
    float GetHeight();

	void Invalidate();

	void SetWindow( Window *wnd );
	Window *GetWindow();

	void SetVisible( bool );
	bool GetVisible();

	void AddChild( Sprite *sp );
	void RemoveChild(Sprite* sp);

	void SetCapture();
	void ReleaseCapture();
    bool IsCapturing();

    void BringToFront();

	void EnableClipChildren(bool bClip);
	bool IsClipChildren();

	void TrackMouseLeave();

	Sprite *GetAncestor();
	Sprite *GetParent();

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

    virtual void OnRecreateResouce(ID2D1RenderTarget *target){}
    virtual void OnParentChanged(Sprite *old, Sprite *new_){}
    virtual void OnThemeChanged() {}

	friend class Window;

protected:
	Sprite(SpritePrivate *pp);

private:
	LTK_DECLARE_PRIVATE(Sprite)
};

} // namespace ltk