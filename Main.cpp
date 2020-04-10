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
#include "Main.h"
//#include "TimerWindow.h"
#include  "duktape/dukglue.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

using namespace ltk;

static int node_count = 0;

static void RecBuildNodes(TreeNode *parent, int depth)
{
	if (depth > 5) {
		return;
	}
	int num = rand() % 13 + 3;
	for (int i = 0; i < num; i++) {
		TreeNode *node = new TreeNode;
		auto text = WStringFormat(L"TreeNode: %d", node_count);
		node->SetText(text.c_str());
		parent->AddChild(node);
		node_count++;
		if (rand() % 100 < 35) {
			RecBuildNodes(node, depth + 1);
		}
	}
}


DemoWindow::DemoWindow()
{
	m_timer = new Timer;
	m_timer->SetParent(this);
	m_timer->SetInterval(1000);
	m_timer->TimeoutDelegate += MakeDelegate(this, &DemoWindow::OnTimer);

	m_onceTimer = new Timer;
	m_onceTimer->SetParent(this);
	m_onceTimer->SetInterval(1000);
	m_onceTimer->TimeoutDelegate += MakeDelegate(this, &DemoWindow::OnOnceTimer);
}

DemoWindow::~DemoWindow()
{
}

void DemoWindow::BuildDemoWindow()
{
	Window *wnd = this;
	//BoxLayout *hbox = new BoxLayout(ltk::Horizontal);
	Splitter *spitter1 = new Splitter(ltk::Horizontal);
	wnd->SetCentralWidget(spitter1);

	TreeView *tree = new TreeView;
	spitter1->AddClient(tree);
	spitter1->SetClientSize(0, 300);

	::srand(ltk::TickCount());

	RecBuildNodes(tree->GetRootNode(), 0);
	LTK_LOG("node_count: %d", node_count);

	BoxLayout *vboxRightPanel = new BoxLayout(ltk::Vertical);
	//vbox->SetSpacing(10);
	spitter1->AddClient(vboxRightPanel);

	ListView *listview1 = new ListView();
	HeaderCtrl *header = listview1->GetHeaderCtrl();
	header->AddColumn(L"Column0", 100);
	header->AddColumn(L"Column1", 200);
	header->AddColumn(L"Column2", 200);
	header->AddColumn(L"Column3", 200);
	header->AddColumn(L"Column4", 200);
	header->AddColumn(L"Column5", 200);
	listview1->UpdateColumnWidth();
	vboxRightPanel->AddLayoutItem(listview1, 0.0f, 1.0f);
	listview1->SelectChangeDelegate += MakeDelegate(this, &DemoWindow::OnListViewSelectChange);
	//listview1->RegisterCallback(LTK_LISTVIEW_SELECT_CHANGE, (LtkCallback)OnListViewSelectChange, this);

	UINT num = rand() % 200;
	for (UINT i = 0; i < num; i++) {
		listview1->AddRow();
		auto text = WStringFormat(L"col:0 row:%d", i);
		listview1->SetCellText(i, 0, text.c_str());
		text = WStringFormat(L"col:1 row:%d", i);
		listview1->SetCellText(i, 1, text.c_str());
		text = WStringFormat(L"col:2 row:%d", i);
		listview1->SetCellText(i, 2, text.c_str());
		text = WStringFormat(L"col:3 row:%d", i);
		listview1->SetCellText(i, 3, text.c_str());
		text = WStringFormat(L"col:4 row:%d", i);
		listview1->SetCellText(i, 4, text.c_str());
		text = WStringFormat(L"col:5 row:%d", i);
		listview1->SetCellText(i, 5, text.c_str());
	}

	TextEdit *edit1 = new TextEdit;
	vboxRightPanel->AddLayoutItem(edit1, 100, 0.0f);

	auto hboxTimerTest = new BoxLayout(ltk::Horizontal);
	vboxRightPanel->AddLayoutItem(hboxTimerTest, 30, 0.0f);

	Button *btnRepeatTimer = new Button;
	btnRepeatTimer->SetText(L"循环定时器");
	btnRepeatTimer->ClickedDelegate += MakeDelegate(m_timer, &Timer::Start);
	hboxTimerTest->AddLayoutItem(btnRepeatTimer, 0, 1);

	Button *btnStopRepeatTimer = new Button;
	btnStopRepeatTimer->SetText(L"停止");
	btnStopRepeatTimer->ClickedDelegate += MakeDelegate(m_timer, &Timer::Stop);
	hboxTimerTest->AddLayoutItem(btnStopRepeatTimer, 0, 1);

	Button *btnOnceTimer = new Button;
	btnOnceTimer->SetText(L"单次定时器");
	btnOnceTimer->ClickedDelegate += MakeDelegate(m_onceTimer, &Timer::StartOnce);
	hboxTimerTest->AddLayoutItem(btnOnceTimer, 0, 1);

	Button *btnStopOnceTimer = new Button;
	btnStopOnceTimer->SetText(L"停止");
	btnStopOnceTimer->ClickedDelegate += MakeDelegate(m_onceTimer, &Timer::Stop);
	hboxTimerTest->AddLayoutItem(btnStopOnceTimer, 0, 1);

	auto hboxTheme = new BoxLayout(ltk::Horizontal);
	vboxRightPanel->AddLayoutItem(hboxTheme, 30, 0.0f);

	Button *btnPixelTheme = new Button;
	hboxTheme->AddLayoutItem(btnPixelTheme, 100);
	btnPixelTheme->SetText(L"位图资源");
	btnPixelTheme->ClickedDelegate += MakeDelegate(
		this, &DemoWindow::OnPixelThemeClicked);

	Button *btnRectTheme = new Button;
	hboxTheme->AddLayoutItem(btnRectTheme, 100);
	btnRectTheme->SetText(L"暗色");
	btnRectTheme->ClickedDelegate += MakeDelegate(
		this, &DemoWindow::OnDarkThemeClicked);

	Button *btnRectThemeLight = new Button;
	hboxTheme->AddLayoutItem(btnRectThemeLight, 100);
	btnRectThemeLight->SetText(L"亮色");
	btnRectThemeLight->ClickedDelegate += MakeDelegate(
		this, &DemoWindow::OnLightThemeClicked);

	MenuBar *menu_bar = new MenuBar;
	wnd->SetMenu(menu_bar);
	menu_bar->AddItem(L"文件");
	menu_bar->AddItem(L"编辑");
	menu_bar->AddItem(L"自适应长度");
	menu_bar->AddItem(L"帮助");

	PopupMenu *popup = new PopupMenu;
	popup->SetWidth(120.f);
	popup->AddItem(L"新建", "");
	popup->AddItem(L"打开", "");
	popup->AddItem(L"历史记录", "");
	popup->AddItem(L"保存", "");
	popup->AddItem(L"另存为", "");
	popup->AddItem(L"退出", "");
	popup->GetMenuItemAt(5)->ClickedDelegate +=  MakeDelegate(
		this, &DemoWindow::OnExitClicked);
	menu_bar->SetPopupMenu(0, popup);

	PopupMenu *popup2 = new PopupMenu;
	popup2->SetWidth(220.f);
	popup2->AddItem(L"C:\\My Document\\file.txt", "");
	popup2->AddItem(L"C:\\My Document\\file2.txt", "");
	popup2->AddItem(L"C:\\My Document\\file3.txt", "");
	popup->SetSubMenu(2, popup2);

	popup = new PopupMenu;
	popup->SetWidth(120.f);
	popup->AddItem(L"撤销", "");
	popup->AddItem(L"重做", "");
	popup->AddSeparator();
	popup->AddItem(L"复制", "");
	popup->AddItem(L"剪切", "");
	popup->AddItem(L"粘贴", "");
	menu_bar->SetPopupMenu(1, popup);

	popup = new PopupMenu;
	popup->SetWidth(120.f);
	popup->AddItem(L"选择", "");
	popup->AddItem(L"钢笔", "");
	popup->AddItem(L"铅笔", "");
	popup->AddItem(L"橡皮擦", "");
	menu_bar->SetPopupMenu(2, popup);

	popup = new PopupMenu;
	popup->SetWidth(120.f);
	popup->AddItem(L"在线文档", "");
	popup->AddItem(L"关于", "");
	menu_bar->SetPopupMenu(3, popup);

	vboxRightPanel->AddSpaceItem(5, 0);
}

void DemoWindow::BuildSplitterTest2()
{
	Window *wnd = this;
	Splitter *spitter1 = new Splitter(ltk::Horizontal);
	wnd->SetCentralWidget(spitter1);
	spitter1->Resize(5);
	spitter1->SetClientAt(0, new Button);
	spitter1->SetClientSize(0, 100);
	//spitter1->SetClientAt(1, new Button);
	spitter1->SetClientSize(1, 100);
	spitter1->SetClientAt(2, new Button);
	spitter1->SetClientSize(2, 100);
	spitter1->SetClientAt(3, new Button);
	spitter1->SetClientSize(3, 100);
	spitter1->SetClientAt(4, new Button);
	spitter1->SetClientSize(4, 100);

	spitter1->DoLayout();
}

void DemoWindow::BuildSplitterTest()
{
	Window *wnd = this;

	Splitter *sp_left = new Splitter(ltk::Vertical);
	sp_left->AddClient(new Button);
	sp_left->SetClientSize(0, 100);
	sp_left->AddClient(new Button);
	sp_left->SetClientSize(1, 100);
	sp_left->AddClient(new Button);
	sp_left->SetClientSize(2, 100);

	Splitter *sp_mid = new Splitter(ltk::Vertical);
	sp_mid->AddClient(new Button);
	sp_mid->SetClientSize(0, 350);
	sp_mid->AddClient(new Button);
	sp_mid->SetClientSize(1, 100);

	Splitter *sp_right = new Splitter(ltk::Vertical);
	sp_right->AddClient(new Button);
	sp_right->SetClientSize(0, 100);
	sp_right->AddClient(new Button);
	sp_right->SetClientSize(1, 100);
	sp_right->AddClient(new Button);
	sp_right->SetClientSize(2, 100);

	Splitter *spitter1 = new Splitter(ltk::Horizontal);
	wnd->SetCentralWidget(spitter1);
	spitter1->AddClient(sp_left);
	spitter1->SetClientSize(0, 100);
	spitter1->AddClient(sp_mid);
	spitter1->SetClientSize(1, 500);
	spitter1->AddClient(sp_right);
	spitter1->SetClientSize(2, 100);
}



void DemoWindow::OnTimer()
{
	LTK_LOG("tick: %d", m_timer->GetId());
}

void DemoWindow::OnOnceTimer()
{
	LTK_LOG("tick: %d", m_onceTimer->GetId());
}

void DemoWindow::OnPixelThemeClicked()
{
	StyleManager::SetCurrentTheme("pixel");
	Window::UpdateTheme();
}

void DemoWindow::OnDarkThemeClicked()
{
	StyleManager::SetCurrentTheme("rect");
	Window::UpdateTheme();
}

void DemoWindow::OnLightThemeClicked()
{

}

void DemoWindow::OnExitClicked()
{
	::PostQuitMessage(0);
}

void DemoWindow::OnClose(BOOL* proceed)
{
	::PostQuitMessage(0);
}

void DemoWindow::OnListViewSelectChange(int row, int oldRow)
{
	LTK_LOG("OnListViewSelectChange: %d %d", row, oldRow);
}

void DemoWindow::InternStringTest()
{
	
}

static void SetupAppStyle()
{
	auto sm = StyleManager::Instance();
	sm->AddTextFormat2("timer_label_fmt", L"微软雅黑", DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, 34, DWRITE_TEXT_ALIGNMENT_CENTER,
		DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}

static duk_ret_t native_print(duk_context *ctx) {
	//duk_current_source_line(ctx);
	//duk_push_string(ctx, " ");
	//duk_insert(ctx, 0);
	duk_join(ctx, duk_get_top(ctx));
	// printf("%s\n", duk_safe_to_string(ctx, -1));
	auto text = Utf8ToUtf16(duk_safe_to_string(ctx, -1));
	text.append(L"\r\n");
	::OutputDebugStringW(text.c_str());
	return 0;
}

static void MyExitApp()
{
	::PostQuitMessage(0);
}

extern duk_context* g_duktape;

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    ltk::LtkInitialize();

	/*
	auto wnd = new DemoWindow;
    wnd->SetCaption(L"LTK测试窗口");
    //wnd->SetBackground("window_bg");
	wnd->BuildDemoWindow();
	//wnd->BuildSplitterTest2();
    wnd->Create(nullptr, SizeF(700, 500));
	wnd->UpdateTheme();
	*/

	/*
	SetupAppStyle();
	TimerWindow *wnd = new TimerWindow;
	//wnd->SetBackground("window_bg");
	wnd->Create(nullptr, SizeF(300, 500));
	wnd->UpdateTheme();
	*/

	duk_context* ctx = duk_create_heap_default();
	g_duktape = ctx;

	duk_push_c_function(ctx, native_print, DUK_VARARGS);
	duk_put_global_string(ctx, "print");

	dukglue_register_function(ctx, MyExitApp, "MyExitApp");
	dukglue_register_constructor<Window>(ctx, "Window");
	dukglue_register_method(ctx, &Window::CreateCenter, "CreateCenter");
	duk_push_string(ctx, 
		"var wnd = new Window(); \n"
		"wnd.CreateCenter(400, 300); \n"
		"wnd.OnDelete = function () { MyExitApp(); }; ");

	if (duk_peval(ctx) != 0) {
		LTK_LOG("eval failed: %s", duk_safe_to_string(ctx, -1));
	} else {
		LTK_LOG("result: %s", duk_safe_to_string(ctx, -1));
	}

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
	//ltk::Ptr<Window> ob = wnd->GetPtr<Window>();
	//delete wnd;
	//LTK_ASSERT(ob.Get() == nullptr);
	
	duk_destroy_heap(ctx);

    LTK_LOG("MessageLoop END");
    ::Sleep(2000);

    ltk::LtkUninitialize();
    return 0;
}
