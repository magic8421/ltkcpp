// LtkCDemo.cpp : 定义应用程序的入口点。
//

#include "pch.h"
#include "framework.h"
#include "LtkCDemo.h"
#include "LtkInterface.h"


MainWindow::~MainWindow() 
{
	SAFE_RELEASE(m_window);
}

HRESULT MainWindow::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}
/*
BOOL MainWindow::OnClose(ILtkWindow* sender)
{
	if (::MessageBox(sender->GetHWND(), L"确定关闭？", NULL, MB_OKCANCEL) == IDOK) {
		return FALSE;
	}
	return TRUE;
}
*/
void MainWindow::OnDestroy(ILtkWindow* sender)
{
	m_window->Dispose();
	::PostQuitMessage(0);
}

void MainWindow::FillListView(ILtkListView* listview)
{
	UINT num = rand() % 200;
	wchar_t text[64];
	for (UINT i = 0; i < num; i++) {
		listview->AddRow();
		StringCbPrintf(text, sizeof(text), L"item:%d", i);
		listview->SetCellText(i, 0, text);
		StringCbPrintf(text, sizeof(text), L"subitem1:%d", i);
		listview->SetCellText(i, 1, text);
		StringCbPrintf(text, sizeof(text), L"subitem2:%d", i);
		listview->SetCellText(i, 2, text);
	}
}

void MainWindow::Create()
{
	ILtkFactory* factory = NULL;
	LtkGetFactory(&factory);
	LtkSize size{ 500.f, 400.f };
	ILtkWindow* window = NULL;
	factory->CreateWindow(&window);

	m_window = window;
	m_window->AddRef();

	ILtkSplitter *splitter1 = NULL;
	factory->CreateSplitter(LTK_HORIZONTAL, &splitter1);

	ILtkButton* btn1 = NULL;
	factory->CreateButton(&btn1);
	btn1->SetText(L"Left");
	splitter1->AddClient(btn1);
	splitter1->SetClientSize(0, 200);
	SAFE_RELEASE(btn1);

	ILtkListView* listview = NULL;
	factory->CreateListView(&listview);
	listview->AddColumn(L"Col1", 200);
	listview->AddColumn(L"Col2", 200);
	listview->AddColumn(L"Col3", 200);
	FillListView(listview);
	splitter1->AddClient(listview);
	SAFE_RELEASE(listview);

	ILtkMenuBar *menu_bar = NULL;
	BuildMenu(&menu_bar);
	window->SetMenuBar(menu_bar);
	SAFE_RELEASE(menu_bar);
	
	window->SetCentralWidget(splitter1);
	SAFE_RELEASE(splitter1);
	window->SetWindowListener(this);
	window->SetActionListener(this);
	window->CreateCentered(NULL, &size);
	window->SetCaption(L"Hello World");
	window->UpdateTheme();

	SAFE_RELEASE(window);
	SAFE_RELEASE(factory);
}

void MainWindow::BuildMenu(ILtkMenuBar ** ppMenu)
{
	ILtkFactory* factory = NULL;
	LtkGetFactory(&factory);

	ILtkMenuBar *menu_bar = NULL;
	factory->CreateMenuBar(&menu_bar);
	menu_bar->AddItem(L"文件");
	menu_bar->AddItem(L"编辑");
	menu_bar->AddItem(L"自适应长度");
	menu_bar->AddItem(L"帮助");

	ILtkPopupMenu *popup = NULL;
	factory->CreatePopupMenu(&popup);
	popup->AddItem(L"新建", "");
	popup->AddItem(L"打开", "");
	popup->AddItem(L"历史记录", "");
	popup->AddItem(L"保存", "");
	popup->AddItem(L"另存为", "");
	popup->AddItem(L"退出", "app_exit");
	menu_bar->SetPopupMenu(0, popup);


	ILtkPopupMenu *popup2 = NULL;
	factory->CreatePopupMenu(&popup2);
	popup2->AddItem(L"C:\\My Document\\file.txt", "");
	popup2->AddItem(L"C:\\My Document\\file2.txt", "");
	popup2->AddItem(L"C:\\My Document\\file3.txt", "");
	popup->SetSubMenu(2, popup2);
	SAFE_RELEASE(popup2);
	SAFE_RELEASE(popup);

	factory->CreatePopupMenu(&popup);
	popup->AddItem(L"撤销", "");
	popup->AddItem(L"重做", "");
	popup->AddSeparator();
	popup->AddItem(L"复制", "");
	popup->AddItem(L"剪切", "");
	popup->AddItem(L"粘贴", "");
	menu_bar->SetPopupMenu(1, popup);
	SAFE_RELEASE(popup);

	factory->CreatePopupMenu(&popup);
	popup->AddItem(L"选择", "");
	popup->AddItem(L"钢笔", "");
	popup->AddItem(L"铅笔", "");
	popup->AddItem(L"橡皮擦", "");
	menu_bar->SetPopupMenu(2, popup);
	SAFE_RELEASE(popup);

	factory->CreatePopupMenu(&popup);
	popup->AddItem(L"在线文档", "");
	popup->AddItem(L"关于", "");
	menu_bar->SetPopupMenu(3, popup);
	SAFE_RELEASE(popup);

	*ppMenu = menu_bar;
	SAFE_RELEASE(factory);
}

void MainWindow::OnClick(IUnknown* sender, LPCSTR name)
{
	if (!name)
		return;
	if (!strcmp(name, "app_exit")) {
		m_window->Dispose();
		::PostQuitMessage(0);
	}
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	LtkInitialize(LTK_VERSION);

	MainWindow* main_wnd = new MainWindow();
	main_wnd->Create();

	LtkRunMessageLoop();
	main_wnd->Release();

	LtkUninitialize();
	return 0;
}