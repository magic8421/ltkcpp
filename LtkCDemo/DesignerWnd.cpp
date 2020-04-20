#include "pch.h"
#include "DesignerWnd.h"
#include "LtkInterface.h"
#include "LtkSplitter.h"
#include "WidgetTreeView.h"
#include "LtkButton.h"
#include "LtkMenu.h"

void DesignerWnd::Create()
{
	LtkWindow::Create();
	LtkSplitter splt1;
	splt1.Create(LTK_HORIZONTAL);
	LtkWindow::SetCentralWidget(&splt1);

	LtkButton btn1;
	btn1.Create();
	btn1.SetText("fuck");
	splt1.AddClient(&btn1);
	splt1.SetClientSize(0, 300);

	btn1.Create();
	btn1.SetText("shit");
	splt1.AddClient(&btn1);
	splt1.SetClientSize(1, 300);

	/*m_wdgtTree.Create();
	splt1.AddClient(&m_wdgtTree);
	splt1.SetClientSize(0, 300);

	LtkTreeView tree2;
	tree2.Create();
	splt1.AddClient(&tree2);
	splt1.SetClientSize(1, 300);*/

	/*
	std::vector<LtkSplitter> vecSplitter;
	LtkSplitter splt2;
	splt2.Create(LTK_HORIZONTAL);
	vecSplitter.push_back(std::move(splt2));
	*/

	LtkWindow::CreateHwndCenter(NULL, 800, 600);
	LtkWindow::SetCaption("LTK轻量界面库设计器");
}

void DesignerWnd::OnDestroy()
{
	::PostQuitMessage(0);
}

void DesignerWnd::OnClose(BOOL* proceed)
{
	if (::MessageBox(LtkWindow_GetHWND(m_hltk), L"确定要退出吗?", 0, MB_OKCANCEL) == IDCANCEL) {
		*proceed = FALSE;
	}
}
