#include "stdafx.h"
#include "StyleManager.h"
#include "Window.h"

using namespace ltk;

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    LtkLogInit();
    LtkInitialize();
    ShadowFrame::Init();
    Window::RegisterWndClass();

    StyleManager::Instance()->LoadFromXml("res\\style.xml");

    RefPtr<Window> wnd;
    wnd.Attach(new Window);
    wnd->SetBackground("window_bg");
    wnd->Create(nullptr, RectF(0, 0, 400, 300));
    wnd->CloseEvent.Attach([](bool &proceed) {
        //proceed = false;
        ::PostQuitMessage(0);
    });

    MSG msg;
    BOOL bRet;
    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
        if (bRet == -1) {
            LTK_LOG("MessageLoop ERROR");
        } else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    LTK_LOG("MessageLoop END");
    ::Sleep(2000);

    LtkUninitialize();

    return 0;
}