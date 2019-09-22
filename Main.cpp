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
#include "MenuBar.h"
#include "Splitter.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

using namespace ltk;

static int node_count = 0;

static void RecBuildNodes(TreeNode *parent, int depth)
{
    if (depth > 4) {
        return;
    }
    int num = rand() % 13 + 3;
    for (int i = 0; i < num; i++) {
        TreeNode *node = new TreeNode;
        wstring text = WStringFormat(L"%d", rand());
        node->SetText(text.c_str());
        parent->AddChild(node);
		node_count++;
        if (rand() % 100 < 35) {
            RecBuildNodes(node, depth + 1);
        }
    }
}

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    ltk::LtkInitialize();

    //StyleManager::NewTheme("rect");
    //SetupVectorStyle1();

    //StyleManager::NewTheme("pixel");
    ////StyleManager::Instance()->LoadFromXml("res\\style.xml");
    //SetupPixelStyle1();

	auto wnd = new Window;
    wnd->SetCaption(L"LTK测试窗口");
	wnd->EnableShadow(false);
	wnd->SetBackground("window_bg");
    wnd->Create(nullptr, SizeF(600, 500));
	wnd->AttachCloseDelegate([](bool &proceed) {
        //proceed = false;
        ::PostQuitMessage(0);
    });

	/*
	Splitter *spitter1 = new Splitter(Splitter::Horizontal);
	wnd->SetClientSprite(spitter1);
	spitter1->AddClient(new Button);
	spitter1->SetClientSize(0, 100);
	spitter1->AddClient(new Button);
	spitter1->SetClientSize(1, 100);
	spitter1->AddClient(new Button);
	spitter1->SetClientSize(2, 100);
	spitter1->AddClient(new Button);
	spitter1->SetClientSize(3, 100);
	*/

    BoxLayout *hbox = new BoxLayout(BoxLayout::Horizontal);
    wnd->SetClientSprite(hbox);

    TreeView *tree = new TreeView;
    hbox->AddLayoutItem(tree, 100, 0.3f);
    
    ::srand(ltk::TickCount());

    RecBuildNodes(tree->GetRootNode(), 0);
	LTK_LOG("node_count: %d", node_count);

    BoxLayout *vboxRightPanel = new BoxLayout(BoxLayout::Vertical);
    //vbox->SetSpacing(10);
    hbox->AddLayoutItem(vboxRightPanel, 100, 0.7f);

    HeaderCtrl *header = new HeaderCtrl;
    header->AddColumn(L"项目名", 100);
    header->AddColumn(L"工程名", 200);
    header->AddColumn(L"负责人", 200);
    vboxRightPanel->AddLayoutItem(header, 30, 0.0f);

    ListView *listview1 = new ListView();
    vboxRightPanel->AddLayoutItem(listview1, 0.0f, 1.0f);
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
    vboxRightPanel->AddLayoutItem(edit1, 100, 0.0f);

    auto hboxTimerTest = new BoxLayout(BoxLayout::Horizontal);
    vboxRightPanel->AddLayoutItem(hboxTimerTest, 30, 0.0f);

    Button *btnRepeatTimer = new Button;
    UINT cookie1 = 0;
    btnRepeatTimer->SetText(L"循环定时器");
	btnRepeatTimer->AttachClickedDelegate([&]() {
        cookie1 = ltk::SetTimer(1000, cookie1,[&]() {
            LTK_LOG("tick: %d", cookie1);
        });
        //wnd->CloseWindow(); // WTF, with [&] you can capture unique_ptr
        //::PostQuitMessage(0);
    });
    hboxTimerTest->AddLayoutItem(btnRepeatTimer, 0, 1);

    Button *btnStopRepeatTimer = new Button;
    btnStopRepeatTimer->SetText(L"停止");
	btnStopRepeatTimer->AttachClickedDelegate([&]() {
        ltk::KillTimer(cookie1);
        cookie1 = 0;
    });
    hboxTimerTest->AddLayoutItem(btnStopRepeatTimer, 0, 1);

    Button *btnOnceTimer = new Button;
    UINT cookie2 = 0;
    btnOnceTimer->SetText(L"单次定时器");
	btnOnceTimer->AttachClickedDelegate([&]() {
        cookie2 = ltk::SetOnceTimer(1000, cookie2, [&]() {
            LTK_LOG("tick: %d", cookie2);
            cookie2 = 0;
        });
        //wnd->CloseWindow(); // WTF, with [&] you can capture unique_ptr
        //::PostQuitMessage(0);
    });
    hboxTimerTest->AddLayoutItem(btnOnceTimer, 0, 1);

    Button *btnStopOnceTimer = new Button;
    btnStopOnceTimer->SetText(L"停止");
	btnStopOnceTimer->AttachClickedDelegate([&]() {
        ltk::KillTimer(cookie2);
        cookie2 = 0;
    });
    hboxTimerTest->AddLayoutItem(btnStopOnceTimer, 0, 1);

    auto hboxTheme = new BoxLayout(BoxLayout::Horizontal);
    vboxRightPanel->AddLayoutItem(hboxTheme, 30, 0.0f);

    Button *btnPixelTheme = new Button;
    hboxTheme->AddLayoutItem(btnPixelTheme, 100);
    btnPixelTheme->SetText(L"位图资源");
	btnPixelTheme->AttachClickedDelegate([wnd]() {
        StyleManager::SetCurrentTheme("pixel");
        wnd->UpdateTheme();
    });

    Button *btnRectTheme = new Button;
    hboxTheme->AddLayoutItem(btnRectTheme, 100);
    btnRectTheme->SetText(L"纯色1");
	btnRectTheme->AttachClickedDelegate([wnd]() {
        StyleManager::SetCurrentTheme("rect");
        wnd->UpdateTheme();
    });

	MenuBar *menu_bar = new MenuBar;
	wnd->SetMenu(menu_bar);
	menu_bar->AddItem(L"文件");
	menu_bar->AddItem(L"编辑");
	menu_bar->AddItem(L"自适应长度");
	menu_bar->AddItem(L"帮助");

	PopupMenu *popup = new PopupMenu;
	popup->SetWidth(120.f);
	popup->AddItem(L"新建");
	popup->AddItem(L"打开");
	popup->AddItem(L"历史记录");
	popup->AddItem(L"保存");
	popup->AddItem(L"另存为");
	popup->AddItem(L"退出");
	menu_bar->SetPopupMenu(0, popup);

	PopupMenu *popup2 = new PopupMenu;
	popup2->SetWidth(220.f);
	popup2->AddItem(L"C:\\My Document\\file.txt");
	popup2->AddItem(L"C:\\My Document\\file2.txt");
	popup2->AddItem(L"C:\\My Document\\file3.txt");
	popup->SetSubMenu(2, popup2);

	popup = new PopupMenu;
	popup->SetWidth(120.f);
	popup->AddItem(L"撤销");
	popup->AddItem(L"重做");
	popup->AddItem(L"复制");
	popup->AddItem(L"剪切");
	popup->AddItem(L"粘贴");
	menu_bar->SetPopupMenu(1, popup);

	popup = new PopupMenu;
	popup->SetWidth(120.f);
	popup->AddItem(L"选择");
	popup->AddItem(L"钢笔");
	popup->AddItem(L"铅笔");
	popup->AddItem(L"橡皮擦");
	menu_bar->SetPopupMenu(2, popup);

	popup = new PopupMenu;
	popup->SetWidth(120.f);
	popup->AddItem(L"在线文档");
	popup->AddItem(L"关于");
	menu_bar->SetPopupMenu(3, popup);

    vboxRightPanel->AddSpaceItem(5, 0);
	
	
	wnd->UpdateTheme();
	//spitter1->DoLayout();


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
	auto ob = ToPtr(wnd);
	delete wnd;
	LTK_ASSERT(ob.Get() == nullptr);

    LTK_LOG("MessageLoop END");
    ::Sleep(2000);

    ltk::LtkUninitialize();
    return 0;
}