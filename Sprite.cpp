//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Window.h"
#include "Sprite.h"
#include "Sprite_p.h"
#include "ltk.h"
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

SpritePrivate::SpritePrivate(Sprite *q) : q_ptr(q)
{
	rect.X = 0;
	rect.Y = 0;
	rect.Width = 10;
	rect.Height = 10;

	bVisible = true;
	bClipChildren = false;
}

SpritePrivate::~SpritePrivate()
{
	for (size_t i = 0; i < this->children.size(); i++) {
		delete this->children[i];
	}
}

Sprite::Sprite() : Object(new SpritePrivate(this))
{
}

Sprite::Sprite(SpritePrivate *pp) : Object(pp)
{
}

Sprite::~Sprite(void)
{
}

RectF Sprite::GetRect()
{
	LTK_D(Sprite);
	return d->rect;
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
	LTK_D(Sprite);
	Sprite *sp = d->parent;
	RectF rcSelf = GetRect();
	RectF rcParent;
	while(sp)
	{
		rcParent = sp->GetRect();
		rcSelf.Offset(rcParent.X, rcParent.Y);
		sp = sp->d_func()->parent;
	}
	return rcSelf;
}

float Sprite::GetWidth()
{
	LTK_D(Sprite);
	return d->rect.Width;
}

float Sprite::GetHeight()
{
	LTK_D(Sprite);
	return d->rect.Height;
}

void Sprite::SetRect( RectF rect )
{
	LTK_D(Sprite);
	// 检查下宽高是否小于0 是则设为0 然后0宽或0高要在OnDraw这些里面特殊处理一下
	rect.Width = max(0.0f, rect.Width);
	rect.Height = max(0.0f, rect.Height);
	if (!d->rect.Equals(rect))
	{
		RectF rcOld = d->rect;
		// TODO OnMove
		d->rect = rect;
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
    if (wnd)
	{
		BOOL ret = ::InvalidateRect(wnd->Handle(), NULL, FALSE);
        LTK_ASSERT(ret);
	}
}

void Sprite::SetWindow( Window *wnd )
{
	LTK_D(Sprite);
	//LTK_LOG("SetWindow: %08x", wnd);
	d->window = wnd;
}

void Sprite::HandlePaint( ID2D1RenderTarget *target )
{
	LTK_D(Sprite);
	if (!d->bVisible)
	{
		return; // 子节点也不会被绘制
	}
	// 前序遍历 让父节点先绘制
	//RectF rc = GetRect();
	//if (10 == rc.Width && 10 == rc.Height)
	//{
	//	LOGW(<<L"Orignal Size 10 10"); // 检查下有没有多余的重绘
	//}

	if (d->bClipChildren)
	{
        auto rcSprite = this->GetClientRect();
        D2D1_RECT_F rcClip = D2D1RectF(rcSprite);
        target->PushAxisAlignedClip(rcClip, D2D1_ANTIALIAS_MODE_ALIASED);
	}
    PaintEvent ev;
    ev.id = ePaint;
    ev.target = target;
    OnPaint(&ev);

    for (size_t i = 0; i < d->children.size(); i++) {
        auto sp = d->children[i];
		RectF rc2 = sp->GetRect();

		TranslateTransform(target, rc2.X, rc2.Y);
		sp->HandlePaint(target);
		TranslateTransform(target, -rc2.X, -rc2.Y);
	}

	if (d->bClipChildren)
	{
        target->PopAxisAlignedClip();
	}
}

void Sprite::AddChild(Sprite *sp)
{
	LTK_D(Sprite);
	for (UINT i = d->children.size(); i > 0; i--) {
        if (d->children[i - 1] == sp) {
			//LTK_ASSERT(false);
			return;
        }
    }
	//sp->SetWindow(d->window);
    if (sp->d_func()->parent) {
        // if sp already has a parent, remove it first.
        sp->d_func()->parent->RemoveChild(sp);
    }
	d->children.push_back(sp);
	sp->OnParentChanged(sp->d_func()->parent, this);
	sp->d_func()->parent = this;
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

// return weak ref
Window * Sprite::GetWindow()
{
	LTK_D(Sprite);
	Sprite *sp = this;
	while (sp->d_func()->parent)
	{
		sp = sp->d_func()->parent;
	}
	return sp->d_func()->window;
}

void Sprite::SetCapture()
{
    auto wnd = GetWindow();
    LTK_ASSERT(wnd);
	if (wnd)
	{
		wnd->SetCapture(this);
	}
}

void Sprite::ReleaseCapture()
{
    auto wnd = GetWindow();
    LTK_ASSERT(wnd);
	if (wnd)
	{
		wnd->ReleaseCapture();
	}
}

bool Sprite::IsCapturing()
{
    Window *wnd = GetWindow();
    if (wnd) {
        return wnd->IsCapturing(this);
    } else {
        return false;
    }
}

void Sprite::BringToFront()
{
	LTK_ASSERT(false);
}

void Sprite::SetVisible( bool v )
{
	LTK_D(Sprite);
	if (d->bVisible != v)
	{
		Invalidate();
	}
	d->bVisible = v;
}

bool Sprite::GetVisible()
{
	LTK_D(Sprite);
	return d->bVisible;
}

void Sprite::EnableClipChildren( bool bClip )
{
	LTK_D(Sprite);
	if (d->bClipChildren != bClip)
	{
		d->bClipChildren = bClip;
		Invalidate();
	}
}

bool Sprite::IsClipChildren()
{
	LTK_D(Sprite);
	return d->bClipChildren;
}

bool Sprite::DispatchMouseEvent(MouseEvent *ev)
{
	LTK_D(Sprite);
	if (!d->bVisible) {
		return false;
	}
    for (auto i = d->children.size(); i > 0; i--) {
        auto sp = d->children[i - 1];
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

Sprite * Sprite::GetAncestor()
{
	LTK_D(Sprite);
	Sprite *sp = this;
	while (sp->d_func()->parent)
	{
		sp = sp->d_func()->parent;
	}
	return sp;
}

Sprite * Sprite::GetParent()
{
	LTK_D(Sprite);
	return d->parent;
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
	LTK_D(Sprite);
	// maybe searh from the end is better, because we always push to the end.
    for (int i = d->children.size() - 1; i >= 0; i--) {
        auto sp2 = d->children[i];
        if (sp2 == sp) {
            //sp2->Release();
			sp2->d_func()->parent = nullptr;
            for (int j = i + 1; j < (int)d->children.size(); j++) {
                d->children[j - 1] = d->children[j];
            }
            d->children.pop_back();
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
    LTK_ASSERT(ret);
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

void Sprite::HandleRecreateResouce(ID2D1RenderTarget *target)
{
	LTK_D(Sprite);
	for (size_t i = 0; i < d->children.size(); i++) {
        auto sp = d->children[i];
        sp->HandleRecreateResouce(target);
    }
    this->OnRecreateResouce(target);
}

void Sprite::HandleThemeChange()
{
	LTK_D(Sprite);
	this->OnThemeChanged();
    for (auto sp : d->children) {
        sp->HandleThemeChange();
    }
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

Sprite* Sprite::SetFocus()
{
	auto wnd = GetWindow();
	if (wnd) {
		auto old = wnd->GetFocusSprite();
		wnd->SetFocusSprite(this);
		return old;
	}
	return nullptr;
}

void Sprite::KillFocus()
{
	auto wnd = GetWindow();
	if (wnd) {
		if (wnd->GetFocusSprite() == this) {
			wnd->SetFocusSprite(nullptr);
		}
	}
}

LPCSTR Sprite::GetName()
{
	LTK_D(Sprite);
	return d->name;
}

void Sprite::SetName(LPCSTR n)
{
	LTK_D(Sprite);
	d->name = n;
}

} // namespace ltk