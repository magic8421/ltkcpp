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

#define LTK_WIDGET_IMPL() \
    STDMETHOD_(void, SetRect)(LtkRect* rect) override { \
        Widget::SetRect(rect); } \
    STDMETHOD_(void, SetVisible)(BOOL b) override { \
        Widget::SetVisible(b); } \
    STDMETHOD_(BOOL, GetVisible)() override { \
        return Widget::GetVisible(); } \
    STDMETHOD(AddChild)(ILtkWidget* w) override { \
        return Widget::AddChild(w); } \
    STDMETHOD_(void, SetRect)(float x, float y, float width, float height) override { \
        Widget::SetRect(x, y, width, height); }




class Widget : public ILtkWidget, public Object
{
public:
	RTTI_DECLARATIONS(Widget, Object);

    Widget(void);
    virtual ~Widget(void);

    LTK_OBJECT_IMPL()
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject) override;

    RectF GetRect();
	RectF GetClientRect();

    STDMETHOD_(void, SetRect)(LtkRect* rect) override;
    STDMETHOD_(void, SetRect)(float x, float y, float width, float height) override;
	void SetRect( RectF rect );
	RectF GetAbsRect();

    float GetWidth();
    float GetHeight();

	void Invalidate();

	void SetWindow( Window *wnd );
	Window *GetWindow();

    STDMETHOD_(void, SetVisible)(BOOL) override;
    STDMETHOD_(BOOL, GetVisible)() override;

    STDMETHOD(AddChild)(ILtkWidget* w) override;

    HRESULT AddChild(Widget *sp );
	void RemoveChild(Widget* sp);

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

	Widget *GetAncestor();
	Widget *GetParent();

    void ShowCaret();
    void SetCaretPos(RectF rc);
    void HideCaret();

    void BeginAnimation();
    void EndAnimation();

	Widget *SetFocus();
	void KillFocus();

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

    std::vector<Widget *> m_children;
    Widget *m_parent = nullptr;

	DISALLOW_COPY_AND_ASSIGN(Widget);
};

} // namespace ltk