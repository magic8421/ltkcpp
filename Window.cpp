//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Window.h"
#include "Common.h"
#include "Sprite.h"
#include "ltk.h"
#include "StyleManager.h"
#include "WindowLayout.h"
#include "MenuBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

const wchar_t * Window::ClsName = L"ltk_cls";
static const long SYSBTN_WIDTH = 22;
static const long CAPTION_HEIGHT = 20 + 3;
static const long SYSICON_SIZE = 24;
static const long WINDOW_BORDER = 6;

Window::Window() :
m_shadowLeft(ShadowFrame::eLeft),
m_shadowTop(ShadowFrame::eTop),
m_shadowRight(ShadowFrame::eRight),
m_shadowBottom(ShadowFrame::eBottom)
{
	m_rectComposition.left = 0;
	m_rectComposition.top = 0;
	m_rectComposition.right = 5;
	m_rectComposition.bottom = 20;

    m_root = new WindowLayout;
    m_root->SetWindow(this);
    
	m_caretHeight = 20;

	SetBackground("window_bg");
}

Window::~Window(void)
{
	::DestroyWindow(m_hwnd);

    if (m_root) {
        delete m_root;
    }

    m_spFocus = INVALID_POINTER(Widget);
    m_spCapture = INVALID_POINTER(Widget);
    m_spHover = INVALID_POINTER(Widget);

    if (m_target) {
        m_target->Release();
    }
    m_target = INVALID_POINTER(ID2D1HwndRenderTarget);

    if (m_brush) {
        m_brush->Release();
    }
    m_brush = INVALID_POINTER(ID2D1SolidColorBrush);

    if (m_atlas) {
        m_atlas->Release();
    }
    m_atlas = INVALID_POINTER(ID2D1Bitmap);
}

void Window::Create(Window *parent, RectF rc)
{
    HWND hParent = NULL;
    if (!parent)
    {
        hParent = ::GetDesktopWindow();
    }
    else
    {
        hParent = parent->m_hwnd;
    }
    DWORD style = WS_VISIBLE;
    
    style |= WS_OVERLAPPEDWINDOW;

    m_shadowLeft.Create();
    m_shadowTop.Create();
    m_shadowRight.Create();
    m_shadowBottom.Create();

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
    RectF rf;
    RECT rc;
    ::GetWindowRect(m_hwnd, &rc);
    rf.X = (float)rc.left;
    rf.Y = (float)rc.top;
    rf.Width = (float)(rc.right - rc.left);
    rf.Height = (float)(rc.bottom - rc.top);
    return rf;
}

void Window::SetRect(RectF rc)
{
    ::MoveWindow(m_hwnd, (int)rc.X, (int)rc.Y, (int)rc.Width, (int)rc.Height, TRUE);
}

void Window::SetCaption(LPCWSTR text)
{
    ::SetWindowText(m_hwnd, text);
    m_root->SetCaptionText(text);
}

SizeF Window::GetClientSize()
{
    RECT rc;
    ::GetClientRect(m_hwnd, &rc);
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

void Window::HandleMouseMessage(UINT message, WPARAM wparam, LPARAM lparam)
{
    //LTK_LOG("Mouse Message: %d %08x %d %d", message, wparam, (short)LOWORD(lparam), (short)HIWORD(lparam));
    MouseEvent ev;
	ev.id = TranslateMessageCode(message);
	ev.flag = LOWORD(wparam);

	if (WM_MOUSEWHEEL == message)
	{
		POINT pt;
		pt.x = (short)LOWORD(lparam);
		pt.y = (short)HIWORD(lparam);
		::ScreenToClient(m_hwnd, &pt);
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
        ::PostMessage(m_hwnd, WM_LBUTTONDOWN, wparam, lparam);
    }

	if (m_spCapture)
	{
		RectF rc = m_spCapture->GetAbsRect();
		ev.x -= rc.X;
		ev.y -= rc.Y;
		m_spCapture->OnEvent(&ev);
	}
	else if (m_root)
	{
		//if (WM_LBUTTONDOWN == message)
		//{
			m_bEnableFocusChange = true;
			m_root->DispatchMouseEvent(&ev);
			
			std::vector<Widget*> defer_remove;
			defer_remove.reserve(20);
			for (std::unordered_set<Widget*>::iterator iter = m_setTrackMouseLeave.begin();
				iter != m_setTrackMouseLeave.end(); ++iter)
			{
                Widget *sp = *iter;
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
				m_setTrackMouseLeave.erase(sp);
			}
			if (m_spFocus && WM_LBUTTONDOWN == message && m_bEnableFocusChange) {
				auto arc = m_spFocus->GetAbsRect();
				if (!arc.Contains(ev.x, ev.y)){
					FocusEvent ev2;
					ev2.id = eKillFocus;
					m_spFocus->OnEvent(&ev2);
					m_spFocus = nullptr;
				}
			}
		//}
	}
}

void Window::HandleMouseLeave()
{
    for (auto iter = m_setTrackMouseLeave.begin(); iter != m_setTrackMouseLeave.end(); ++iter)
    {
        Widget *sp = *iter;
        MouseEvent e2;
        e2.id = eMouseLeave;
        e2.delta = 0;
        e2.flag = 0;
        e2.x = 0;
        e2.y = 0;
        sp->OnEvent(&e2);
        // Fire the event and remove sp from the set;
    }
    m_setTrackMouseLeave.clear();
}

LRESULT Window::HandleNcHitTest(const POINT &pt)
{
    //LTK_LOG("WM_NCHITTEST %d %d", pt.x, pt.y);
    const long margin = 7;
    RECT rcWnd;
    ::GetClientRect(m_hwnd, &rcWnd);
    const long width = rcWnd.right - rcWnd.left;
    const long height = rcWnd.bottom - rcWnd.top;
    auto rcCaption = DipRectToScreen(m_root->GetCaptionRect());
    
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
        return HandleNcHitTest(pt);
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
        HandleMouseMessage(message, wparam, lparam);
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
        do {
            UINT cx = LOWORD(lparam);
            UINT cy = HIWORD(lparam);

            if (wparam == SIZE_MAXIMIZED || wparam == SIZE_RESTORED) {
                if (m_target) {
                    m_target->Resize(D2D1::SizeU(cx, cy));
                }
                OnSize((float)cx, (float)cy, (DWORD)wparam);
                UpdateShadowFrame(true);
            }
            else if (wparam == SIZE_MINIMIZED) {
                m_setAnimation.clear();
                ::ReleaseCapture();
                ::KillTimer(hwnd, TIMER_ANIMATION);
                ::timeEndPeriod(m_timerResolution);
                LTK_LOG("WM_SIZE KillTimer");
            }
        } while (0);
        return 0;
    case WM_MOVE:
        UpdateShadowFrame(false);
        break;
    case WM_ACTIVATE:
        if (LOWORD(wparam)) {
            UpdateShadowFrame(false);
        }
        break;
        // TODO hide shadow when maximized or hide main window.
    case WM_MOUSELEAVE:
        HandleMouseLeave();
        break;
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_CHAR:
        if (m_spFocus)
        {
            m_spFocus->HandleKeyEvent(message, (DWORD)wparam, (DWORD)lparam);
        }
        break;
    case WM_IME_SETCONTEXT:
    case WM_IME_STARTCOMPOSITION:
    case WM_IME_COMPOSITION:
    case WM_IME_ENDCOMPOSITION:
    case WM_INPUTLANGCHANGE:
        return OnImeEvent(message, wparam, lparam);
    case WM_SETFOCUS:
        //LOG("WM_SETFOCUS");
        ::CreateCaret(hwnd, NULL, 1, m_caretHeight);
        //::ShowCaret(hwnd);
        if (m_spFocus)
        {
			FocusEvent ev;
            ev.id = eSetFocus;
            m_spFocus->OnEvent(&ev);
        }
        return 0;
    case WM_KILLFOCUS:
        //LOG("WM_KILLFOCUS");
        ::DestroyCaret();
        if (m_spFocus)
        {
			FocusEvent ev;
            ev.id = eKillFocus;
            m_spFocus->OnEvent(&ev);
        }
        LTK_LOG("WM_KILLFOCUS KillTimer");
        KillTimer(hwnd, TIMER_ANIMATION);
        ::timeEndPeriod(m_timerResolution);
        m_setAnimation.clear();
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
        m_shadowLeft.Destroy();
        m_shadowTop.Destroy();
        m_shadowRight.Destroy();
        m_shadowBottom.Destroy();
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
		thiz->m_hwnd = hwnd;
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
		thiz->m_hwnd = 0;
		return 0;
	}
    return thiz->WndProc(hwnd, message, wparam, lparam);
}

void Window::DrawNonClient()
{
    SizeF size = this->GetClientSize();
    RectF rc(0, 0, size.Width, size.Height);
	if (m_background) {
		m_background->Draw(this, m_target, rc, AbstractBackground::Normal, 1.0f);
	}
    //DrawTextureNineInOne(
    //    m_target,
    //    this->GetAtlasBitmap(),
    //    m_background.atlas,
    //    m_background.margin,
    //    rc,
    //    1.0f,
    //    m_background.scale);
}

ID2D1SolidColorBrush *Window::GetStockBrush()
{
    return m_brush;
}

void Window::RecreateResouce()
{
    HRESULT hr = E_FAIL;
    SAFE_RELEASE(m_brush);
    hr = m_target->CreateSolidColorBrush(D2D1::ColorF(0.5f, 0.5f, 0.5f), &m_brush);
    LTK_ASSERT(SUCCEEDED(hr));

    SAFE_RELEASE(m_atlas);
    hr = LoadBitmapFromFile(m_target, L"res\\atlas.png", &m_atlas);
    LTK_ASSERT(SUCCEEDED(hr));
}

void Window::OnPaint(HWND hwnd )
{
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(hwnd, &ps);
    HRESULT hr = E_FAIL;

    if (!m_target)
    {
        LTK_LOG("RecreateResouce %d", ltk::TickCount());

        RECT rc;
        ::GetClientRect(hwnd, &rc);

        hr = GetD2DFactory()->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(
            hwnd, D2D1::SizeU(rc.right, rc.bottom)), &m_target);
        assert(SUCCEEDED(hr));

        if (m_root)
        {
            m_root->HandleRecreateResouce(m_target);
        }
        this->RecreateResouce();
    }

    m_target->BeginDraw();
    m_target->SetTransform(D2D1::Matrix3x2F::Identity());
    //TranslateTransform(m_target, 0.5f, 0.5f);
    //m_target->Clear(StyleManager::Instance()->GetColor(StyleManager::clrBackground));
    m_target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
    this->DrawNonClient();

    if (m_root)
    {
        RectF rc = m_root->GetRect();
        TranslateTransform(m_target, rc.X, rc.Y);
        m_root->HandlePaint(m_target);
        TranslateTransform(m_target, -rc.X, -rc.Y);
    }

    hr = m_target->EndDraw();
    if (hr == D2DERR_RECREATE_TARGET)
    {
        hr = S_OK;
        LTK_LOG("release the render target.");
        SAFE_RELEASE(m_target);
    }
    else if (FAILED(hr)) {
        LTK_LOG("EndDraw failed: %d", hr);
    }
    ::EndPaint(hwnd, &ps);
}

bool Window::OnSize(float cx, float cy, DWORD flag)
{
	if (m_root) {
		ScreenCoordToDip(cx, cy);
		m_root->SetRect(RectF(1.0f, 1.0f, (float)(cx - 2.0f), (float)(cy - 1.0f)));
	}
    return false;
}

void Window::CloseWindow()
{
    BOOL proceed = TRUE;
    OnClose(&proceed);
    if (proceed) {
        ::DestroyWindow(m_hwnd);
    }
}

void Window::Minimize()
{
    ::ShowWindow(m_hwnd, SW_MINIMIZE);
}

void Window::Maximize()
{
    WINDOWPLACEMENT wp = { 0 };
    wp.length = sizeof(wp);
    ::GetWindowPlacement(m_hwnd, &wp);
    if (wp.showCmd == SW_MAXIMIZE) {
        ::ShowWindow(m_hwnd, SW_RESTORE);
    } else {
        ::ShowWindow(m_hwnd, SW_MAXIMIZE);
    }
}



typedef void(CALLBACK *WindowCloseCallback)(void *userdata, BOOL *pProceed, BOOL *bHandled);
typedef void(CALLBACK *WindowDestroyCallback)(void *userdata, BOOL *bHandled);

void Window::OnClose(BOOL* proceed)
{
	SetDelegateInvoker(this);
	this->CloseDelegate(proceed);
    BOOL bHandle = FALSE;
    InvokeCallbacks<WindowCloseCallback>(LTK_WINDOW_CLOSE, proceed, &bHandle);
}

void Window::OnDestroy()
{
	SetDelegateInvoker(this);
	this->DestroyDelegate();
    BOOL bHandle = FALSE;
    InvokeCallbacks<WindowDestroyCallback>(LTK_WINDOW_DESTROY, &bHandle);
}

HWND Window::GetHWND()
{
	return m_hwnd;
}

LRESULT Window::OnImeEvent( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    // TODO 加个开关 能够关闭输入法 当焦点不在输入框上的时候就关闭
	switch(uMsg)
	{
	case WM_IME_SETCONTEXT:
		do 
		{
			//LOG(<<"WM_IME_SETCONTEXT");
			BOOL handled = FALSE;
			m_ime.CreateImeWindow(m_hwnd);
			m_ime.CleanupComposition(m_hwnd);
			m_ime.SetImeWindowStyle(m_hwnd, uMsg, wParam, lParam, &handled);
		} while (0);
		return 0;
	case WM_IME_STARTCOMPOSITION:
		//LOG(<<"WM_IME_STARTCOMPOSITION");
		m_ime.CreateImeWindow(m_hwnd);
		m_ime.ResetComposition(m_hwnd);
		return 0;
	case WM_IME_COMPOSITION:
		do
		{
			//LOG(<<"WM_IME_COMPOSITION");
			ImeComposition comp;
			m_ime.UpdateImeWindow(m_hwnd);
			m_ime.GetResult(m_hwnd, lParam, &comp);
			if (GCS_RESULTSTR == comp.string_type)
			{
				wstring tmp = comp.ime_string; // 这里结尾居然多出个0 可能是改了谷歌的代码导致的问题
				tmp.resize(wcslen(tmp.c_str()));
				LOGW(<<tmp);
				OnImeInput(tmp.c_str());
				//m_text += tmp;
				//::InvalidateRect(GetHWND(), NULL, TRUE);
			}
			m_ime.ResetComposition(m_hwnd);
			m_ime.EnableIME(m_hwnd, m_rectComposition, false); // 输入窗口跟随
		}while(0);
		return 0;
	case WM_IME_ENDCOMPOSITION:
		//LOG(<<"WM_IME_ENDCOMPOSITION");
		m_ime.ResetComposition(m_hwnd);
		m_ime.DestroyImeWindow(m_hwnd);
		//::ShowCaret(m_hwnd);
		::DefWindowProc(m_hwnd, uMsg, wParam, lParam);
		return 0;
	case WM_INPUTLANGCHANGE:
		//LOG(<<"WM_INPUTLANGCHANGE");
		m_ime.SetInputLanguage();
		::DefWindowProc(m_hwnd, uMsg, wParam, lParam);
		return 0;
	case WM_KEYDOWN:
		return 0;
	}
	return 0;
}

void Window::SetImePosition( float x, float y )
{
	m_rectComposition.left = (int)x;
	m_rectComposition.right = (int)x + 5;
	m_rectComposition.top = (int)y;
	m_rectComposition.bottom = (int)y + 20;
}

Widget *Window::GetRootWidget()
{
    return m_root;
}

Widget *Window::SetCentralWidget(Widget *sp)
{
    return m_root->SetCentralWidget(sp);
}

MenuBar *Window::SetMenu(MenuBar *m)
{
	return m_root->SetMenuBar(m);
}

MenuBar * Window::GetMenu()
{
	return m_root->GetMenuBar();
}

void Window::SetFocusWidget(Widget *sp )
{
	if (m_spFocus == sp)
	{
		return;
	}
	if (m_spFocus)
	{
		FocusEvent ev;
        ev.id = eKillFocus;
        m_spFocus->OnEvent(&ev);
	}
	m_spFocus = sp;
	if (m_spFocus)
	{
		FocusEvent ev;
        ev.id = eSetFocus;
        m_spFocus->OnEvent(&ev);
	}
}

Widget *Window::GetFocusWidget()
{
	return m_spFocus;
}

void Window::DisableFocusChange()
{
	m_bEnableFocusChange = false;
}

void Window::OnImeInput( PCTSTR text )
{
	if (m_spFocus)
	{
		// 其他的Widget也有可能去接受ime消息。比如再来一个RichEdit
		m_spFocus->HandleImeInput(text);
	}
}

void Window::SetCaretHeight( float h)
{
	m_caretHeight = (int)h;
}

void Window::SetCapture(Widget *sp )
{
	LTK_ASSERT(sp->GetWindow() == this);
	m_spCapture = sp;
    LTK_ASSERT(::IsWindow(m_hwnd));
	auto hwnd = ::SetCapture(m_hwnd);
    //LTK_LOG("old capture %p", hwnd);
}

void Window::ReleaseCapture()
{
	m_spCapture = NULL;
	::ReleaseCapture();
    //LTK_LOG("ReleaseCapture");
}

bool Window::IsCapturing(Widget *sp)
{
    return m_spCapture == sp;
}

void Window::ShowCaret()
{
	LTK_LOG("ShowCaret");
	::ShowCaret(m_hwnd);
}

void Window::HideCaret()
{
    LTK_LOG("HideCaret");
	::HideCaret(m_hwnd);
}

void Window::TrackMouseLeave(Widget *sp )
{
	if (sp->GetWindow() == this)
	{
		m_setTrackMouseLeave.insert(sp);
	}
	// Track for the HWND
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(tme);
    tme.hwndTrack = m_hwnd;
    tme.dwFlags = TME_LEAVE;
    tme.dwHoverTime = HOVER_DEFAULT;
    ::TrackMouseEvent(&tme);
}

void Window::BeginAnimation(Widget *sp)
{
    //if (!::IsIconic(m_hwnd)) {
        if (m_setAnimation.size() == 0)
        {
            const int  TARGET_RESOLUTION = 1;         // 1-millisecond target resolution
            TIMECAPS tc;
            UINT     wTimerRes;

            if (::timeGetDevCaps(&tc, sizeof(TIMECAPS)) == TIMERR_NOERROR) 
            {
                wTimerRes = min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);
                ::timeBeginPeriod(wTimerRes); 
                m_timerResolution = wTimerRes;
            }
            ::SetTimer(m_hwnd, TIMER_ANIMATION, 15, NULL);
        }
        m_setAnimation.insert(sp);
    //}
}

void Window::EndAnimation(Widget *sp)
{
    if (m_setAnimation.size() == 0)
    {
        ::KillTimer(m_hwnd, TIMER_ANIMATION);
        ::timeEndPeriod(m_timerResolution);
        return;
    }
    auto iter = m_setAnimation.find(sp);
    if (iter != m_setAnimation.end())
    {
        m_setAnimation.erase(iter);
        if (m_setAnimation.size() == 0)
        {
            ::KillTimer(m_hwnd, TIMER_ANIMATION);
            ::timeEndPeriod(m_timerResolution);
        }
    }
}

ID2D1Bitmap *Window::GetAtlasBitmap()
{
    return m_atlas;
}

void Window::SetBackground(LPCSTR style)
{
    m_background = StyleManager::Instance()->GetBackground(style);
    m_styleName = style;
}

void Window::UpdateTheme()
{
    m_background = StyleManager::Instance()->GetBackground(m_styleName.c_str());
    this->OnThemeChanged();
	if (m_root) {
		m_root->HandleThemeChange();
		// TODO send a eSizeChanged to the Widget tree.
		// let the controls use the new style measure.
		float cx, cy;
		RECT rc;
		::GetClientRect(m_hwnd, &rc);
		cx = (float)rc.right;
		cy = (float)rc.bottom;
		ltk::ScreenCoordToDip(cx, cy);
		SizeEvent ev;
		ev.id = eSizeChanged;
		ev.width = cx;
		ev.height = cy;
		m_root->OnEvent(&ev);
	}
}

void Window::UpdateShadowFrame(bool bRedraw)
{
    HDWP hdwp = ::BeginDeferWindowPos(4);
    m_shadowLeft.Update(m_hwnd, hdwp, bRedraw);
    m_shadowTop.Update(m_hwnd, hdwp, bRedraw);
    m_shadowRight.Update(m_hwnd, hdwp, bRedraw);
    m_shadowBottom.Update(m_hwnd, hdwp, bRedraw);
    BOOL ret = ::EndDeferWindowPos(hdwp);
    LTK_ASSERT(ret);
}

Events Window::TranslateMessageCode(UINT message)
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