//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Window.h"
#include "Sprite.h"
#include "ltk.h"
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

Widget::Widget(void)
{
	m_rect.X = 0;
	m_rect.Y = 0;
	m_rect.Width = 10;
	m_rect.Height = 10;

	m_bVisible = true;
	m_bClipChildren = false;
}

Widget::~Widget(void)
{
    for (size_t i = 0; i < m_children.size(); i++) {
        delete m_children[i];
	}
}

RectF Widget::GetRect()
{
	return m_rect;
}

RectF Widget::GetClientRect()
{
	RectF rc = this->GetRect();
	rc.X = 0.0f;
	rc.Y = 0.0f;
	return rc;
}

RectF Widget::GetAbsRect()
{
	Widget *sp = m_parent;
	RectF rcSelf = GetRect();
	RectF rcParent;
	while(sp)
	{
		rcParent = sp->GetRect();
		rcSelf.Offset(rcParent.X, rcParent.Y);
		sp = sp->m_parent;
	}
	return rcSelf;
}

float Widget::GetX()
{
    return m_rect.X;
}

float Widget::GetY()
{
    return m_rect.Y;
}

float Widget::GetWidth()
{
    return m_rect.Width;
}

float Widget::GetHeight()
{
    return m_rect.Height;
}

void Widget::SetRect(float x, float y, float width, float height)
{
    this->SetRect(RectF(x, y, width, height));
}

void Widget::SetPosition(float x, float y)
{
    this->SetRect(RectF(x, y, this->GetWidth(), this->GetHeight()));
}

void Widget::SetSize(float width, float height)
{
    this->SetRect(RectF(this->GetX(), this->GetY(), width, height));
}

void Widget::SetRect(RectF rect)
{
	// 检查下宽高是否小于0 是则设为0 然后0宽或0高要在OnDraw这些里面特殊处理一下
	rect.Width = max(0.0f, rect.Width);
	rect.Height = max(0.0f, rect.Height);
	if (!m_rect.Equals(rect))
	{
		RectF rcOld = m_rect;
		// TODO OnMove
		m_rect = rect;
		this->Invalidate(); // 刷新整个窗口
		// 原先回掉的顺序错了 导致在OnSize里面GetRect和OnSize的参数不一样 诡异错误
		if (rect.Width != rcOld.Width || rect.Height != rcOld.Height)
		{
            SizeEvent ev;
            ev.id = eSizeChanged;
            ev.width = rect.Width;
            ev.height = rect.Height;
            this->OnEvent(&ev);
		}
	}
}

void Widget::Invalidate()
{
	// 0指针访问 不挂是因为x64系统一个bug 记得打开调试中的Win32异常断点
	Window *wnd = GetWindow();
    if (wnd)
	{
		BOOL ret = ::InvalidateRect(wnd->GetHWND(), NULL, FALSE);
        LTK_ASSERT(ret);
	}
}

void Widget::SetWindow( Window *wnd )
{
    //LTK_LOG("SetWindow: %08x", wnd);
	m_window = wnd;
}

void Widget::HandlePaint( ID2D1RenderTarget *target )
{
	if (!m_bVisible)
	{
		return; // 子节点也不会被绘制
	}
	// 前序遍历 让父节点先绘制
	//RectF rc = GetRect();
	//if (10 == rc.Width && 10 == rc.Height)
	//{
	//	LOGW(<<L"Orignal Size 10 10"); // 检查下有没有多余的重绘
	//}

	if (m_bClipChildren)
	{
        auto rcWidget = this->GetClientRect();
        D2D1_RECT_F rcClip = D2D1RectF(rcWidget);
        target->PushAxisAlignedClip(rcClip, D2D1_ANTIALIAS_MODE_ALIASED);
	}
    PaintEvent ev;
    ev.id = ePaint;
    ev.target = target;
    OnPaint(&ev);

    for (size_t i = 0; i < m_children.size(); i++) {
        auto sp = m_children[i];
		RectF rc2 = sp->GetRect();

		TranslateTransform(target, rc2.X, rc2.Y);
		sp->HandlePaint(target);
		TranslateTransform(target, -rc2.X, -rc2.Y);
	}

	if (m_bClipChildren)
	{
        target->PopAxisAlignedClip();
	}
}

void Widget::AddChild(Widget *sp)
{
    for (UINT i =  m_children.size(); i > 0; i--) {
        if (m_children[i - 1] == sp) {
			//LTK_ASSERT(false);
			return;
        }
    }
	//sp->SetWindow(m_window);
    if (sp->m_parent) {
        // if sp already has a parent, remove it first.
        sp->m_parent->RemoveChild(sp);
    }
	m_children.push_back(sp);
	sp->OnParentChanged(sp->m_parent, this);
	sp->m_parent = this;
}

void Widget::HandleKeyEvent( UINT message, DWORD keyCode, DWORD flag )
{
	KeyEvent ev;
    ev.keyCode = keyCode;
    ev.flag = flag;

	switch(message)
	{
	case WM_KEYDOWN:
        ev.id = eKeyDown;
        OnEvent(&ev);
		break;
	case WM_KEYUP:
        ev.id = eKeyUp;
        OnEvent(&ev);
		break;
	case WM_CHAR:
        ev.id = eCharInput;
        OnEvent(&ev);
		break;
	}
}

void Widget::HandleImeInput(LPCTSTR text)
{
    ImeEvent ev;
    ev.id = eImeInput;
    ev.text = text;
    OnEvent(&ev);
}

// return weak ref
Window * Widget::GetWindow()
{
	Widget *sp = this;
	while (sp->m_parent)
	{
		sp = sp->m_parent;
	}
	return sp->m_window;
}

void Widget::SetCapture()
{
    auto wnd = GetWindow();
    LTK_ASSERT(wnd);
	if (wnd)
	{
		wnd->SetCapture(this);
	}
}

void Widget::ReleaseCapture()
{
    auto wnd = GetWindow();
    LTK_ASSERT(wnd);
	if (wnd)
	{
		wnd->ReleaseCapture();
	}
}

bool Widget::IsCapturing()
{
    Window *wnd = GetWindow();
    if (wnd) {
        return wnd->IsCapturing(this);
    } else {
        return false;
    }
}

void Widget::BringToFront()
{
    auto parent = m_parent;
	m_parent->RemoveChild(this);
    parent->AddChild(this);
}

void Widget::SetVisible( bool v )
{
	if (m_bVisible != v)
	{
		Invalidate();
	}
	m_bVisible = v;
}

bool Widget::GetVisible()
{
	return m_bVisible;
}

void Widget::EnableClipChildren( bool bClip )
{
	if (m_bClipChildren != bClip)
	{
		m_bClipChildren = bClip;
		Invalidate();
	}
}

bool Widget::GetClipChildren()
{
	return m_bClipChildren;
}

bool Widget::DispatchMouseEvent(MouseEvent *ev)
{
	if (!m_bVisible) {
		return false;
	}
    for (auto i = m_children.size(); i > 0; i--) {
        auto sp = m_children[i - 1];
        auto rc = sp->GetRect();
        if (rc.Contains(ev->x, ev->y)) {
            MouseEvent ev2 = *ev;
            ev2.x -= rc.X;
            ev2.y -= rc.Y;
            if (sp->DispatchMouseEvent(&ev2)) {
                return true;
            }
        }
    }
    return this->OnEvent(ev);
}

Widget * Widget::GetAncestor()
{
	Widget *sp = this;
	while (sp->m_parent)
	{
		sp = sp->m_parent;
	}
	return sp;
}

Widget * Widget::GetParent()
{
	return m_parent;
}

void Widget::TrackMouseLeave()
{
	Window *wnd = GetWindow();
	if (wnd)
	{
		wnd->TrackMouseLeave(this);
	}
}

void Widget::RemoveChild( Widget *sp )
{
    // maybe searh from the end is better, because we always push to the end.
    for (int i = m_children.size() - 1; i >= 0; i--) {
        auto sp2 = m_children[i];
        if (sp2 == sp) {
            //sp2->Release();
			sp2->m_parent = nullptr;
            for (int j = i + 1; j < (int)m_children.size(); j++) {
                m_children[j - 1] = m_children[j];
            }
            m_children.pop_back();
            i--;
        }
    }
}

void Widget::ShowCaret()
{
    GetWindow()->ShowCaret();
}

void Widget::SetCaretPos(RectF rc)
{
    RECT rc2 = DipRectToScreen(rc);
    RECT arc = DipRectToScreen(Widget::GetAbsRect());
    rc2.left += arc.left;
    rc2.top += arc.top;
    rc2.right += arc.left;
    rc2.bottom += arc.top;
    GetWindow()->SetImePosition((float)rc2.left, (float)rc2.top);
    HWND hwnd = GetWindow()->GetHWND();
    ::DestroyCaret(); // 这里销毁重新建立 才能改变高度
    ::CreateCaret(hwnd, NULL, (int)rc.Width, (int)rc.Height); // 可以加个参数制定虚线光标(HBITMAP)1
    BOOL ret = ::ShowCaret(hwnd); // TODO 这里太脏了 可能显示出来就隐藏不掉 应该一对一绑定
    LTK_ASSERT(ret);
    GetWindow()->SetCaretHeight(rc.Height);
    ::SetCaretPos(rc2.left, rc2.top);
}

void Widget::HideCaret()
{
    ::DestroyCaret();
}

bool Widget::OnEvent(Event *ev)
{
    bool bHandled = false;
    UINT id = ev->id;

    switch (id)
    {
    case eMouseMove:
        bHandled = OnMouseMove(static_cast<MouseEvent*>(ev));
        break;
    case eSizeChanged:
        bHandled = OnSize(static_cast<SizeEvent*>(ev));
        break;
    case eMouseLeave:
        bHandled = OnMouseLeave(static_cast<MouseEvent*>(ev));
        break;
    case eLBtnDown:
        bHandled = OnLBtnDown(static_cast<MouseEvent*>(ev));
        break;
    case eLBtnUp:
        bHandled = OnLBtnUp(static_cast<MouseEvent*>(ev));
        break;
    case eRBtnDown:
        bHandled = OnRBtnDown(static_cast<MouseEvent*>(ev));
        break;
    case eRBtnUp:
        bHandled = OnRBtnUp(static_cast<MouseEvent*>(ev));
        break;
    case eMouseWheel:
        bHandled = OnMouseWheel(static_cast<MouseEvent*>(ev));
        break;
    case eKeyDown:
        bHandled = OnKeyDown(static_cast<KeyEvent*>(ev));
        break;
    case eKeyUp:
        bHandled = OnKeyUp(static_cast<KeyEvent*>(ev));
        break;
    case eCharInput:
        bHandled = OnChar(static_cast<KeyEvent*>(ev));
        break;
    case eImeInput:
        bHandled = OnImeInput(static_cast<ImeEvent*>(ev));
        break;
    case eSetFocus:
        bHandled = OnSetFocus(static_cast<FocusEvent*>(ev));
        break;
    case eKillFocus:
        bHandled = OnKillFocus(static_cast<FocusEvent*>(ev));
        break;
    default:
        bHandled = false;
    }
	return bHandled;
}

void Widget::HandleRecreateResouce(ID2D1RenderTarget *target)
{
    for (size_t i = 0; i < m_children.size(); i++) {
        auto sp = m_children[i];
        sp->HandleRecreateResouce(target);
    }
    this->RecreateResouce(target);
}

void Widget::HandleThemeChange()
{
    this->OnThemeChanged();
    for (auto sp : m_children) {
        sp->HandleThemeChange();
    }
}

void Widget::BeginAnimation()
{
    //LTK_LOG("BeginAnimation");
    Window *wnd = GetWindow();
    wnd->BeginAnimation(this);
}

void Widget::EndAnimation()
{
    //LTK_LOG("EndAnimation");
    Window *wnd = GetWindow();
    if (wnd) {
        wnd->EndAnimation(this);
    }
}

Widget* Widget::SetFocus()
{
	auto wnd = GetWindow();
	if (wnd) {
		auto old = wnd->GetFocusWidget();
		wnd->SetFocusWidget(this);
		return old;
	}
	return nullptr;
}

void Widget::KillFocus()
{
	auto wnd = GetWindow();
	if (wnd) {
		if (wnd->GetFocusWidget() == this) {
			wnd->SetFocusWidget(nullptr);
		}
	}
}

} // namespace ltk