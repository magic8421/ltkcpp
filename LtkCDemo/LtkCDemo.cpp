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

BOOL MainWindow::OnClose(ILtkWindow* sender)
{
	if (::MessageBox(sender->GetHWND(), L"确定关闭？", NULL, MB_OKCANCEL) == IDOK) {
		return FALSE;
	}
	return TRUE;
}

void MainWindow::OnDestroy(ILtkWindow* sender)
{
	sender->SetEventListener(NULL);
	::PostQuitMessage(0);
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

	ILtkButton* btn = NULL;
	factory->CreateButton(&btn);
	btn->SetText(L"Hello World");
	btn->SetRect(10, 10, 100, 100);
	window->SetClientWidget(btn);


	window->SetEventListener(this);
	window->CreateCentered(NULL, &size);
	window->UpdateTheme();

	window->Release();
	factory->Release();
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	LtkInitialize();

	MainWindow* main_wnd = new MainWindow();
	main_wnd->Create();

	LtkRunMessageLoop();
	main_wnd->Release();

	LtkUninitialize();
	return 0;
}