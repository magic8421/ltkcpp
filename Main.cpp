#include "stdafx.h"
#include "StyleManager.h"
#include "Window.h"
#include "Button.h"

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

    unique_ptr<Window> wnd;
    wnd.reset(new Window);
    wnd->SetBackground("window_bg");
    wnd->Create(nullptr, RectF(0, 0, 400, 300));
    wnd->CloseEvent.Attach([](bool &proceed) {
        //proceed = false;
        ::PostQuitMessage(0);
    });

    Button *btn1 = new Button;
    btn1->SetText(L"´ó¼ÒºÃ");
    btn1->ClickedEvent.Attach([&]() {
        wnd->CloseWindow(); // WTF, with [&] you can capture unique_ptr
        ::PostQuitMessage(0);
    });
    //btn1->SetRect(RectF(10, 50, 100, 40));
    //wnd->GetRootSprite()->AddChild(btn1);
    BoxLayout *vbox = wnd->GetRootSprite()->As<BoxLayout>();
    vbox->SetMargin(10);
    vbox->AddLayoutItem(btn1, 40);

    vbox->DoLayout();

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