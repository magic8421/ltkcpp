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

SpritePrivate::SpritePrivate(Sprite *q) : ObjectPrivate(q)
{
}

SpritePrivate::~SpritePrivate()
{
}

Sprite::Sprite() : Object(new SpritePrivate(this))
{
	d_func()->Init();
}

Sprite::Sprite(SpritePrivate *d) : Object(d)
{
	d->Init();
}

Sprite::~Sprite(void)
{
}

void SpritePrivate::Init()
{
	auto d = this;
	d->rect.X = 0;
	d->rect.Y = 0;
	d->rect.Width = 10;
	d->rect.Height = 10;
	d->isSpriteType = true;
}

RectF Sprite::GetRect()
{
	LTK_CHECK_THREAD;
	LTK_D(Sprite);
	return d->rect;
}

RectF Sprite::GetClientRect()
{
	LTK_CHECK_THREAD;
	RectF rc = this->GetRect();
	rc.X = 0.0f;
	rc.Y = 0.0f;
	return rc;
}

RectF Sprite::GetAbsRect()
{
	LTK_CHECK_THREAD;
	LTK_D(Sprite);
	Sprite *sp = GetParentSprite();
	RectF rcSelf = GetRect();
	RectF rcParent;
	while(sp)
	{
		rcParent = sp->GetRect();
		rcSelf.Offset(rcParent.X, rcParent.Y);
		sp = sp->GetParentSprite();
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
	LTK_CHECK_THREAD;
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
	LTK_CHECK_THREAD;
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
	LTK_CHECK_THREAD;
	LTK_D(Sprite);
	//LTK_LOG("SetWindow: %08x", wnd);
	d->window = wnd;
}

void SpritePrivate::HandlePaint(ID2D1RenderTarget *target)
{
	LTK_Q(Sprite);
	auto d = this;

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
        auto rcSprite = q_func()->GetClientRect();
        D2D1_RECT_F rcClip = D2D1RectF(rcSprite);
        target->PushAxisAlignedClip(rcClip, D2D1_ANTIALIAS_MODE_ALIASED);
	}
    PaintEvent ev;
    ev.id = ePaint;
    ev.target = target;
	q_func()->OnPaint(&ev);

    for (size_t i = 0; i < q->GetChildCount(); i++) {
        auto obj = q->GetChildAt(i);
		if (obj->IsSpriteType()) {
			auto sp = static_cast<Sprite *>(obj);
			RectF rc2 = sp->GetRect();

			TranslateTransform(target, rc2.X, rc2.Y);
			sp->d_func()->HandlePaint(target);
			TranslateTransform(target, -rc2.X, -rc2.Y);
		}
	}

	if (d->bClipChildren)
	{
        target->PopAxisAlignedClip();
	}
}

void SpritePrivate::HandleKeyEvent(UINT message, DWORD keyCode, DWORD flag)
{
	KeyEvent ev;
    ev.keyCode = keyCode;
    ev.flag = flag;

	switch(message)
	{
	case WM_KEYDOWN:
        ev.id = eKeyDown;
		q_func()->OnEvent(&ev);
		break;
	case WM_KEYUP:
        ev.id = eKeyUp;
		q_func()->OnEvent(&ev);
		break;
	case WM_CHAR:
        ev.id = eCharInput;
		q_func()->OnEvent(&ev);
		break;
	}
}

void SpritePrivate::HandleImeInput(LPCTSTR text)
{
    ImeEvent ev;
    ev.id = eImeInput;
    ev.text = text;
	q_func()->OnEvent(&ev);
}

// return weak ref
Window * Sprite::GetWindow()
{
	LTK_CHECK_THREAD;
	LTK_D(Sprite);
	Sprite *sp = this;
	while (sp->GetParentSprite())
	{
		sp = sp->GetParentSprite();
	}
	return sp->d_func()->window;
}

void Sprite::SetCapture()
{
	LTK_CHECK_THREAD;
	auto wnd = GetWindow();
    LTK_ASSERT(wnd);
	if (wnd)
	{
		wnd->SetCapture(this);
	}
}

void Sprite::ReleaseCapture()
{
	LTK_CHECK_THREAD;
	auto wnd = GetWindow();
    LTK_ASSERT(wnd);
	if (wnd)
	{
		wnd->ReleaseCapture();
	}
}

bool Sprite::IsCapturing()
{
	LTK_CHECK_THREAD;
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
	LTK_CHECK_THREAD;
	LTK_D(Sprite);
	if (d->bVisible != v)
	{
		Invalidate();
	}
	d->bVisible = v;
}

bool Sprite::GetVisible()
{
	LTK_CHECK_THREAD;
	LTK_D(Sprite);
	return d->bVisible;
}

void Sprite::EnableClipChildren( bool bClip )
{
	LTK_CHECK_THREAD;
	LTK_D(Sprite);
	if (d->bClipChildren != bClip)
	{
		d->bClipChildren = bClip;
		Invalidate();
	}
}

bool Sprite::IsClipChildren()
{
	LTK_CHECK_THREAD;
	LTK_D(Sprite);
	return d->bClipChildren;
}

bool SpritePrivate::DispatchMouseEvent(MouseEvent *ev)
{
	LTK_Q(Sprite);
	if (!this->bVisible) {
		return false;
	}
    for (auto i = q->GetChildCount(); i > 0; i--) {
        auto obj = q->GetChildAt(i - 1);
		if (obj->IsSpriteType()) {
			auto sp = static_cast<Sprite *>(obj);
			auto rc = sp->GetRect();
			if (rc.Contains(ev->x, ev->y)) {
				MouseEvent ev2 = *ev;
				ev2.x -= rc.X;
				ev2.y -= rc.Y;
				if (sp->d_func()->DispatchMouseEvent(&ev2)) {
					return true;
				}
			}
		}
    }
	return q_func()->OnEvent(ev);
}

Sprite * Sprite::GetAncestor()
{
	LTK_CHECK_THREAD;
	LTK_D(Sprite);
	Sprite *sp = this;
	while (sp->GetParentSprite())
	{
		sp = sp->GetParentSprite();
	}
	return sp;
}

Sprite *Sprite::GetParentSprite()
{
	LTK_D(Sprite);
	if (GetParent() && GetParent()->IsSpriteType()) {
		return static_cast<Sprite *>(GetParent());
	}
	else {
		return nullptr;
	}
}

void Sprite::TrackMouseLeave()
{
	LTK_CHECK_THREAD;
	Window *wnd = GetWindow();
	if (wnd)
	{
		wnd->TrackMouseLeave(this);
	}
}

void Sprite::ShowCaret()
{
	LTK_CHECK_THREAD;
	GetWindow()->ShowCaret();
}

void Sprite::SetCaretPos(RectF rc)
{
	LTK_CHECK_THREAD;
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

void SpritePrivate::HandleRecreateResouce(ID2D1RenderTarget *target)
{
	LTK_Q(Sprite);

	for (size_t i = 0; i < q->GetChildCount(); i++) {
		auto obj = q->GetChildAt(i);
		if (obj->IsSpriteType()) {
			auto sp = static_cast<Sprite *>(obj);
			sp->d_func()->HandleRecreateResouce(target);
		}
    }
	q->OnRecreateResouce(target);
}

void SpritePrivate::HandleThemeChange()
{
	LTK_Q(Sprite);
	q->OnThemeChanged();
	for (size_t i = 0; i < q->GetChildCount(); i++) {
		auto obj = q->GetChildAt(i);
		if (obj->IsSpriteType()) {
			auto sp = static_cast<Sprite *>(obj);
			sp->d_func()->HandleThemeChange();
		}
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
	LTK_CHECK_THREAD;
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
	LTK_CHECK_THREAD;
	auto wnd = GetWindow();
	if (wnd) {
		if (wnd->GetFocusSprite() == this) {
			wnd->SetFocusSprite(nullptr);
		}
	}
}

LPCSTR Sprite::GetName()
{
	LTK_CHECK_THREAD;
	LTK_D(Sprite);
	return d->name;
}

void Sprite::SetName(LPCSTR n)
{
	LTK_CHECK_THREAD;
	LTK_D(Sprite);
	d->name = n;
}

} // namespace ltk