#include "stdafx.h"
#include "Common.h"
#include "Object.h"
#include "HiddenWindow.h"

namespace ltk {

static HWND g_hwndUITask = 0;
const static UINT WM_DELETE_LATER = WM_USER + 1;

void HiddenWindow::Init()
{
    WNDCLASS wc;
    ::ZeroMemory(&wc, sizeof(wc));
    wc.hInstance = ::GetModuleHandle(L"ltkcpp.dll"); // TODO what if we change the dll name?
    wc.lpszClassName = L"MyMsgWnd";
    wc.lpfnWndProc = MyWndProc;
    auto ret = ::RegisterClass(&wc);
    LTK_ASSERT(ret);

    g_hwndUITask = ::CreateWindow(L"MyMsgWnd", L"", WS_POPUP, 0, 0, 10, 10, HWND_MESSAGE, NULL, 
        ::GetModuleHandle(L"ltkcpp.dll"), NULL);
    LTK_ASSERT(g_hwndUITask && ::IsWindow(g_hwndUITask));
    int i = 1;
    i = 2;
}

LRESULT CALLBACK HiddenWindow::MyWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_DELETE_LATER:
        do {
            delete (Object*)wparam;
        } while (0);
        return 0;
    default:
        break;
    }
    return ::DefWindowProc(hwnd, message, wparam, lparam);
}

void HiddenWindow::PostDeleteLater(Object* obj)
{
    ::PostMessage(g_hwndUITask, WM_DELETE_LATER, (WPARAM)obj, NULL);
}

} // namespace ltk