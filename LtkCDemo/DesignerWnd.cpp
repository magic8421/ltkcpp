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
	LtkRegisterCallback(this->hWindow, LTK_ACTION, (LtkCallback)DesignerWnd::OnAction, this);
	LtkWindow_SetCentralWidget(this->hWindow, splt1);

	this->hTreeView = LtkTreeView_New();
	UINT idx= LtkSplitter_AddClient(splt1, this->hTreeView);
	LtkSplitter_SetClientSize(splt1, idx, 300);
	this->hRootNode = LtkTreeNode_New();
	LtkTreeNode_SetText(this->hRootNode, "Window");
	LtkTreeNode_AddChild(LtkTreeView_GetRootNode(this->hTreeView), this->hRootNode);

	HLTK btn1 = LtkButton_New();
	LtkButton_SetText(btn1, "shit");
	idx = LtkSplitter_AddClient(splt1, btn1);
	LtkSplitter_SetClientSize(splt1, idx, 300);

	this->BuildMenu();
	LtkWindow_CreateCenter(this->hWindow, NULL, 800, 600);
	LtkWindow_SetCaption(this->hWindow, "LTK轻量界面库设计器");
}

int DesignerWnd::OnAction(void* userdata, LPCSTR name)
{
	DesignerWnd* self = ((ltk::RTTI*)userdata)->As<DesignerWnd>();
	if (!strcmp(name, "add_vbox")) {
		HLTK select = LtkTreeView_GetSelectedNode(self->hTreeView);
		if (select) {
			HLTK node = LtkTreeNode_New();
			LtkTreeNode_SetText(node, "VBox");
			LtkTreeNode_AddChild(select, node);
		}
	}
	else if (!strcmp(name, "add_hbox")) {
		HLTK select = LtkTreeView_GetSelectedNode(self->hTreeView);
		if (select) {
			HLTK node = LtkTreeNode_New();
			LtkTreeNode_SetText(node, "HBox");
			LtkTreeNode_AddChild(select, node);
		}
	}
	else if (!strcmp(name, "add_button")) {
		HLTK select = LtkTreeView_GetSelectedNode(self->hTreeView);
		if (select) {
			HLTK node = LtkTreeNode_New();
			LtkTreeNode_SetText(node, "Button");
			LtkTreeNode_AddChild(select, node);
		}
	}
	return 0;
}

void DesignerWnd::BuildMenu()
{
	HLTK menu_bar = LtkMenuBar_New();
	LtkMenuBar_AddItem(menu_bar, "文件");
	LtkMenuBar_AddItem(menu_bar, "编辑");
	LtkMenuBar_AddItem(menu_bar, "控件");
	LtkMenuBar_AddItem(menu_bar, "皮肤");
	LtkMenuBar_AddItem(menu_bar, "帮助");
	LtkWindow_SetMenu(this->hWindow, menu_bar);

	HLTK popup_menu = LtkPopupMenu_New();
	LtkPopupMenu_AddItem(popup_menu, "xml布局测试", "new_file");
	LtkPopupMenu_AddItem(popup_menu, "打开", "open_file");
	LtkPopupMenu_AddItem(popup_menu, "最近", "");
	LtkPopupMenu_AddItem(popup_menu, "关闭", "close_file");
	LtkPopupMenu_AddItem(popup_menu, "退出", "exit_app");
	LtkMenuBar_SetPopupMenu(menu_bar, 0, popup_menu);

	HLTK popup2 = LtkPopupMenu_New();
	LtkPopupMenu_SetWidth(popup2, 200);
	LtkPopupMenu_AddItem(popup2, "C:\\路径\\文件", "recent_file_01");
	LtkPopupMenu_AddItem(popup2, "C:\\路径\\文件", "recent_file_02");
	LtkPopupMenu_AddItem(popup2, "C:\\路径\\文件", "recent_file_03");
	LtkPopupMenu_SetSubMenu(popup_menu, 2, popup2);

	popup_menu = LtkPopupMenu_New();
	LtkPopupMenu_AddItem(popup_menu, "撤销", "undo_action");
	LtkPopupMenu_AddItem(popup_menu, "重做", "redo_action");
	LtkPopupMenu_AddSeparator(popup_menu);
	LtkPopupMenu_AddItem(popup_menu, "复制", "copy_action");
	LtkPopupMenu_AddItem(popup_menu, "剪切", "cut_action");
	LtkPopupMenu_AddItem(popup_menu, "粘贴", "paste_action");
	LtkMenuBar_SetPopupMenu(menu_bar, 1, popup_menu);

	popup_menu = LtkPopupMenu_New();
	LtkPopupMenu_AddItem(popup_menu, "VBox", "add_vbox");
	LtkPopupMenu_AddItem(popup_menu, "HBox", "add_hbox");
	LtkPopupMenu_AddItem(popup_menu, "Button", "add_button");
	LtkMenuBar_SetPopupMenu(menu_bar, 2, popup_menu);

	popup_menu = LtkPopupMenu_New();
	LtkPopupMenu_AddItem(popup_menu, "位图素材", "pixel_theme");
	LtkPopupMenu_AddItem(popup_menu, "暗色", "dark_theme");
	LtkPopupMenu_AddItem(popup_menu, "亮色", "light_theme");
	LtkMenuBar_SetPopupMenu(menu_bar, 3, popup_menu);

	popup_menu = LtkPopupMenu_New();
	LtkPopupMenu_AddItem(popup_menu, "在线手册", "online_manual");
	LtkPopupMenu_AddItem(popup_menu, "关于", "about_action");
	LtkMenuBar_SetPopupMenu(menu_bar, 4, popup_menu);
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
