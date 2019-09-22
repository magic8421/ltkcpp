//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Window.h"
#include "Window_p.h"
#include "Common.h"
#include "Sprite.h"
#include "Sprite_p.h"
#include "ltk.h"
#include "StyleManager.h"
#include "WindowLayout.h"
#include "MenuBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

static const wchar_t * ClsName = L"ltk_cls";
enum { TIMER_ANIMATION = 100 };
static const long SYSBTN_WIDTH = 22;
static const long CAPTION_HEIGHT = 20 + 3;
static const long SYSICON_SIZE = 24;
static const long WINDOW_BORDER = 6;

WindowPrivate::WindowPrivate(Window *wnd) :
	ObjectPrivate(wnd),
	shadowLeft(ShadowFrame::eLeft),
	shadowTop(ShadowFrame::eTop),
	shadowRight(ShadowFrame::eRight),
	shadowBottom(ShadowFrame::eBottom) 
{
	rectComposition.left = 0;
	rectComposition.top = 0;
	rectComposition.right = 5;
	rectComposition.bottom = 20;

	sprite = new WindowLayout;
	sprite->SetWindow(wnd);

	caretHeight = 20;
}

Window::Window() : Object(new WindowPrivate(this))
{
}

Window::~Window(void)
{
	LTK_D(Window);

    if (d->sprite) {
        delete d->sprite;
    }

	d->spFocus = INVALID_POINTER(Sprite);
	d->spCapture = INVALID_POINTER(Sprite);
	d->spHover = INVALID_POINTER(Sprite);

	if (d->target) {
		d->target->Release();
    }
	d->target = INVALID_POINTER(ID2D1HwndRenderTarget);

	if (d->brush) {
		d->brush->Release();
    }
	d->brush = INVALID_POINTER(ID2D1SolidColorBrush);

	if (d->atlas) {
		d->atlas->Release();
    }
	d->atlas = INVALID_POINTER(ID2D1Bitmap);

	::DestroyWindow(d->hwnd);
}

void Window::Create(Window *parent, RectF rc)
{
	LTK_D(Window);

    HWND hParent = NULL;
    if (!parent)
    {
        hParent = ::GetDesktopWindow();
    }
    else
    {
		hParent = parent->d_func()->hwnd;
    }
    DWORD style = WS_VISIBLE;
    
    style |= WS_OVERLAPPEDWINDOW;

	d->shadowLeft.Create();
	d->shadowTop.Create();
	d->shadowRight.Create();
	d->shadowBottom.Create();
	d->ShowShadowFrame(d->bShadow);

    style |=  WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

    DipCoordToScreen(rc.X, rc.Y);
    DipCoordToScreen(rc.Width, rc.Height);
    ::CreateWindowEx(0, ClsName, L"", style,
        (int)rc.X, (int)rc.Y, (int)rc.Width, (int)rc.Height,
        hParent, NULL, HINST_THISCOMPONENT, this);
}

void Window::Create(Window *parent, SizeF size)
{
	POINT pt = { 0 };
	auto ret = ::GetCursorPos(&pt);
	LTK_ASSERT(ret);
	auto monitor = ::MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
	LTK_ASSERT(monitor);
	MONITORINFO info = { 0 };
	info.cbSize = sizeof(info);
	ret = ::GetMonitorInfoW(monitor, &info);
	LTK_ASSERT(monitor);
	const auto &rc = info.rcWork;
	RectF rcWnd;
	rcWnd.X = (rc.right - rc.left - size.Width) / 2.f + rc.left;
	rcWnd.Y = (rc.bottom - rc.top - size.Height) / 2.f + rc.top;
	rcWnd.Width = size.Width;
	rcWnd.Height = size.Height;
	this->Create(parent, rcWnd);
}

RectF Window::GetRect()
{
	LTK_D(Window);

    RectF rf;
    RECT rc;
	::GetWindowRect(d->hwnd, &rc);
    rf.X = (float)rc.left;
    rf.Y = (float)rc.top;
    rf.Width = (float)(rc.right - rc.left);
    rf.Height = (float)(rc.bottom - rc.top);
    return rf;
}

void Window::SetRect(RectF rc)
{
	LTK_D(Window);

	::MoveWindow(d->hwnd, (int)rc.X, (int)rc.Y, (int)rc.Width, (int)rc.Height, TRUE);
}

void Window::SetCaption(LPCWSTR text)
{
	LTK_D(Window);

	::SetWindowText(d->hwnd, text);
	d->sprite->SetCaptionText(text);
}

void Window::EnableShadow(bool b)
{
	LTK_D(Window);
	d->bShadow = b;
}

SizeF Window::GetClientSize()
{
	LTK_D(Window);

    RECT rc;
	::GetClientRect(d->hwnd, &rc);
    SizeF sf((float)rc.right - rc.left, (float)rc.bottom - rc.top);
    ScreenCoordToDip(sf.Width, sf.Height);
    return sf;
}

void Window::RegisterWndClass()
{
	WNDCLASS wc;
	wc.style         = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc   = WndProcStatic;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = HINST_THISCOMPONENT;
	wc.hIcon         = NULL;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;// (HBRUSH)(COLOR_WINDOW + 1); // TODO 改成NULL 防止系统去擦除背景(闪烁) 双缓冲 还有clip children clip sibling
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = ClsName;

	ATOM a = RegisterClass(&wc);
    assert(a);
}

void WindowPrivate::HandleMouseMessage(UINT message, WPARAM wparam, LPARAM lparam)
{
    //LTK_LOG("Mouse Message: %d %08x %d %d", message, wparam, (short)LOWORD(lparam), (short)HIWORD(lparam));
	MouseEvent ev;
	ev.id = this->TranslateMessageCode(message);
	ev.flag = LOWORD(wparam);

	if (WM_MOUSEWHEEL == message)
	{
		POINT pt;
		pt.x = (short)LOWORD(lparam);
		pt.y = (short)HIWORD(lparam);
		::ScreenToClient(this->hwnd, &pt);
		ev.x = (float)pt.x;
		ev.y = (float)pt.y;
		ev.delta = (float)(short)HIWORD(wparam) / 120.0F;
        //LOG("WM_MOUSEWHEEL: " << event.x << " " << event.y);
	}
	else
	{
		ev.x = (float)(short)LOWORD(lparam); //先切2字节 然后转有符号 再转浮点
		ev.y = (float)(short)HIWORD(lparam);
		ev.delta = 0.0f;
	}
    ScreenCoordToDip(ev.x, ev.y);

    if (WM_LBUTTONDBLCLK == message)
    {
		::PostMessage(this->hwnd, WM_LBUTTONDOWN, wparam, lparam);
    }

	if (this->spCapture)
	{
		RectF rc = this->spCapture->GetAbsRect();
		ev.x -= rc.X;
		ev.y -= rc.Y;
		this->spCapture->OnEvent(&ev);
	}
	else if (this->sprite)
	{
		//if (WM_LBUTTONDOWN == message)
		//{
		this->bEnableFocusChange = true;
		this->sprite->d_func()->DispatchMouseEvent(&ev);
			
			std::vector<Sprite *> defer_remove;
			defer_remove.reserve(20);
			for (std::unordered_set<Sprite *>::iterator iter = this->setTrackMouseLeave.begin();
				iter != this->setTrackMouseLeave.end(); ++iter)
			{
				Sprite *sp = *iter;
				RectF rc = sp->GetAbsRect();
				if (!rc.Contains(ev.x, ev.y))
				{
					MouseEvent e2 = ev;
					e2.id = eMouseLeave;
					sp->OnEvent(&e2);
					defer_remove.push_back(sp);
					// Fire the event and remove sp from the set;
				}
			}
			for (auto sp: defer_remove)
			{
				this->setTrackMouseLeave.erase(sp);
			}
			if (this->spFocus && WM_LBUTTONDOWN == message && this->bEnableFocusChange) {
				auto arc = this->spFocus->GetAbsRect();
				if (!arc.Contains(ev.x, ev.y)){
					FocusEvent ev2;
					ev2.id = eKillFocus;
					this->spFocus->OnEvent(&ev2);
					this->spFocus = nullptr;
				}
			}
		//}
	}
}

void WindowPrivate::HandleMouseLeave()
{
	for (auto iter = this->setTrackMouseLeave.begin(); iter != this->setTrackMouseLeave.end(); ++iter)
    {
        Sprite *sp = *iter;
        MouseEvent e2;
        e2.id = eMouseLeave;
        e2.delta = 0;
        e2.flag = 0;
        e2.x = 0;
        e2.y = 0;
        sp->OnEvent(&e2);
        // Fire the event and remove sp from the set;
    }
	this->setTrackMouseLeave.clear();
}

LRESULT WindowPrivate::HandleNcHitTest(const POINT &pt)
{
    //LTK_LOG("WM_NCHITTEST %d %d", pt.x, pt.y);
    const long margin = 7;
    RECT rcWnd;
	::GetClientRect(this->hwnd, &rcWnd);
    const long width = rcWnd.right - rcWnd.left;
    const long height = rcWnd.bottom - rcWnd.top;
	auto rcCaption = DipRectToScreen(this->sprite->GetCaptionRect());
    
	long caption_h = 35;

    if (pt.x < margin && pt.y < margin) {
        return HTTOPLEFT;
    }
    if (pt.x < margin && pt.y > height - margin) {
        return HTBOTTOMLEFT;
    }
    if (pt.x > width - margin && pt.y < margin) {
        return HTTOPRIGHT;
    }
    if (pt.x > width - margin && pt.y > height - margin) {
        return HTBOTTOMRIGHT;
    }
    if (pt.x < margin) {
        return HTLEFT;
    }
    if (pt.y < margin) {
        return HTTOP;
    }
    if (pt.x > width - margin) {
        return HTRIGHT;
    }
    if (pt.y > height - margin) {
        return HTBOTTOM;
    }
    //if (pt.y < caption_h && pt.x < 30) {
    //    return HTSYSMENU;
    //}
    if (::PtInRect(&rcCaption, pt)) {
        return HTCAPTION;
    }
    return HTCLIENT;
}

LRESULT Window::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	LTK_D(Window);

    POINT pt;
    switch (message)
    {
    case WM_PAINT:
        OnPaint(hwnd);
        break;
    case WM_NCHITTEST:
        pt.x = (short)LOWORD(lparam);
        pt.y = (short)HIWORD(lparam);
        ::ScreenToClient(hwnd, &pt);
        return d->HandleNcHitTest(pt);
    case WM_NCCALCSIZE:
        if (wparam) {
            return 0;
        }
        break;
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_LBUTTONDBLCLK:
    case WM_MOUSEWHEEL:
		d->HandleMouseMessage(message, wparam, lparam);
        break;
    case WM_ERASEBKGND:
        return TRUE;
    case WM_TIMER:
		if (wparam == TIMER_ANIMATION)
        {
            static int cnt = 0;
            cnt++;
            if (cnt % 100 == 0){
                LTK_LOG("TIMER_ANIMATION %d", cnt);
            }
            ::InvalidateRect(hwnd, NULL, FALSE);
        }
        return 0;
    case WM_SIZE:
        do
        {
            UINT cx = LOWORD(lparam);
            UINT cy = HIWORD(lparam);

			if (d->target)
            {
				d->target->Resize(D2D1::SizeU(cx, cy));
            }
            OnSize((float)cx, (float)cy, (DWORD)wparam);
			d->UpdateShadowFrame(true);

            //LTK_LOG("WM_SIZE %d", wparam);
            if (wparam == SIZE_MAXIMIZED) {
				d->sprite->DoLayout();
            }
            else if (wparam == SIZE_RESTORED){
				d->sprite->DoLayout();
            }
            else if (wparam == SIZE_MINIMIZED) {
				d->setAnimation.clear();
                ::ReleaseCapture();
				::KillTimer(hwnd, TIMER_ANIMATION);
                LTK_LOG("WM_SIZE KillTimer");
            }
        } while (0);
        return 0;
    case WM_MOVE:
		d->UpdateShadowFrame(false);
        break;
    case WM_ACTIVATE:
        if (LOWORD(wparam)) {
			d->UpdateShadowFrame(false);
        }
        break;
        // TODO hide shadow when maximized or hide main window.
    case WM_MOUSELEAVE:
		d->HandleMouseLeave();
        break;
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_CHAR:
		if (d->spFocus)
        {
			d->spFocus->d_func()->HandleKeyEvent(message, (DWORD)wparam, (DWORD)lparam);
        }
        break;
    case WM_IME_SETCONTEXT:
    case WM_IME_STARTCOMPOSITION:
    case WM_IME_COMPOSITION:
    case WM_IME_ENDCOMPOSITION:
    case WM_INPUTLANGCHANGE:
		return d->OnImeEvent(message, wparam, lparam);
    case WM_SETFOCUS:
        //LOG("WM_SETFOCUS");
		::CreateCaret(hwnd, NULL, 1, d->caretHeight);
        //::ShowCaret(hwnd);
		if (d->spFocus)
        {
			FocusEvent ev;
            ev.id = eSetFocus;
			d->spFocus->OnEvent(&ev);
        }
        return 0;
    case WM_KILLFOCUS:
        //LOG("WM_KILLFOCUS");
        ::DestroyCaret();
		if (d->spFocus)
        {
			FocusEvent ev;
            ev.id = eKillFocus;
			d->spFocus->OnEvent(&ev);
        }
        LTK_LOG("WM_KILLFOCUS KillTimer");
		KillTimer(hwnd, TIMER_ANIMATION);
		d->setAnimation.clear();
        ::ReleaseCapture();
        return 0;
    case WM_SYSCOMMAND:
        //LTK_LOG("WM_SYSCOMMAND %08X %08X", wparam, lparam);
        break;
    case WM_WINDOWPOSCHANGING:
        do
        {
            WINDOWPOS *wp = (WINDOWPOS *)lparam;
            //LTK_LOG("WM_WINDOWPOSCHANGING %08X", wp->flags);
        } while (0);
        break;
    case WM_CREATE:
        // Force WM_NCCALCSIZE
        SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
        LTK_LOG("window created: 0x%08X", hwnd);
        //do 
        //{
        //	RECT rc;
        //	::GetClientRect(hwnd, &rc);
        //	::PostMessage(hwnd, WM_SIZE, 0, MAKELPARAM(rc.right, rc.bottom)); // TODO 这里如果创建出来的是全屏的 第2个参数怎么办呢
        //} while (0);
        return 0;
    case WM_CLOSE:
        this->CloseWindow();
        return 0;
    case WM_DESTROY:
        OnDestroy();
        //CallEventHandler(L, "OnDestroy", 0, 0);
		d->shadowLeft.Destroy();
		d->shadowTop.Destroy();
		d->shadowRight.Destroy();
		d->shadowBottom.Destroy();
        return 0;
    }
    return ::DefWindowProc(hwnd, message, wparam, lparam);
}

LRESULT CALLBACK Window::WndProcStatic(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	Window *thiz = nullptr;

	if (WM_NCCREATE == message)
	{
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lparam);
        thiz = reinterpret_cast<Window*>(lpcs->lpCreateParams);
		thiz->d_func()->hwnd = hwnd;
		SetWindowLongPtr(hwnd, GWLP_USERDATA,
			reinterpret_cast<LPARAM>(thiz));
	}
	else if (WM_GETMINMAXINFO == message)
	{
        MINMAXINFO* mmi = (MINMAXINFO*)lparam;
        //auto ret = ::DefWindowProc(hwnd, message, wparam, lparam);
        //LTK_LOG("WM_GETMINMAXINFO min %d %d max %d %d",
        //    mmi->ptMinTrackSize.x,
        //    mmi->ptMinTrackSize.y,
        //    mmi->ptMaxTrackSize.x,
        //    mmi->ptMaxTrackSize.y);
        POINT pt;
        ::GetCursorPos(&pt);
        HMONITOR mon = ::MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
        MONITORINFO info = { 0 };
        info.cbSize = sizeof(info);
        ::GetMonitorInfoW(mon, &info);
        mmi->ptMaxTrackSize.x = info.rcWork.right - info.rcWork.left;
        mmi->ptMaxTrackSize.y = info.rcWork.bottom - info.rcWork.top;
        mmi->ptMaxPosition.x = 0;
        mmi->ptMaxPosition.y = 0;
        return 0;
	} else {
        thiz = reinterpret_cast<Window *>
            (GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}
	if (!thiz) {
        LTK_LOG("WndProc thiz is NULL, message: %d", message);
        return ::DefWindowProc(hwnd, message, wparam, lparam);
	}
	if (WM_NCDESTROY == message) {
		thiz->d_func()->hwnd = 0;
		return 0;
	}
    return thiz->WndProc(hwnd, message, wparam, lparam);
}

void WindowPrivate::DrawNonClient()
{
    SizeF size = q_func()->GetClientSize();
    RectF rc(0, 0, size.Width, size.Height);
	if (this->background) {
		this->background->Draw(q_func(), this->target, rc, AbstractBackground::Normal, 1.0f);
	}
    //DrawTextureNineInOne(
    //    this->target,
    //    this->GetAtlasBitmap(),
    //    this->background.atlas,
    //    this->background.margin,
    //    rc,
    //    1.0f,
    //    this->background.scale);
}

ID2D1SolidColorBrush *Window::GetStockBrush()
{
	LTK_D(Window);
	return d->brush;
}

void WindowPrivate::RecreateResouce()
{
	HRESULT hr = E_FAIL;
	SAFE_RELEASE(this->brush);
	hr = this->target->CreateSolidColorBrush(D2D1::ColorF(0.5f, 0.5f, 0.5f), &this->brush);
    LTK_ASSERT(SUCCEEDED(hr));

	SAFE_RELEASE(this->atlas);
	hr = LoadBitmapFromFile(this->target, L"res\\atlas.png", &this->atlas);
    LTK_ASSERT(SUCCEEDED(hr));
}

void Window::OnPaint(HWND hwnd )
{
	LTK_D(Window);
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(hwnd, &ps);
    HRESULT hr = E_FAIL;

	if (!d->target)
    {
        LTK_LOG("RecreateResouce %d", ltk::TickCount());

        RECT rc;
        ::GetClientRect(hwnd, &rc);

        hr = GetD2DFactory()->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(
			hwnd, D2D1::SizeU(rc.right, rc.bottom)), &d->target);
        assert(SUCCEEDED(hr));

		if (d->sprite)
        {
			d->sprite->d_func()->HandleRecreateResouce(d->target);
        }
        d->RecreateResouce();
    }

	d->target->BeginDraw();
	d->target->SetTransform(D2D1::Matrix3x2F::Identity());
    //TranslateTransform(d->target, 0.5f, 0.5f);
    //d->target->Clear(StyleManager::Instance()->GetColor(StyleManager::clrBackground));
	d->target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
    d->DrawNonClient();

	if (d->sprite)
    {
		RectF rc = d->sprite->GetRect();
		TranslateTransform(d->target, rc.X, rc.Y);
		d->sprite->d_func()->HandlePaint(d->target);
		TranslateTransform(d->target, -rc.X, -rc.Y);
    }

	hr = d->target->EndDraw();
    if (hr == D2DERR_RECREATE_TARGET)
    {
        hr = S_OK;
        LTK_LOG("release the render target.");
		SAFE_RELEASE(d->target);
    }
    else if (FAILED(hr)) {
        LTK_LOG("EndDraw failed: %d", hr);
    }
    ::EndPaint(hwnd, &ps);
}

bool Window::OnSize(float cx, float cy, DWORD flag)
{
	LTK_D(Window);
	if (d->sprite) {
		ScreenCoordToDip(cx, cy);
		d->sprite->SetRect(RectF(1.0f, 1.0f, (float)(cx - 2.0f), (float)(cy - 1.0f)));
	}
    return false;
}

void Window::CloseWindow()
{
	LTK_D(Window);
	bool proceed = false;
    OnClose(proceed);
    if (proceed) {
		::DestroyWindow(d->hwnd);
    }
}

void Window::Minimize()
{
	LTK_D(Window);
	::ShowWindow(d->hwnd, SW_MINIMIZE);
}

void Window::Maximize()
{
	LTK_D(Window);
	WINDOWPLACEMENT wp = { 0 };
    wp.length = sizeof(wp);
	::GetWindowPlacement(d->hwnd, &wp);
    if (wp.showCmd == SW_MAXIMIZE) {
		::ShowWindow(d->hwnd, SW_RESTORE);
    } else {
		::ShowWindow(d->hwnd, SW_MAXIMIZE);
    }
}

bool Window::OnClose(bool &proceed)
{
	LTK_D(Window);
	proceed = true;
    d->CloseDelegate.Invoke(std::ref(proceed));
    return proceed;
}

void Window::OnDestroy()
{ 
}

HWND Window::Handle()
{
	LTK_D(Window);
	return d->hwnd;
}

LRESULT WindowPrivate::OnImeEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// TODO 加个开关 能够关闭输入法 当焦点不在输入框上的时候就关闭
	switch(uMsg)
	{
	case WM_IME_SETCONTEXT:
		do 
		{
			//LOG(<<"WM_IME_SETCONTEXT");
			BOOL handled = FALSE;
			this->ime.CreateImeWindow(this->hwnd);
			this->ime.CleanupComposition(this->hwnd);
			this->ime.SetImeWindowStyle(this->hwnd, uMsg, wParam, lParam, &handled);
		} while (0);
		return 0;
	case WM_IME_STARTCOMPOSITION:
		//LOG(<<"WM_IME_STARTCOMPOSITION");
		this->ime.CreateImeWindow(this->hwnd);
		this->ime.ResetComposition(this->hwnd);
		return 0;
	case WM_IME_COMPOSITION:
		do
		{
			//LOG(<<"WM_IME_COMPOSITION");
			ImeComposition comp;
			this->ime.UpdateImeWindow(this->hwnd);
			this->ime.GetResult(this->hwnd, lParam, &comp);
			if (GCS_RESULTSTR == comp.string_type)
			{
				wstring tmp = comp.ime_string; // 这里结尾居然多出个0 可能是改了谷歌的代码导致的问题
				tmp.resize(wcslen(tmp.c_str()));
				//LOGW(<<tmp); // TODO wide version of LTK_LOG
				q_func()->OnImeInput(tmp.c_str());
				//this->text += tmp;
				//::InvalidateRect(GetHWND(), NULL, TRUE);
			}
			this->ime.ResetComposition(this->hwnd);
			this->ime.EnableIME(this->hwnd, this->rectComposition, false); // 输入窗口跟随
		}while(0);
		return 0;
	case WM_IME_ENDCOMPOSITION:
		//LOG(<<"WM_IME_ENDCOMPOSITION");
		this->ime.ResetComposition(this->hwnd);
		this->ime.DestroyImeWindow(this->hwnd);
		//::ShowCaret(this->hwnd);
		::DefWindowProc(this->hwnd, uMsg, wParam, lParam);
		return 0;
	case WM_INPUTLANGCHANGE:
		//LOG(<<"WM_INPUTLANGCHANGE");
		this->ime.SetInputLanguage();
		::DefWindowProc(this->hwnd, uMsg, wParam, lParam);
		return 0;
	case WM_KEYDOWN:
		return 0;
	}
	return 0;
}

void Window::SetImePosition( float x, float y )
{
	LTK_D(Window);
	d->rectComposition.left = (int)x;
	d->rectComposition.right = (int)x + 5;
	d->rectComposition.top = (int)y;
	d->rectComposition.bottom = (int)y + 20;
}

Sprite *Window::GetRootSprite()
{
	LTK_D(Window);
	return d->sprite;
}

Sprite *Window::SetClientSprite(Sprite *sp)
{
	LTK_D(Window);
	return d->sprite->SetClientSprite(sp);
}

MenuBar *Window::SetMenu(MenuBar *m)
{
	LTK_D(Window);
	return d->sprite->SetMenuBar(m);
}

MenuBar * Window::GetMenu()
{
	LTK_D(Window);
	return d->sprite->GetMenuBar();
}

void Window::SetFocusSprite( Sprite *sp )
{
	LTK_D(Window);
	if (d->spFocus == sp)
	{
		return;
	}
	if (d->spFocus)
	{
		FocusEvent ev;
        ev.id = eKillFocus;
		d->spFocus->OnEvent(&ev);
	}
	d->spFocus = sp;
	if (d->spFocus)
	{
		FocusEvent ev;
        ev.id = eSetFocus;
		d->spFocus->OnEvent(&ev);
	}
}

Sprite *Window::GetFocusSprite()
{
	LTK_D(Window);
	return d->spFocus;
}

void Window::DisableFocusChange()
{
	LTK_D(Window);
	d->bEnableFocusChange = false;
}

// TODO change to private?
void Window::OnImeInput( PCTSTR text )
{
	LTK_D(Window);
	if (d->spFocus)
	{
		// 其他的Sprite也有可能去接受ime消息。比如再来一个RichEdit
		d->spFocus->d_func()->HandleImeInput(text);
	}
}

void Window::SetCaretHeight( float h)
{
	LTK_D(Window);
	d->caretHeight = (int)h;
}

void Window::SetCapture( Sprite *sp )
{
	LTK_D(Window);
	LTK_ASSERT(sp->GetWindow() == this);
	d->spCapture = sp;
	LTK_ASSERT(::IsWindow(d->hwnd));
	auto hwnd = ::SetCapture(d->hwnd);
    //LTK_LOG("old capture %p", hwnd);
}

void Window::ReleaseCapture()
{
	LTK_D(Window);
	d->spCapture = NULL;
	::ReleaseCapture();
    //LTK_LOG("ReleaseCapture");
}

bool Window::IsCapturing(Sprite *sp)
{
	LTK_D(Window);
	return d->spCapture == sp;
}

void Window::ShowCaret()
{
	LTK_D(Window);
	LTK_LOG("ShowCaret");
	::ShowCaret(d->hwnd);
}

void Window::HideCaret()
{
	LTK_D(Window);
	LTK_LOG("HideCaret");
	::HideCaret(d->hwnd);
}

void Window::TrackMouseLeave( Sprite *sp )
{
	LTK_D(Window);
	if (sp->GetWindow() == this)
	{
		d->setTrackMouseLeave.insert(sp);
	}
	// Track for the HWND
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(tme);
	tme.hwndTrack = d->hwnd;
    tme.dwFlags = TME_LEAVE;
    tme.dwHoverTime = HOVER_DEFAULT;
    ::TrackMouseEvent(&tme);
}

void Window::BeginAnimation(Sprite *sp)
{
	LTK_D(Window);
	//if (!::IsIconic(d->hwnd)) {
	if (d->setAnimation.size() == 0)
        {
			::SetTimer(d->hwnd, TIMER_ANIMATION, 0, NULL);
        }
	d->setAnimation.insert(sp);
    //}
}

void Window::EndAnimation(Sprite *sp)
{
	LTK_D(Window);
	if (d->setAnimation.size() == 0)
    {
		::KillTimer(d->hwnd, TIMER_ANIMATION);
        return;
    }
	auto iter = d->setAnimation.find(sp);
	if (iter != d->setAnimation.end())
    {
		d->setAnimation.erase(iter);
        if (d->setAnimation.size() == 0)
        {
			::KillTimer(d->hwnd, TIMER_ANIMATION);
        }
    }
}

ID2D1Bitmap *Window::GetAtlasBitmap()
{
	LTK_D(Window);
	return d->atlas;
}

void Window::SetBackground(LPCSTR style)
{
	LTK_D(Window);
	d->background = StyleManager::Instance()->GetBackground(style);
	d->styleName = style;
}

void Window::UpdateTheme()
{
	LTK_D(Window);
	d->background = StyleManager::Instance()->GetBackground(d->styleName.c_str());
    this->OnThemeChanged();
	if (d->sprite) {
		d->sprite->d_func()->HandleThemeChange();
		// TODO send a eSizeChanged to the Sprite tree.
		// let the controls use the new style measure.
		float cx, cy;
		RECT rc;
		::GetClientRect(d->hwnd, &rc);
		cx = (float)rc.right;
		cy = (float)rc.bottom;
		ltk::ScreenCoordToDip(cx, cy);
		SizeEvent ev;
		ev.id = eSizeChanged;
		ev.width = cx;
		ev.height = cy;
		d->sprite->OnEvent(&ev);
	}
}

Cookie Window::AttachCloseDelegate(const std::function<void(bool &)> &cb)
{
	LTK_D(Window);
	return d->CloseDelegate.Attach(cb);
}

void Window::RemoveCloseDelegate(Cookie c)
{
	LTK_D(Window);
	d->CloseDelegate.Remove(c);
}

void WindowPrivate::ShowShadowFrame(bool show)
{
	this->shadowLeft.Show(show);
	this->shadowTop.Show(show);
	this->shadowRight.Show(show);
	this->shadowBottom.Show(show);
}

void WindowPrivate::UpdateShadowFrame(bool bRedraw)
{
	HDWP hdwp = ::BeginDeferWindowPos(4);
	this->shadowLeft.Update(this->hwnd, hdwp, bRedraw);
	this->shadowTop.Update(this->hwnd, hdwp, bRedraw);
	this->shadowRight.Update(this->hwnd, hdwp, bRedraw);
	this->shadowBottom.Update(this->hwnd, hdwp, bRedraw);
    BOOL ret = ::EndDeferWindowPos(hdwp);
    LTK_ASSERT(ret);
}

Events WindowPrivate::TranslateMessageCode(UINT message)
{
	switch (message) {
	case WM_MOUSEMOVE:
		return eMouseMove;
	case WM_MOUSEWHEEL:
		return eMouseWheel;
	case WM_LBUTTONDOWN:
		return eLBtnDown;
	case WM_LBUTTONUP:
		return eLBtnUp;
	case WM_LBUTTONDBLCLK:
		return eLBtnDbClick;
	case WM_MOUSELEAVE:
		return eMouseLeave;
	case WM_RBUTTONDOWN:
		return eRBtnDown;
	case WM_RBUTTONUP:
		return eRBtnDown;
	default:
		LTK_ASSERT(false);
		break;
	}
	return (Events)0;
}

} // namespace ltk