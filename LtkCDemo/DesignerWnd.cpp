#include "pch.h"
#include "DesignerWnd.h"
#include "LtkInterface.h"
#include "LtkSplitter.h"
#include "WidgetTreeView.h"
#include "LtkButton.h"
#include "LtkMenu.h"

#ifdef DEBUG
#define new DEBUG_NEW
#endif // DEBUG


DesignerWnd::~DesignerWnd()
{
	LtkDelete(this->hWindow);
}

void DesignerWnd::Create()
{
	this->hWindow = LtkWindow_New();
	HLTK splt1 = LtkSplitter_New(LTK_HORIZONTAL);
	LtkRegisterCallback(this->hWindow, LTK_WINDOW_CLOSE, (LtkCallback)DesignerWnd::OnClose, this);
	LtkRegisterCallback(this->hWindow, LTK_WINDOW_DESTROY, (LtkCallback)DesignerWnd::OnDestroy, this);
	LtkWindow_SetCentralWidget(this->hWindow, splt1);

	HLTK btn1 = LtkButton_New();
	LtkButton_SetText(btn1, "fuck");
	UINT idx= LtkSplitter_AddClient(splt1, btn1);
	LtkSplitter_SetClientSize(splt1, idx, 300);

	btn1 = LtkButton_New();
	LtkButton_SetText(btn1, "shit");
	idx = LtkSplitter_AddClient(splt1, btn1);
	LtkSplitter_SetClientSize(splt1, idx, 300);

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

	LtkWindow_CreateCenter(this->hWindow, NULL, 800, 600);
	LtkWindow_SetCaption(this->hWindow, "LTK轻量界面库设计器");
}

void DesignerWnd::BuildMenu()
{

}

int DesignerWnd::OnDestroy(void* userdata)
{
	::PostQuitMessage(0);
	return 0;
}

int DesignerWnd::OnClose(void* userdata, BOOL* proceed)
{
	DesignerWnd* self = ((ltk::RTTI*)userdata)->As<DesignerWnd>();

	if (::MessageBox(LtkWindow_GetHWND(self->hWindow), L"确定要退出吗?", 0, MB_OKCANCEL) == IDCANCEL) {
		*proceed = FALSE;
	}
	return 0;
}
