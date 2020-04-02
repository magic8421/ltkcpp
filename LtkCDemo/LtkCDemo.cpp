// LtkCDemo.cpp : 定义应用程序的入口点。
//

#include "pch.h"
#include "framework.h"
#include "LtkCDemo.h"
#include "LtkInterface.h"

void CALLBACK OnWindowClose(void* userdata, BOOL* proceed, BOOL* bHandled)
{
	HLTK wnd = LtkGetEventSender();
	if (::MessageBox(LtkWindow_GetHWND(wnd), L"close ?", 0, MB_OKCANCEL) == IDCANCEL) {
		*proceed = FALSE;
	}
}

void CALLBACK OnWindowDestroy(void* userdata, BOOL *bHandled)
{
	::PostQuitMessage(0);
}

static int node_count = 0;

static void RecBuildNodes(LtkTreeNode *parent, int depth)
{
	if (depth > 4) {
		return;
	}
	int num = rand() % 13 + 3;
	wchar_t buf[128];
	for (int i = 0; i < num; i++) {
		HLTK node = LtkTreeNode_New();
		StringCbPrintf(buf, sizeof(buf), L"TreeNode: %d", node_count);
		LtkTreeNode_SetText((LtkTreeNode*)node, buf); // 内层循环建议不做类型检查
		LtkTreeNode_AddChild((LtkTreeNode*)parent, (LtkTreeNode*)node);
		node_count++;
		if (rand() % 100 < 35) {
			RecBuildNodes((LtkTreeNode*)node, depth + 1);
		}
	}
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	LtkInitialize();

	HLTK wnd = LtkWindow_New();
	//LtkWindow_SetBackground(wnd, "window_bg");
	LtkRegisterCallback(
		wnd, LTK_WINDOW_DESTROY, (LtkCallback)OnWindowDestroy, NULL);
	LtkRegisterCallback(
		wnd, LTK_WINDOW_CLOSE, (LtkCallback)OnWindowClose, NULL);

	HLTK splitter_h = LtkSplitter_New(LTK_HORIZONTAL);
	LtkWindow_SetCentralWidget(wnd, splitter_h);

	LtkSplitter_Resize(splitter_h, 2);

	HLTK tree_view = LtkTreeView_New();
	RecBuildNodes(LTK_TREENODE(
		LtkTreeView_GetRootNode(LTK_TREEVIEW(tree_view))), 0);

	HLTK btn = NULL;
	//HLTK btn = LtkButton_New();
	LtkSplitter_SetClientAt(splitter_h, 0, tree_view);
	LtkSplitter_SetClientSize(splitter_h, 0, 200);

	HLTK splitter_v = LtkSplitter_New(LTK_VERTICAL);
	LtkSplitter_Resize(splitter_v, 2);
	LtkSplitter_SetClientAt(splitter_h, 1, splitter_v);

	HLTK list_view = LtkListView_New();
	HLTK header = LtkListView_GetHeaderCtrl(LTK_LISTVIEW(list_view));
	LtkHeaderCtrl_AddColumn(LTK_HEADERCTRL(header), L"项目名", 100);
	LtkHeaderCtrl_AddColumn(LTK_HEADERCTRL(header), L"工程名", 200);
	LtkHeaderCtrl_AddColumn(LTK_HEADERCTRL(header), L"负责人", 200);
	LtkHeaderCtrl_AddColumn(LTK_HEADERCTRL(header), L"资金", 100);
	LtkListView_UpdateColumnWidth(LTK_LISTVIEW(list_view));

	UINT num = rand() % 200;
	for (UINT i = 0; i < num; i++) {
		wchar_t buf[128];
		LtkListView_AddRow(LTK_LISTVIEW(list_view));
		StringCbPrintf(buf, sizeof(buf), L"col:0 row:%d", i);
		LtkListView_SetCellText(LTK_LISTVIEW(list_view), i, 0, buf);
		StringCbPrintf(buf, sizeof(buf), L"col:1 row:%d", i);
		LtkListView_SetCellText(LTK_LISTVIEW(list_view), i, 1, buf);
		StringCbPrintf(buf, sizeof(buf), L"col:2 row:%d", i);
		LtkListView_SetCellText(LTK_LISTVIEW(list_view), i, 2, buf);
		StringCbPrintf(buf, sizeof(buf), L"col:3 row:%d", i);
		LtkListView_SetCellText(LTK_LISTVIEW(list_view), i, 3, buf);
	}

	LtkSplitter_SetClientAt(splitter_v, 0, list_view);
	LtkSplitter_SetClientSize(splitter_v, 0, 350);
	
	HLTK text_edit = LtkTextEdit_New();
	LtkSplitter_SetClientAt(splitter_v, 1, text_edit);

	HLTK menu_bar = LtkMenuBar_New();
	LtkMenuBar_AddItem(LTK_MENUBAR(menu_bar), L"文件");
	LtkMenuBar_AddItem(LTK_MENUBAR(menu_bar), L"编辑");
	LtkMenuBar_AddItem(LTK_MENUBAR(menu_bar), L"皮肤");
	LtkMenuBar_AddItem(LTK_MENUBAR(menu_bar), L"帮助");
	LtkWindow_SetMenu(wnd, menu_bar);

	HLTK popup_menu = LtkPopupMenu_New();
	LtkPopupMenu_AddItem(LTK_POPUPMENU(popup_menu), L"新建", "");
	LtkPopupMenu_AddItem(LTK_POPUPMENU(popup_menu), L"打开", "");
	LtkPopupMenu_AddItem(LTK_POPUPMENU(popup_menu), L"最近", "");
	LtkPopupMenu_AddItem(LTK_POPUPMENU(popup_menu), L"关闭", "");
	LtkPopupMenu_AddItem(LTK_POPUPMENU(popup_menu), L"退出", "");
	LtkMenuBar_SetPopupMenu(LTK_MENUBAR(menu_bar), 0, LTK_POPUPMENU(popup_menu));

	HLTK popup2 = LtkPopupMenu_New();
	LtkPopupMenu_SetWidth(LTK_POPUPMENU(popup2), 200);
	LtkPopupMenu_AddItem(LTK_POPUPMENU(popup2), L"C:\\路径\\文件", "");
	LtkPopupMenu_AddItem(LTK_POPUPMENU(popup2), L"C:\\路径\\文件", "");
	LtkPopupMenu_AddItem(LTK_POPUPMENU(popup2), L"C:\\路径\\文件", "");
	LtkPopupMenu_SetSubMenu(LTK_POPUPMENU(popup_menu), 2, LTK_POPUPMENU(popup2));

	LtkWindow_CreateCenter(wnd, NULL, 800, 600);
	LtkWindow_SetCaption(wnd, L"Ltk测试窗口");
	LtkWindow_UpdateTheme(wnd);

	LtkRunMessageLoop();

	LtkFree(wnd);
	LtkUninitialize();
	return 0;
}


