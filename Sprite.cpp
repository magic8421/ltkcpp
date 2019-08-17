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

Sprite::Sprite(void)
{
	m_rect.X = 0;
	m_rect.Y = 0;
	m_rect.Width = 10;
	m_rect.Height = 10;

	m_bVisible = true;
	m_enableFocus = false;
	m_bMouseIn = false;
	m_bClipChildren = false;
}

Sprite::~Sprite(void)
{
    for (size_t i = 0; i < m_children.size(); i++) {
        delete m_children[i];
	}
    m_window = INVALID_POINTER(Window);
    m_parent = INVALID_POINTER(Sprite);

    delete m_name;
    m_name = INVALID_POINTER(const char);
}

RectF Sprite::GetRect()
{
	return m_rect;
}

void Sprite::SetName(const char *name)
{
    delete m_name;
    m_name = _strdup(name);
}

const char * Sprite::GetName()
{
    return m_name;
}

RectF Sprite::GetClientRect()
{
	RectF rc = this->GetRect();
	rc.X = 0.0f;
	rc.Y = 0.0f;
	return rc;
}

RectF Sprite::GetAbsRect()
{
	Sprite *sp = m_parent;
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

float Sprite::GetWidth()
{
    return m_rect.Width;
}

float Sprite::GetHeight()
{
    return m_rect.Height;
}

void Sprite::SetRect( RectF rect )
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

void Sprite::Invalidate()
{
	// 0指针访问 不挂是因为x64系统一个bug 记得打开调试中的Win32异常断点
	Window *wnd = GetWindow();
    if (wnd && ::IsWindow(wnd->Handle()))
	{
		BOOL ret = ::InvalidateRect(wnd->Handle(), NULL, FALSE);
        LTK_ASSERT(ret);
	}
}

void Sprite::SetWindow( Window *wnd )
{
    //LTK_LOG("SetWindow: %08x", wnd);
	m_window = wnd;
}

void Sprite::SetWindowRecursive(Window *wnd)
{
    for (size_t i = 0; i < m_children.size(); i++) {
        auto sp = m_children[i];
        sp->SetWindowRecursive(wnd);
        sp->m_window = wnd;
    }
}

void Sprite::HandlePaint( ID2D1RenderTarget *target )
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
        auto rcSprite = this->GetClientRect();
        D2D1_RECT_F rcClip = D2D1RectF(rcSprite);
        target->PushAxisAlignedClip(rcClip, D2D1_ANTIALIAS_MODE_ALIASED);
	}
    PaintEvent ev;
    ev.id = ePaint;
    ev.target = target;
    OnEvent(&ev);

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

void Sprite::AddChild(Sprite *sp)
{
    if (sp->m_parent == this) {
        LTK_LOG("same parent");
        return;
    }
    for (UINT i = 0; i < m_children.size(); i ++) {
        if (m_children[i] == sp) {
            __debugbreak();
        }
    }
    m_children.push_back(sp);
	sp->SetWindow(m_window);
    if (sp->m_parent) {
        // if sp already has a parent, remove it first.
        sp->m_parent->RemoveChild(sp);
    }
    sp->OnParentChanged(sp->m_parent, this);
	sp->m_parent = this;
}

bool Sprite::TranslateMouseEvent( MouseEvent *ev )
{
    bool ret = false;
	switch(ev->message)
	{
	case WM_MOUSEMOVE:
        ev->id = eMouseMove;
        ret = OnEvent(ev);
		if (!m_bMouseIn)
		{
			m_bMouseIn = true;
            ev->id = eMouseEnter;
            OnEvent(ev); // 例外
		}
		break;
	case WM_MOUSEWHEEL:
        ev->id = eMouseWheel;
        ret = OnEvent(ev);
		break;
	case WM_LBUTTONDOWN:
        ev->id = eLBtnDown;
        ret = OnEvent(ev);
		if (m_enableFocus)
		{
			GetWindow()->SetFocusSprite(this);
		}
		break;
	case WM_LBUTTONUP:
        ev->id = eLBtnUp;
        ret = OnEvent(ev);
        break;
	case WM_MOUSELEAVE:
		m_bMouseIn = false;
        ev->id = eMouseLeave;
        OnEvent(ev); // 例外
        break;
	}
    return ret;
}

void Sprite::HandleCapturedMouseEvent( MouseEvent *ev)
{
	switch(ev->message)
	{
	case WM_MOUSEMOVE:
        ev->id = eMouseMove;
        OnEvent(ev);
		break;
	case WM_LBUTTONDOWN:
        ev->id = eLBtnDown;
        OnEvent(ev);
		break;
	case WM_LBUTTONUP:
        ev->id = eLBtnUp;
        OnEvent(ev);
        break;
	}			
}

void Sprite::HandleKeyEvent( UINT message, DWORD keyCode, DWORD flag )
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

void Sprite::HandleImeInput(LPCTSTR text)
{
    ImeEvent ev;
    ev.id = eImeInput;
    ev.text = text;
    OnEvent(&ev);
}

void Sprite::EnableFocus( bool enable )
{
	m_enableFocus = enable;
}

// return weak ref
Window * Sprite::GetWindow()
{
	Sprite *sp = this;
	while (sp->m_parent)
	{
		sp = sp->m_parent;
	}
	return sp->m_window;
}

void Sprite::SetCapture()
{
    auto wnd = GetWindow();
    assert(wnd); // 这个在lua包装函数里检查 报成lua错误
	if (wnd)
	{
		wnd->SetCapture(this);
	}
}

void Sprite::ReleaseCapture()
{
	assert(GetWindow()); // 这个在lua包装函数里检查 报成lua错误
	if (GetWindow())
	{
		GetWindow()->ReleaseCapture();
	}
}

void Sprite::BringToFront()
{
    __debugbreak();
}

void Sprite::SetVisible( bool v )
{
	if (m_bVisible != v)
	{
		Invalidate();
	}
	m_bVisible = v;
}

bool Sprite::GetVisible()
{
	return m_bVisible;
}

void Sprite::EnableClipChildren( bool bClip )
{
	if (m_bClipChildren != bClip)
	{
		m_bClipChildren = bClip;
		Invalidate();
	}
}

bool Sprite::GetClipChildren()
{
	return m_bClipChildren;
}

bool Sprite::DispatchMouseEvent(MouseEvent *ev)
{
    for (size_t i = 0; i < m_children.size(); i++) {
        auto sp = m_children[i];
        RectF rc = sp->GetRect();
        if (rc.Contains(ev->x, ev->y)) {
            MouseEvent ev2 = *ev;
            ev2.x -= rc.X;
            ev2.y -= rc.Y;
            if (sp->DispatchMouseEvent(&ev2)) {
                return true;
            }
        }
    }
    return this->TranslateMouseEvent(ev);
}

Sprite * Sprite::GetAncestor()
{
	Sprite *sp = this;
	while (sp->m_parent)
	{
		sp = sp->m_parent;
	}
	return sp;
}

Sprite * Sprite::GetParent()
{
	return m_parent;
}

void Sprite::TrackMouseLeave()
{
	Window *wnd = GetWindow();
	if (wnd)
	{
		wnd->TrackMouseLeave(this);
	}
}

void Sprite::RemoveChild( Sprite *sp )
{
    // maybe searh from the end is better, because we always push to the end.
    for (int i = m_children.size() - 1; i >= 0; i--) {
        auto sp2 = m_children[i];
        if (sp2 == sp) {
            //sp2->Release();
            for (int j = i + 1; j < (int)m_children.size(); j++) {
                m_children[j - 1] = m_children[j];
            }
            m_children.pop_back();
            i--;
        }
    }
}

void Sprite::ShowCaret()
{
    GetWindow()->ShowCaret();
}

void Sprite::SetCaretPos(RectF rc)
{
    RECT rc2 = DipRectToScreen(rc);
    RECT arc = DipRectToScreen(Sprite::GetAbsRect());
    rc2.left += arc.left;
    rc2.top += arc.top;
    rc2.right += arc.left;
    rc2.bottom += arc.top;
    GetWindow()->SetImePosition((float)rc2.left, (float)rc2.top);
    HWND hwnd = GetWindow()->Handle();
    ::DestroyCaret(); // 这里销毁重新建立 才能改变高度
    ::CreateCaret(hwnd, NULL, (int)rc.Width, (int)rc.Height); // 可以加个参数制定虚线光标(HBITMAP)1
    BOOL ret = ::ShowCaret(hwnd); // TODO 这里太脏了 可能显示出来就隐藏不掉 应该一对一绑定
    assert(ret);
    GetWindow()->SetCaretHeight(rc.Height);
    ::SetCaretPos(rc2.left, rc2.top);
}

void Sprite::HideCaret()
{
    ::DestroyCaret();
}

bool Sprite::OnEvent(Event *ev)
{
    bool bHandled = false;
    UINT id = ev->id;

    switch (id)
    {
    case ePaint:
        bHandled = OnPaint(static_cast<PaintEvent*>(ev));
        break;
    case eMouseMove:
        bHandled = OnMouseMove(static_cast<MouseEvent*>(ev));
        break;
    case eSizeChanged:
        bHandled = OnSize(static_cast<SizeEvent*>(ev));
        break;
    case eMouseEnter:
        bHandled = OnMouseEnter(static_cast<MouseEvent*>(ev));
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

void Sprite::HandleRecreateResouce(ID2D1RenderTarget *target)
{
    for (size_t i = 0; i < m_children.size(); i++) {
        auto sp = m_children[i];
        sp->HandleRecreateResouce(target);
    }
    this->RecreateResouce(target);
}

void Sprite::BeginAnimation()
{
    //LTK_LOG("BeginAnimation");
    Window *wnd = GetWindow();
    wnd->BeginAnimation(this);
}

void Sprite::EndAnimation()
{
    //LTK_LOG("EndAnimation");
    Window *wnd = GetWindow();
    if (wnd) {
        wnd->EndAnimation(this);
    }
}

} // namespace ltk