//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StyleManager.h"
#include "Window.h"
#include "Button.h"
#include "ListView.h"
#include "TextEdit.h"
#include "TreeView.h"
#include "TimerManager.h"
#include "SetupStyles.h"
#include "BoxLayout.h"
#include "HeaderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

using namespace ltk;

void MyDumpMemoryLeak()
{
    _CrtDumpMemoryLeaks();
}

static void RecBuildNodes(TreeNode *parent, int depth)
{
    if (depth > 4) {
        return;
    }
    int num = rand() % 15 + 1;
    for (int i = 0; i < num; i++) {
        TreeNode *node = new TreeNode;
        wstring text = WStringFormat(L"%d", rand());
        node->SetText(text.c_str());
        parent->AddChild(node);
        if (rand() % 100 < 25) {
            RecBuildNodes(node, depth + 1);
        }
    }
}

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    LtkInitialize();

    StyleManager::Instance()->LoadFromXml("res\\style.xml");
    SetupPixelStyle1();

    unique_ptr<Window> wnd;
    wnd.reset(new Window);
    wnd->SetCaption(L"LTK测试窗口");
    wnd->SetBackground("window_bg");
    wnd->Create(nullptr, RectF(0, 0, 600, 500));
    wnd->CloseEvent.Attach([](bool &proceed) {
        //proceed = false;
        ::PostQuitMessage(0);
    });

    BoxLayout *hbox = new BoxLayout(BoxLayout::Horizontal);
    wnd->SetClientSprite(hbox);

    TreeView *tree = new TreeView;
    hbox->AddLayoutItem(tree, 100, 0.3f);
    
    ::srand(::GetTickCount());
    RecBuildNodes(tree->GetRootNode(), 0);


    BoxLayout *vbox = new BoxLayout(BoxLayout::Vertical);
    //vbox->SetSpacing(10);
    hbox->AddLayoutItem(vbox, 100, 0.7f);

    HeaderCtrl *header = new HeaderCtrl;
    header->AddColumn(L"项目名", 100);
    header->AddColumn(L"工程名", 200);
    header->AddColumn(L"负责人", 200);
    vbox->AddLayoutItem(header, 30, 0.0f);

    ListView *listview1 = new ListView();
    vbox->AddLayoutItem(listview1, 0.0f, 1.0f);
    listview1->SetHeaderCtrl(header);
    std::wstring text;
    UINT num = rand() % 200;
    for (UINT i = 0; i < num; i++) {
        text = WStringFormat(L"item:%d", i);
        listview1->AddItem(text.c_str());
        text = WStringFormat(L"subitem1:%d", i);
        listview1->SetSubItemText(i, 1, text.c_str());
        text = WStringFormat(L"subitem2:%d", i);
        listview1->SetSubItemText(i, 2, text.c_str());
    }

    TextEdit *edit1 = new TextEdit;
    vbox->AddLayoutItem(edit1, 100, 0.0f);

    auto hbox_btns = new BoxLayout(BoxLayout::Horizontal);
    vbox->AddLayoutItem(hbox_btns, 30, 0.0f);

    Button *btn1 = new Button;
    UINT cookie1 = 0;
    btn1->SetText(L"循环定时器");
    btn1->ClickedEvent.Attach([&]() {
        cookie1 = ltk::SetTimer(1000, cookie1,[&]() {
            LTK_LOG("tick: %d", cookie1);
        });
        //wnd->CloseWindow(); // WTF, with [&] you can capture unique_ptr
        //::PostQuitMessage(0);
    });
    hbox_btns->AddLayoutItem(btn1, 0, 1);

    Button *btn2 = new Button;
    btn2->SetText(L"停止");
    btn2->ClickedEvent.Attach([&]() {
        ltk::KillTimer(cookie1);
        cookie1 = 0;
    });
    hbox_btns->AddLayoutItem(btn2, 0, 1);

    Button *btn3 = new Button;
    UINT cookie2 = 0;
    btn3->SetText(L"单次定时器");
    btn3->ClickedEvent.Attach([&]() {
        cookie2 = ltk::SetOnceTimer(1000, cookie2, [&]() {
            LTK_LOG("tick: %d", cookie2);
            cookie2 = 0;
        });
        //wnd->CloseWindow(); // WTF, with [&] you can capture unique_ptr
        //::PostQuitMessage(0);
    });
    hbox_btns->AddLayoutItem(btn3, 0, 1);

    Button *btn4 = new Button;
    btn4->SetText(L"停止");
    btn4->ClickedEvent.Attach([&]() {
        ltk::KillTimer(cookie2);
        cookie2 = 0;
    });
    hbox_btns->AddLayoutItem(btn4, 0, 1);

    auto hbox_btns2 = new BoxLayout(BoxLayout::Horizontal);
    vbox->AddLayoutItem(hbox_btns2, 30, 0.0f);

    Button *btnPixelTheme = new Button;
    hbox_btns2->AddLayoutItem(btnPixelTheme, 100);
    btnPixelTheme->SetText(L"位图资源");
    btnPixelTheme->ClickedEvent.Attach([]() {
        StyleManager::Instance()->Clear();
        SetupPixelStyle1();
    });

    Button *btnRectTheme = new Button;
    hbox_btns2->AddLayoutItem(btnRectTheme, 100);
    btnRectTheme->SetText(L"纯色1");
    btnRectTheme->ClickedEvent.Attach([]() {
        StyleManager::Instance()->Clear();
        SetupVectorStyle1();
    });

    vbox->AddSpaceItem(5, 0);
    hbox->DoLayout();


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