//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShadowFrame.h"
#include "Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

const UINT ShadowFrame::WM_RENDER_LATER = WM_USER + 1;

ShadowFrame::ShadowFrame(Mode m) : m_mode(m)
{
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
        do 
        {
            RECT rc;
            ::GetWindowRect(hwnd, &rc);
            thiz->OnDraw(rc);
        } while (0);
        break;
    case WM_NCDESTROY:
        thiz->m_hwnd = 0;
        break;
    }
    return ::DefWindowProc(hwnd, message, wparam, lparam);
}

void ShadowFrame::Create()
{
     HWND hwnd = ::CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW, L"ltk_ShadowFrame",
        L"", WS_POPUPWINDOW, 0, 0, 100, 100, NULL, NULL, HINST_THISCOMPONENT, this);
     LTK_ASSERT(hwnd != NULL);
}

void ShadowFrame::Destroy()
{
    ::DestroyWindow(m_hwnd);
}

void ShadowFrame::Show(bool show)
{
	if (show) {
		::ShowWindow(m_hwnd, SW_SHOW);
	}
	else {
		::ShowWindow(m_hwnd, SW_HIDE);
	}
}

void ShadowFrame::Init()
{
    m_bitmap = Gdiplus::Bitmap::FromFile(L"res\\shadow.png");
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
        rc2.top = rc.top - m_sizeTop;
        rc2.right = rc.left;
        rc2.bottom = rc.bottom + m_sizeBottom;
        break;
    case eTop:
        rc2.left = rc.left;
        rc2.top = rc.top - m_sizeTop;
        rc2.right = rc.right;
        rc2.bottom = rc.top;
        break;
    case eRight:
        rc2.left = rc.right;
        rc2.top = rc.top - m_sizeTop;
        rc2.right = rc.right + m_sizeRight;
        rc2.bottom = rc.bottom + m_sizeBottom;
        break;
    case eBottom:
        rc2.left = rc.left;
        rc2.top = rc.bottom;
        rc2.right = rc.right;
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
    Gdiplus::Rect rc2;
    int width = rc.Width;
    int height = rc.Height;
    
    switch (m_mode) {
    case eLeft:
        rc.Y = m_sizeTop;
        rc.Height = height - m_sizeTop - m_sizeBottom;
        g.DrawImage(m_bitmap, rc, 0, m_sizeTop, m_sizeLeft, m_bitmap->GetHeight() - m_sizeTop - m_sizeBottom,
            Gdiplus::UnitPixel, NULL, NULL, NULL);
        rc.X = 0;
        rc.Y = 0;
        rc.Width = m_sizeLeft;
        rc.Height = m_sizeTop;
        g.DrawImage(m_bitmap, rc, 0, 0, m_sizeLeft, m_sizeTop,
            Gdiplus::UnitPixel, NULL, NULL, NULL);
        rc.Y = height - m_sizeBottom;
        rc.Height = m_sizeBottom;
        g.DrawImage(m_bitmap, rc, 0, m_bitmap->GetHeight() - m_sizeBottom, m_sizeLeft, m_sizeBottom,
            Gdiplus::UnitPixel, NULL, NULL, NULL);
        break;
    case eTop:
        g.DrawImage(m_bitmap, rc, m_sizeLeft, 0, m_bitmap->GetWidth() - m_sizeLeft - m_sizeRight,
            m_sizeTop, Gdiplus::UnitPixel, NULL, NULL, NULL);
        break;
    case eRight:
        rc.X = width - m_sizeRight;
        rc.Y = m_sizeTop;
        rc.Width = m_sizeRight;
        rc.Height = height - m_sizeTop - m_sizeBottom;
        g.DrawImage(m_bitmap, rc, m_bitmap->GetWidth() - m_sizeRight, m_sizeTop, m_sizeRight, m_bitmap->GetHeight() - m_sizeTop - m_sizeBottom,
            Gdiplus::UnitPixel, NULL, NULL, NULL);
        rc.X = 0;
        rc.Y = 0;
        rc.Width = m_sizeRight;
        rc.Height = m_sizeTop;
        g.DrawImage(m_bitmap, rc, m_bitmap->GetWidth() - m_sizeRight, 0, m_sizeRight, m_sizeTop,
            Gdiplus::UnitPixel, NULL, NULL, NULL);
        rc.X = width - m_sizeRight;
        rc.Y = height - m_sizeBottom;
        rc.Height = m_sizeBottom;
        g.DrawImage(m_bitmap, rc, m_bitmap->GetWidth() - m_sizeRight, m_bitmap->GetHeight() - m_sizeBottom,
            m_sizeRight, m_sizeBottom,
            Gdiplus::UnitPixel, NULL, NULL, NULL);
        break;
    case eBottom:
        g.DrawImage(m_bitmap, rc, m_sizeLeft, m_bitmap->GetHeight() - m_sizeBottom,
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

Gdiplus::Bitmap * ShadowFrame::m_bitmap = nullptr;

}
