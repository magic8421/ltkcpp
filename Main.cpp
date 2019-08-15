#include "stdafx.h"
#include "StyleManager.h"
#include "Window.h"
#include "Button.h"
#include "ListView.h"
#include "TextEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

using namespace ltk;

void MyDumpMemoryLeak()
{
    _CrtDumpMemoryLeaks();
}

void test_deffer()
{
    char *p = nullptr;
    DEFER_BEGIN()
        if (p) {
            LTK_LOG("has p");
            delete p;
        } else {
            LTK_LOG("no p");
        }
    DEFER_END()
    p = new char[10];
}

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    test_deffer();

    LtkLogInit();
    LtkInitialize();
    ShadowFrame::Init();
    Window::RegisterWndClass();

    StyleManager::Instance()->LoadFromXml("res\\style.xml");

    unique_ptr<Window> wnd;
    wnd.reset(new Window);
    wnd->SetCaption(L"LTK测试窗口");
    wnd->SetBackground("window_bg");
    wnd->Create(nullptr, RectF(0, 0, 600, 500));
    wnd->CloseEvent.Attach([](bool &proceed) {
        //proceed = false;
        ::PostQuitMessage(0);
    });

    Button *btn1 = new Button;
    btn1->SetText(L"大家好");
    btn1->ClickedEvent.Attach([&]() {
        wnd->CloseWindow(); // WTF, with [&] you can capture unique_ptr
        ::PostQuitMessage(0);
    });

    //btn1->SetRect(RectF(10, 50, 100, 40));
    //wnd->GetRootSprite()->AddChild(btn1);
    BoxLayout *vbox = new BoxLayout(BoxLayout::Vertical);
    vbox->SetMargin(10);
    vbox->AddLayoutItem(btn1, 40);
    wnd->SetClientSprite(vbox);

    TextEdit *edit1 = new TextEdit;
    vbox->AddLayoutItem(edit1, 200, 0.0f);

    HeaderCtrl *header = new HeaderCtrl;
    header->AddColumn(L"项目名", 100);
    header->AddColumn(L"工程名", 400);
    header->AddColumn(L"负责人", 600);
    vbox->AddLayoutItem(header, 30, 0.0f);

    ListView *listview1 = new ListView();
    vbox->AddLayoutItem(listview1, 0.0f, 1.0f);
    listview1->SetHeaderCtrl(header);
    std::wstring text;
    for (UINT i = 0; i < 200; i++) {
        text = WStringFormat(L"item:%d", i);
        listview1->AddItem(text.c_str());
        text = WStringFormat(L"subitem1:%d", i);
        listview1->SetSubItemText(i, 1, text.c_str());
        text = WStringFormat(L"subitem2:%d", i);
        listview1->SetSubItemText(i, 2, text.c_str());
    }

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

    ::atexit(MyDumpMemoryLeak);
    return 0;
}