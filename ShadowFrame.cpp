//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShadowFrame.h"
#include "Common.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW 
//#endif

namespace ltk {

static const UINT WM_RENDER_LATER = WM_USER + 1;
static const LPCWSTR PNG_PATH = L"res\\round_wnd.png";
static const long m_sizeLeft = 27;
static const long m_sizeTop = 46;
static const long m_sizeRight = 25;
static const long m_sizeBottom = 26;

ShadowFrame::ShadowFrame(Mode m) : m_mode(m)
{
	m_font = new Gdiplus::Font(L"Î¢ÈíÑÅºÚ", 16.f, Gdiplus::FontStyleRegular,
		Gdiplus::UnitPixel, NULL);
	LTK_ASSERT(m_font->GetLastStatus() == Gdiplus::Ok);
}


ShadowFrame::~ShadowFrame()
{
    if (m_hwnd) {
        ::DestroyWindow(m_hwnd);
    }
}

void ShadowFrame::RegisterWndClass()
{
    WNDCLASS wc;
    wc.style = CS_VREDRAW | CS_HREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = HINST_THISCOMPONENT;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"ltk_ShadowFrame";

    ATOM a = RegisterClass(&wc);
    LTK_ASSERT(a);
}

LRESULT CALLBACK ShadowFrame::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    ShadowFrame *thiz = nullptr;
    if (WM_NCCREATE == message) {
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lparam);
        thiz = reinterpret_cast<ShadowFrame*>(lpcs->lpCreateParams);
        thiz->m_hwnd = hwnd;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(thiz));
    }
    else {
        thiz = reinterpret_cast<ShadowFrame *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    switch (message) {
    case WM_RENDER_LATER:
        {
            RECT rc;
            ::GetWindowRect(hwnd, &rc);
            thiz->OnDraw(rc);
        }
        break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDBLCLK:
		return thiz->HandleMouseMessage(hwnd, message, wparam, lparam);
	case WM_SETFOCUS:
		thiz->m_oldFocus = (HWND)wparam;
		break;
    case WM_NCDESTROY:
        thiz->m_hwnd = 0;
        break;
    }
    return ::DefWindowProc(hwnd, message, wparam, lparam);
}

void ShadowFrame::Create()
{
     HWND hwnd = ::CreateWindowEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW, L"ltk_ShadowFrame",
        L"", WS_POPUPWINDOW | WS_VISIBLE, 0, 0, 100, 100, NULL, NULL, HINST_THISCOMPONENT, this);
     LTK_ASSERT(hwnd != NULL);
}

void ShadowFrame::Destroy()
{
    ::DestroyWindow(m_hwnd);
}

void ShadowFrame::SetParent(HWND hParent)
{
	m_hParent = hParent;
}

void ShadowFrame::Init()
{
    m_bitmap = Gdiplus::Bitmap::FromFile(PNG_PATH);
    LTK_ASSERT(m_bitmap && m_bitmap->GetLastStatus() == Gdiplus::Ok);
    RegisterWndClass();
}

void ShadowFrame::Free()
{
    delete m_bitmap;
}

void ShadowFrame::Update(HWND hParent, HDWP &hdwp, bool bRedraw)
{
    RECT rc;
    ::GetWindowRect(hParent, &rc);
    RECT rc2 = { 0 };

    switch (m_mode) {
    case eLeft:
        rc2.left = rc.left - m_sizeLeft;
        rc2.top = rc.top;
        rc2.right = rc.left;
        rc2.bottom = rc.bottom;
        break;
    case eTop:
        rc2.left = rc.left - m_sizeLeft;
        rc2.top = rc.top - m_sizeTop;
        rc2.right = rc.right + m_sizeRight;
        rc2.bottom = rc.top;
        break;
    case eRight:
        rc2.left = rc.right;
        rc2.top = rc.top;
        rc2.right = rc.right + m_sizeRight;
        rc2.bottom = rc.bottom;
        break;
    case eBottom:
        rc2.left = rc.left - m_sizeLeft;
        rc2.top = rc.bottom;
        rc2.right = rc.right + m_sizeRight;
        rc2.bottom = rc.bottom + m_sizeBottom;
        break;
    default:
        LTK_ASSERT(false);
    }
    hdwp = ::DeferWindowPos(hdwp, m_hwnd, hParent, rc2.left, rc2.top, rc2.right - rc2.left, rc2.bottom - rc2.top, SWP_NOACTIVATE);
    if (bRedraw) {
        //OnDraw(rc2);
        ::PostMessage(m_hwnd, WM_RENDER_LATER, 0, 0);
    }
}

void ShadowFrame::OnDraw(const RECT &rc)
{
    HDC dcScreen = ::GetDC(NULL);
    LTK_ASSERT(dcScreen != NULL);
    HDC dcMemory = ::CreateCompatibleDC(dcScreen);
    LTK_ASSERT(dcMemory != NULL);
    HBITMAP bmp = ::CreateBitmap(rc.right - rc.left, rc.bottom - rc.top, 1, 32, NULL);
    LTK_ASSERT(bmp != NULL);
    HGDIOBJ oldBmp = ::SelectObject(dcMemory, (HGDIOBJ)bmp);

    Gdiplus::Graphics gdipg(dcMemory);
    Gdiplus::Rect rc3(0, 0, rc.right - rc.left, rc.bottom - rc.top);
    DrawShadow(gdipg, rc3);

    BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    POINT ptScreen;
    ptScreen.x = rc.left;
    ptScreen.y = rc.top;
    SIZE size;
    size.cx = rc.right - rc.left;
    size.cy = rc.bottom - rc.top;
    POINT ptSrc;
    ptSrc.x = 0;
    ptSrc.y = 0;
    BOOL bRet = ::UpdateLayeredWindow(m_hwnd, dcScreen, &ptScreen, &size, dcMemory,
        &ptSrc, 0, &blendPixelFunction, ULW_ALPHA);
    LTK_ASSERT(bRet);

    ::DeleteObject(bmp);
    ::SelectObject(dcMemory, oldBmp);
    ::DeleteDC(dcMemory);
    ::ReleaseDC(m_hwnd, dcScreen);
}

void ShadowFrame::DrawShadow(Gdiplus::Graphics &g, Gdiplus::Rect rc)
{
	// rc is the destination rect.
    int width = rc.Width;
    int height = rc.Height;
    
    switch (m_mode) {
    case eLeft:
        g.DrawImage(m_bitmap, rc, 
			0, m_sizeTop, 
			m_sizeLeft, m_bitmap->GetHeight() - m_sizeTop - m_sizeBottom,
            Gdiplus::UnitPixel, NULL, NULL, NULL);
        break;
    case eTop:
		rc.X = m_sizeLeft;
		rc.Y = 0;
		rc.Width = width - m_sizeLeft - m_sizeRight;
        g.DrawImage(m_bitmap, rc, m_sizeLeft, 0, m_bitmap->GetWidth() - m_sizeLeft - m_sizeRight,
            m_sizeTop, Gdiplus::UnitPixel, NULL, NULL, NULL);
		rc.X = 0;
		rc.Y = 0;
		rc.Width = m_sizeLeft;
		g.DrawImage(m_bitmap, rc,
			0, 0,
			m_sizeLeft, m_sizeTop,
			Gdiplus::UnitPixel, NULL, NULL, NULL);
		rc.X = width - m_sizeRight;
		rc.Width = m_sizeRight;
		rc.Height = m_sizeTop;
		g.DrawImage(m_bitmap, rc,
			m_bitmap->GetWidth() - m_sizeRight, 0,
			m_sizeRight, m_sizeTop,
			Gdiplus::UnitPixel, NULL, NULL, NULL);
        break;
    case eRight:
        g.DrawImage(m_bitmap, rc,
			m_bitmap->GetWidth() - m_sizeRight, m_sizeTop,
			m_sizeRight, m_bitmap->GetHeight() - m_sizeTop - m_sizeBottom,
            Gdiplus::UnitPixel, NULL, NULL, NULL);
        break;
    case eBottom:
		rc.X = 0;
		rc.Width = m_sizeLeft;
		g.DrawImage(m_bitmap, rc,
			0, m_bitmap->GetHeight() - m_sizeBottom,
			m_sizeLeft, m_sizeBottom,
			Gdiplus::UnitPixel, NULL, NULL, NULL);
		
		rc.X = width - m_sizeRight;
		rc.Width = m_sizeRight;
		g.DrawImage(m_bitmap, rc,
			m_bitmap->GetWidth() - m_sizeRight, m_bitmap->GetHeight() - m_sizeBottom,
			m_sizeRight, m_sizeBottom,
			Gdiplus::UnitPixel, NULL, NULL, NULL);

		rc.X = m_sizeLeft;
		rc.Width = width - m_sizeLeft - m_sizeRight;
        g.DrawImage(m_bitmap, rc,
			m_sizeLeft, m_bitmap->GetHeight() - m_sizeBottom,
            m_bitmap->GetWidth() - m_sizeLeft - m_sizeRight, m_sizeBottom,
            Gdiplus::UnitPixel, NULL, NULL, NULL);
        break;
    default:
        do 
        {
            Gdiplus::SolidBrush brush(0xcc00ccff);
            g.FillRectangle(&brush, rc);
        } while (0);
    }
}

LRESULT ShadowFrame::HandleMouseMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	const long margin = 18;
	long xPos = (short)LOWORD(lparam);
	long yPos = (short)HIWORD(lparam);
	switch (msg) {
	case WM_MOUSEMOVE:
		switch (m_mode) {
		case eTop:
			if (yPos < margin) {
				::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
			} 
			break;
		case eLeft:
			if (xPos < margin) {
				::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			}
			break;
		}
		if (m_bCapture) {
			HandleMouseMove(xPos, yPos);
		}
		break;
	case WM_LBUTTONDOWN:
		::GetCursorPos(&m_ptClick);
		::GetWindowRect(m_hParent, &m_oldRc);
		switch (m_mode) {
		case eTop:
			if (yPos > margin) {
				m_bCapture = true;
				::SetCapture(hwnd);
				m_action = aCaption;
			} else {
				m_bCapture = true;
				::SetCapture(hwnd);
				m_action = aTop;
			}
			break;
		}
		break;
	case WM_LBUTTONUP:
		if (m_bCapture) {
			::ReleaseCapture();
			m_bCapture = false;
			if (m_oldFocus) {
				::SetFocus(m_oldFocus);
			}
		}
		break;
	}
	return 0;
}

static void SetWindowRect(HWND hwnd, const RECT &rc)
{
	::MoveWindow(hwnd, rc.left, rc.top,
		rc.right - rc.left, rc.bottom - rc.top, FALSE);
}

void ShadowFrame::HandleMouseMove(long x, long y)
{
	POINT pt;
	::GetCursorPos(&pt);
	long delta_x = pt.x - m_ptClick.x;
	long delta_y = pt.y - m_ptClick.y;
	RECT rc = m_oldRc;


	switch (m_action) {
	case aCaption:
		rc.left = m_oldRc.left + delta_x;
		rc.top = m_oldRc.top + delta_y;
		rc.right = m_oldRc.right + delta_x;
		rc.bottom = m_oldRc.bottom + delta_y;
		SetWindowRect(m_hParent, rc);
		break;
	case aTop:
		rc.top = m_oldRc.top + delta_y;
		SetWindowRect(m_hParent, rc);
		break;
	}
}

void ShadowFrame::SetCaption(LPCWSTR t)
{
	m_caption = t;
}

Gdiplus::Bitmap * ShadowFrame::m_bitmap = nullptr;

}
