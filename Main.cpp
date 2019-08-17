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

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

using namespace ltk;

void MyDumpMemoryLeak()
{
    _CrtDumpMemoryLeaks();
}

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
    TreeNode *node1 = new TreeNode;
    node1->SetText(L"第一项");
    tree->GetRootNode()->AddChild(node1);

    TreeNode *node2 = new TreeNode;
    node2->SetText(L"子项目1");
    node1->AddChild(node2);

    node2 = new TreeNode;
    node2->SetText(L"子项目2");
    node1->AddChild(node2);

    node2 = new TreeNode;
    node2->SetText(L"子项目3");
    node1->AddChild(node2);

    TreeNode *node3 = new TreeNode;
    node3->SetText(L"子子项目1");
    node2->AddChild(node3);

    node2 = new TreeNode;
    node2->SetText(L"子项目4");
    node1->AddChild(node2);

    node1 = new TreeNode;
    node1->SetText(L"第二项");
    tree->GetRootNode()->AddChild(node1);

    node1 = new TreeNode;
    node1->SetText(L"第三项");
    tree->GetRootNode()->AddChild(node1);


    BoxLayout *vbox = new BoxLayout(BoxLayout::Vertical);
    vbox->SetMargin(10);
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
    for (UINT i = 0; i < 200; i++) {
        text = WStringFormat(L"item:%d", i);
        listview1->AddItem(text.c_str());
        text = WStringFormat(L"subitem1:%d", i);
        listview1->SetSubItemText(i, 1, text.c_str());
        text = WStringFormat(L"subitem2:%d", i);
        listview1->SetSubItemText(i, 2, text.c_str());
    }

    TextEdit *edit1 = new TextEdit;
    vbox->AddLayoutItem(edit1, 100, 0.0f);

    Button *btn1 = new Button;
    btn1->SetText(L"退出");
    btn1->ClickedEvent.Attach([&]() {
        wnd->CloseWindow(); // WTF, with [&] you can capture unique_ptr
        ::PostQuitMessage(0);
    });
    vbox->AddLayoutItem(btn1, 40);


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