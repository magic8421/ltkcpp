// LtkCDemo.cpp : 定义应用程序的入口点。
//

#include "pch.h"
#include "framework.h"
#include "LtkCDemo.h"
#include "LtkInterface.h"


void CALLBACK MyEventCallback(void* userdata)
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
		LtkObject *node = LtkTreeNode_New();
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
	LtkInitialize();

	LtkObject* wnd = LtkWindow_New();
	//LtkWindow_SetBackground(wnd, "window_bg");
	LtkObject_RegisterCallback(
		(LtkObject*)wnd, LTK_WINDOW_DESTROY, (LtkCallback)MyEventCallback, NULL);

	LtkObject* splitter_h = LtkSplitter_New(LTK_HORIZONTAL);
	LtkWindow_SetClientSprite(LTK_WINDOW(wnd), LTK_SPRITE(splitter_h));

	LtkSplitter_Resize(LTK_SPLITTER(splitter_h), 2);

	LtkObject* tree_view = LtkTreeView_New();
	RecBuildNodes(LTK_TREENODE(
		LtkTreeView_GetRootNode(LTK_TREEVIEW(tree_view))), 0);

	LtkObject* btn = NULL;
	//LtkObject* btn = LtkButton_New();
	LtkSplitter_SetClientAt(LTK_SPLITTER(splitter_h), 0, LTK_SPRITE(tree_view));
	LtkSplitter_SetClientSize(LTK_SPLITTER(splitter_h), 0, 200);

	LtkObject* splitter_v = LtkSplitter_New(LTK_VERTICAL);
	LtkSplitter_Resize(LTK_SPLITTER(splitter_v), 2);
	LtkSplitter_SetClientAt(LTK_SPLITTER(splitter_h), 1, LTK_SPRITE(splitter_v));

	LtkObject* list_view = LtkListView_New();
	LtkObject* header = LtkListView_GetHeaderCtrl(LTK_LISTVIEW(list_view));
	LtkHeaderCtrl_AddColumn(LTK_HEADERCTRL(header), L"项目名", 100);
	LtkHeaderCtrl_AddColumn(LTK_HEADERCTRL(header), L"工程名", 200);
	LtkHeaderCtrl_AddColumn(LTK_HEADERCTRL(header), L"负责人", 200);
	LtkListView_UpdateColumnWidth(LTK_LISTVIEW(list_view));

	UINT num = rand() % 200;
	for (UINT i = 0; i < num; i++) {
		wchar_t buf[128];
		LtkListView_AddRow(LTK_LISTVIEW(list_view));
		StringCbPrintf(buf, sizeof(buf), L"item:%d", i);
		LtkListView_SetCellText(LTK_LISTVIEW(list_view), i, 0, buf);
		StringCbPrintf(buf, sizeof(buf), L"subitem1:%d", i);
		LtkListView_SetCellText(LTK_LISTVIEW(list_view), i, 1, buf);
		StringCbPrintf(buf, sizeof(buf), L"subitem2:%d", i);
		LtkListView_SetCellText(LTK_LISTVIEW(list_view), i, 2, buf);
	}

	LtkSplitter_SetClientAt(LTK_SPLITTER(splitter_v), 0, LTK_SPRITE(list_view));
	LtkSplitter_SetClientSize(LTK_SPLITTER(splitter_v), 0, 350);
	
	LtkObject* text_edit = LtkTextEdit_New();
	LtkSplitter_SetClientAt(LTK_SPLITTER(splitter_v), 1, LTK_SPRITE(text_edit));

	LtkObject* menu_bar = LtkMenuBar_New();
	LtkMenuBar_AddItem(LTK_MENUBAR(menu_bar), L"文件");
	LtkMenuBar_AddItem(LTK_MENUBAR(menu_bar), L"编辑");
	LtkMenuBar_AddItem(LTK_MENUBAR(menu_bar), L"皮肤");
	LtkMenuBar_AddItem(LTK_MENUBAR(menu_bar), L"帮助");
	LtkWindow_SetMenu(LTK_WINDOW(wnd), LTK_MENUBAR(menu_bar));

	LtkObject* popup_menu = LtkPopupMenu_New();
	LtkPopupMenu_AddItem(LTK_POPUPMENU(popup_menu), L"新建");
	LtkPopupMenu_AddItem(LTK_POPUPMENU(popup_menu), L"打开");
	LtkPopupMenu_AddItem(LTK_POPUPMENU(popup_menu), L"最近");
	LtkPopupMenu_AddItem(LTK_POPUPMENU(popup_menu), L"关闭");
	LtkPopupMenu_AddItem(LTK_POPUPMENU(popup_menu), L"退出");
	LtkMenuBar_SetPopupMenu(LTK_MENUBAR(menu_bar), 0, LTK_POPUPMENU(popup_menu));

	LtkObject* popup2 = LtkPopupMenu_New();
	LtkPopupMenu_SetWidth(LTK_POPUPMENU(popup2), 200);
	LtkPopupMenu_AddItem(LTK_POPUPMENU(popup2), L"C:\\路径\\文件");
	LtkPopupMenu_AddItem(LTK_POPUPMENU(popup2), L"C:\\路径\\文件");
	LtkPopupMenu_AddItem(LTK_POPUPMENU(popup2), L"C:\\路径\\文件");
	LtkPopupMenu_SetSubMenu(LTK_POPUPMENU(popup_menu), 2, LTK_POPUPMENU(popup2));

	LtkWindow_CreateCenter(LTK_WINDOW(wnd), NULL, 800, 600);
	LtkWindow_SetCaption(LTK_WINDOW(wnd), L"Ltk测试窗口");
	LtkWindow_UpdateTheme(LTK_WINDOW(wnd));

	LtkRunMessageLoop();

	LtkFree((LtkObject *)wnd);
	LtkUninitialize();
	return 0;
}


