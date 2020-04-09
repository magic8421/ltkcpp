// LtkCDemo.cpp : 定义应用程序的入口点。
//

#include "pch.h"
#include "framework.h"
#include "LtkCDemo.h"
#include "LtkInterface.h"

struct DemoData
{
	HLTK main_wnd;
	HLTK builder_wnd;
} g_data;

void LtkLogImpl(const char *source, int line, const char *format, ...)
{
	const size_t SIZE = 1024 * 2;
	char buffer[SIZE];
	va_list varg;
	va_start(varg, format);
	::StringCbVPrintfA(buffer, SIZE, format, varg);
	va_end(varg);
	char buffer2[SIZE];
	::StringCbPrintfA(buffer2, SIZE, "%s\t%s(%d)\r\n", buffer, source, line);

	::OutputDebugStringA(buffer2);
}

int CALLBACK OnXmlWindowDestroy(void* userdata)
{
	DemoData* self = (DemoData*)userdata;
	//LtkDelete(self->builder_wnd); // TODO 这里会嵌套然后完蛋
	self->builder_wnd = NULL;
	::EnableWindow(LtkWindow_GetHWND(self->main_wnd), TRUE);
	::SetFocus(LtkWindow_GetHWND(self->main_wnd));
	return 0;
}

int CALLBACK OnAction(void* userdata, LPCSTR name)
{
	LTK_LOG("OnAction: %s", name);
	DemoData* self = (DemoData*)userdata;
	
	if (!strcmp(name, "exit_app")) {
		//::PostMessage(LtkWindow_GetHWND(self->main_wnd), WM_CLOSE, 0, 0); 
		//LtkDeleteLater(self->main_wnd);
		// 这里并不能弹出确认对话框 怀疑是因为下面一句 跟踪调试发现确实调用到了MessageBox
		// 但没有显示出来 可能是PostQuitMessage会导致消息循环直接退出
		::PostQuitMessage(0);
	}
	else if (!strcmp(name, "new_file")) {
		if (!self->builder_wnd) {
			self->builder_wnd = LtkWindow_New();
			LtkRegisterCallback( self->builder_wnd,
				LTK_OBJECT_DELETE, (LtkCallback)OnXmlWindowDestroy, self);
			HLTK tree = LtkBuildFromXml("res\\test_tree.xml");
			LtkWindow_SetCentralWidget(self->builder_wnd, tree);
			LtkWindow_CreateCenter(self->builder_wnd, LtkWindow_GetHWND(self->main_wnd),
				600, 450);
			::EnableWindow(LtkWindow_GetHWND(self->main_wnd), FALSE);
			//LtkWindow_UpdateTheme(self->builder_wnd);
		}
	}
	return 0;
}

int CALLBACK OnWindowClose(void* userdata, BOOL* proceed)
{
	HLTK wnd = LtkGetEventSender();
	if (::MessageBox(LtkWindow_GetHWND(wnd), L"close ?", 0, MB_OKCANCEL) == IDCANCEL) {
		*proceed = FALSE;
	}
	return 0;
}

int CALLBACK OnWindowDestroy(void* userdata)
{
	DemoData* self = (DemoData*)userdata;
	self->main_wnd = NULL;
	::PostQuitMessage(0);
	return 0;
}

static int node_count = 0;

static void RecBuildNodes(HLTK parent, int depth)
{
	if (depth > 5) {
		return;
	}
	int num = rand() % 13 + 3;
	wchar_t buf[128];
	for (int i = 0; i < num; i++) {
		HLTK node = LtkTreeNode_New();
		StringCbPrintf(buf, sizeof(buf), L"TreeNode: %d", node_count);
		LtkTreeNode_SetText(node, buf); // 内层循环建议不做类型检查
		LtkTreeNode_AddChild(parent, node);
		node_count++;
		if (rand() % 100 < 40) {
			RecBuildNodes(node, depth + 1);
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
	g_data.main_wnd = wnd;
	//LtkWindow_SetBackground(wnd, "window_bg");
	LtkRegisterCallback(
		wnd, LTK_OBJECT_DELETE, (LtkCallback)OnWindowDestroy, &g_data);
	LtkRegisterCallback(
		wnd, LTK_WINDOW_CLOSE, (LtkCallback)OnWindowClose, &g_data);
	LtkRegisterCallback(
		wnd, LTK_ACTION, (LtkCallback)OnAction, &g_data);

	HLTK splitter_h = LtkSplitter_New(LTK_HORIZONTAL);
	LtkWindow_SetCentralWidget(wnd, splitter_h);

	HLTK tree_view = LtkTreeView_New();
	auto start = ::GetTickCount();
	//LtkEnalbeApiCheck(FALSE);
	RecBuildNodes(LtkTreeView_GetRootNode(tree_view), 0);
	//LtkEnalbeApiCheck(TRUE);
	LTK_LOG("prepare tree node: %d, time: %dms", node_count, ::GetTickCount() - start);

	HLTK btn = NULL;
	//HLTK btn = LtkButton_New();
	UINT idx = LtkSplitter_AddClient(splitter_h, tree_view);
	LtkSplitter_SetClientSize(splitter_h, idx, 200);

	HLTK splitter_v = LtkSplitter_New(LTK_VERTICAL);
	LtkSplitter_AddClient(splitter_h, splitter_v);

	HLTK list_view = LtkListView_New();
	HLTK header = LtkListView_GetHeaderCtrl(list_view);
	LtkHeaderCtrl_AddColumn(header, "项目名", 100);
	LtkHeaderCtrl_AddColumn(header, "工程名", 200);
	LtkHeaderCtrl_AddColumn(header, "负责人", 200);
	LtkHeaderCtrl_AddColumn(header, "资金", 100);
	LtkListView_UpdateColumnWidth(list_view);

	UINT num = rand() % 200;
	for (UINT i = 0; i < num; i++) {
		wchar_t buf[128];
		LtkListView_AddRow(list_view);
		StringCbPrintf(buf, sizeof(buf), L"col:0 row:%d", i);
		LtkListView_SetCellText(list_view, i, 0, buf);
		StringCbPrintf(buf, sizeof(buf), L"col:1 row:%d", i);
		LtkListView_SetCellText(list_view, i, 1, buf);
		StringCbPrintf(buf, sizeof(buf), L"col:2 row:%d", i);
		LtkListView_SetCellText(list_view, i, 2, buf);
		StringCbPrintf(buf, sizeof(buf), L"col:3 row:%d", i);
		LtkListView_SetCellText(list_view, i, 3, buf);
	}

	idx = LtkSplitter_AddClient(splitter_v, list_view);
	LtkSplitter_SetClientSize(splitter_v, idx, 350);
	
	HLTK text_edit = LtkTextEdit_New();
	LtkSplitter_AddClient(splitter_v, text_edit);

	HLTK menu_bar = LtkMenuBar_New();
	LtkMenuBar_AddItem(menu_bar, L"文件");
	LtkMenuBar_AddItem(menu_bar, L"编辑");
	LtkMenuBar_AddItem(menu_bar, L"皮肤");
	LtkMenuBar_AddItem(menu_bar, L"帮助");
	LtkWindow_SetMenu(wnd, menu_bar);

	HLTK popup_menu = LtkPopupMenu_New();
	LtkPopupMenu_AddItem(popup_menu, "新建", "new_file");
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
	LtkPopupMenu_AddItem(popup_menu, "位图素材", "pixel_theme");
	LtkPopupMenu_AddItem(popup_menu, "暗色", "dark_theme");
	LtkPopupMenu_AddItem(popup_menu, "亮色", "light_theme");
	LtkMenuBar_SetPopupMenu(menu_bar, 2, popup_menu);

	popup_menu = LtkPopupMenu_New();
	LtkPopupMenu_AddItem(popup_menu, "在线手册", "online_manual");
	LtkPopupMenu_AddItem(popup_menu, "关于", "about_action");
	LtkMenuBar_SetPopupMenu(menu_bar, 3, popup_menu);

	LtkWindow_CreateCenter(wnd, NULL, 800, 600);
	LtkWindow_SetCaption(wnd, "Ltk测试窗口");
	//LtkWindow_UpdateTheme(wnd);

	LtkRunMessageLoop();
	LtkDelete(g_data.builder_wnd);
	LtkDelete(g_data.main_wnd);

	LtkUninitialize();
	return 0;
}


