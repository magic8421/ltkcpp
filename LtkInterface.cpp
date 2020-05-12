#include "stdafx.h"
#include "LtkInterface.h"
#include "ltk.h"
#include "Object.h"
#include "Window.h"
#include "Button.h"
#include "BoxLayout.h"
#include "ListView.h"
#include "Splitter.h"
#include "TreeView.h"
#include "TextEdit.h"
#include "MenuBar.h"
#include "StyleManager.h"
#include "Builder.h"

using namespace ltk;

BOOL g_bApiCheck = TRUE;

LTK_API void WINAPI LtkEnalbeApiCheck(BOOL b)
{
	g_bApiCheck = b;
}

#define LTK_CAST(type, hltk) ltk_cast<type>((hltk), __FUNCTION__)

template<typename T>
T* ltk_cast(HLTK hltk, const char* func_name)
{
	if (g_bApiCheck) { 
		Object* _obj = (Object*)hltk; 
		if (!Object::CheckValid(_obj)) { 
			char buf[256]; 
			StringCbPrintfA(buf, sizeof(buf), "[%s]中句柄无效: 0x%08x", __FUNCTION__, hltk); 
			MessageBoxA(NULL, buf, NULL, 0);
			__debugbreak(); 
		}
		if (!_obj->Is(T::TypeIdClass())) { 
			char buf[256]; 
			StringCbPrintfA(buf, sizeof(buf), "[%s]中句柄类型错误,需要[%s],传入[%s]",
				 __FUNCTION__, T::TypeNameClass(), _obj->TypeNameInstance());
			MessageBoxA(NULL, buf, NULL, 0);
			__debugbreak(); 
		}
	}
	return reinterpret_cast<T *>(hltk);
}

#define LTK_CHECK_TYPE(hltk, klass, name) \
if (g_bApiCheck) { \
    Object* _obj = (Object*)hltk; \
	if (!Object::CheckValid(_obj)) { \
        char buf[256]; \
		StringCbPrintfA(buf, sizeof(buf), "[%s]中句柄无效: 0x%08x", __FUNCTION__, hltk); \
        MessageBoxA(NULL, buf, NULL, 0);\
		__debugbreak(); \
	}\
	if (!_obj->Is(klass::TypeIdClass())) { \
        char buf[256]; \
		StringCbPrintfA(buf, sizeof(buf), "[%s]中句柄类型错误,需要[%s],传入[%s]",\
			 __FUNCTION__, klass::TypeNameClass(), _obj->TypeNameInstance());\
        MessageBoxA(NULL, buf, NULL, 0);\
		__debugbreak(); \
	}\
}\
klass *name = (klass *)hltk; \


LTK_API UINT WINAPI LtkInitialize()
{
	ltk::LtkInitialize();
	return 0;
}

LTK_API void WINAPI LtkUninitialize()
{
	ltk::LtkUninitialize();
}

LTK_API void WINAPI LtkRunMessageLoop()
{
	MSG msg;
	BOOL bRet;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if (bRet == -1) {
			// ???
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

LTK_API void WINAPI LtkDelete(HLTK obj)
{
	if (!obj) return;
	LTK_CHECK_TYPE(obj, Object, pobj);
	delete pobj;
}

LTK_API void WINAPI LtkDeleteLater(HLTK obj)
{
	if (!obj) return;
	LTK_CHECK_TYPE(obj, Object, pobj);
	pobj->DeleteLater();
}

LTK_API BOOL WINAPI LtkIsValid(HLTK obj)
{
	if (!obj) return FALSE;
	if (g_bApiCheck) {
		return (BOOL)Object::CheckValid((Object*)obj);
	}
	else {
		return TRUE;
	}
}

LTK_API LPCSTR WINAPI LtkInternString(LPCSTR str)
{
	return Object::InternString(str);
}

LTK_API void WINAPI LtkSetName(HLTK o, LPCSTR name)
{
	LTK_CHECK_TYPE(o, Object, pobj);
	pobj->SetName(name);
}

LTK_API LPCSTR WINAPI LtkGetName(HLTK o)
{
	LTK_CHECK_TYPE(o, Object, pobj);
	return pobj->GetName();
}

LTK_API void WINAPI LtkRegisterCallback(HLTK obj, UINT event_id, LtkCallback cb, void* userdata)
{
	LTK_CHECK_TYPE(obj, Object, pobj);
	pobj->RegisterCallback(event_id, cb, userdata);
}

LTK_API void WINAPI LtkUnregisterCallback(HLTK obj, UINT event_id, LtkCallback cb, void* userdata)
{
	// TODO
}

LTK_API HLTK WINAPI LtkGetEventSender()
{
	return (HLTK)Object::GetDelegateInvoker();
}

LTK_API HLTK WINAPI LtkBuildFromXml(LPCSTR path)
{
	return (HLTK)Builder::Instance()->WidgetFromXml(path);
}



//////////////////////////////////////////////////////////////////////////
// Widget
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Window
//////////////////////////////////////////////////////////////////////////


LTK_API HLTK WINAPI LtkWindow_New_(LPCSTR source, int line)
{
	auto obj = new Window;
	obj->SetSourceLine(source, line);
	return (HLTK)obj;
}

LTK_API BOOL WINAPI LtkWindow_CheckType(HLTK hltk)
{
	return reinterpret_cast<Object*>(hltk)->Is(Window::TypeIdClass());
}

LTK_API void WINAPI LtkWindow_Create(HLTK self, HWND parent, LtkRect* rc)
{
	Gdiplus::RectF rcf(rc->x, rc->y, rc->width, rc->height);
	LTK_CHECK_TYPE(self, Window, thiz);
	thiz->Create(parent, rcf);
}

LTK_API void WINAPI LtkWindow_CreateCenter(HLTK self, HWND parent, float width, float height)
{
	Gdiplus::SizeF size(width, height);
	LTK_CHECK_TYPE(self, Window, thiz);
	thiz->Create(parent, size);
}

LTK_API void WINAPI LtkWindow_SetCaption(HLTK self, LPCSTR text)
{
	LTK_CHECK_TYPE(self, Window, thiz);
	thiz->SetCaption(LtkA2W(text).c_str());
}

LTK_API void WINAPI LtkWindow_SetBackground(HLTK self, LPCSTR name)
{
	LTK_CHECK_TYPE(self, Window, thiz);
	thiz->SetBackground(name);
}

LTK_API void WINAPI LtkWindow_UpdateTheme(HLTK self)
{
	LTK_CHECK_TYPE(self, Window, thiz);
	thiz->UpdateTheme();
}

LTK_API void WINAPI LtkWindow_SetCentralWidget(HLTK self, HLTK widget)
{
	LTK_CHECK_TYPE(self, Window, thiz);
	LTK_CHECK_TYPE(widget, Widget, w);
	thiz->SetCentralWidget(w);
}

LTK_API void WINAPI LtkWindow_SetMenu(HLTK self, HLTK menu_bar)
{
	LTK_CHECK_TYPE(self, Window, thiz);
	LTK_CHECK_TYPE(menu_bar, MenuBar, mb);
	thiz->SetMenu(mb);
}

LTK_API HWND WINAPI LtkWindow_GetHWND(HLTK self)
{
	LTK_CHECK_TYPE(self, Window, thiz);
	return thiz->GetHWND();
}

//////////////////////////////////////////////////////////////////////////
// StyleManager
//////////////////////////////////////////////////////////////////////////

LTK_API void WINAPI LtkStyleManager_SwitchTheme(LPCSTR name)
{
	StyleManager::Instance()->SwitchTheme(name);
}


//////////////////////////////////////////////////////////////////////////
// BoxLayout
//////////////////////////////////////////////////////////////////////////

LTK_API HLTK WINAPI LtkBoxLayout_New_(UINT orientation, LPCSTR source, int line)
{
	auto obj = new BoxLayout((ltk::Orientation)orientation);
	obj->SetSourceLine(source, line);
	return (HLTK)obj;
}

LTK_API void WINAPI LtkBoxLayout_AddLayoutItem(
	HLTK self, HLTK widget, float preferedSize, float growFactor)
{
	LTK_CHECK_TYPE(self, BoxLayout, thiz);
	LTK_CHECK_TYPE(widget, Widget, w);
	thiz->AddLayoutItem(w, preferedSize, growFactor);
}

LTK_API void WINAPI LtkBoxLayout_AddSpaceItem(HLTK self, float preferedSize, float growFactor)
{
	LTK_CHECK_TYPE(self, BoxLayout, thiz);
	thiz->AddSpaceItem(preferedSize, growFactor);
}

LTK_API void WINAPI LtkBoxLayout_SetSpacing(HLTK self, float spacing)
{
	LTK_CHECK_TYPE(self, BoxLayout, thiz);
	thiz->SetSpacing(spacing);
}

//////////////////////////////////////////////////////////////////////////
// Button
//////////////////////////////////////////////////////////////////////////

LTK_API HLTK WINAPI LtkButton_New_(LPCSTR source, int line)
{
	auto obj = new Button;
	obj->SetSourceLine(source, line);
	return (HLTK)obj;
}

LTK_API void WINAPI LtkButton_SetText(HLTK self, LPCSTR text)
{
	LTK_CHECK_TYPE(self, Button, thiz);
	thiz->SetText(LtkA2W(text).c_str());
}

//////////////////////////////////////////////////////////////////////////
// HeaderCtrl
//////////////////////////////////////////////////////////////////////////

LTK_API HLTK WINAPI LtkHeaderCtrl_New_(LPCSTR source, int line)
{
	auto obj = new HeaderCtrl();
	obj->SetSourceLine(source, line);
	return (HLTK)obj;
}

LTK_API void WINAPI LtkHeaderCtrl_AddColumn(HLTK self, LPCSTR text, float width)
{
	HeaderCtrl* thiz = LTK_CAST(HeaderCtrl, self);
	thiz->AddColumn(LtkA2W(text).c_str(), width);
}

//////////////////////////////////////////////////////////////////////////
// ListView
//////////////////////////////////////////////////////////////////////////

LTK_API HLTK WINAPI LtkListView_New_(LPCSTR source, int line)
{
	return (HLTK)new ListView;
}

LTK_API void WINAPI LtkListView_AddColumn(HLTK self, LPCSTR text, float width)
{
	LTK_CHECK_TYPE(self, ListView, thiz);
	auto header = thiz->GetHeaderCtrl();
	header->AddColumn(LtkA2W(text).c_str(), width);
}

LTK_API UINT WINAPI LtkListView_AddRow(HLTK self)
{
	ListView* thiz = LTK_CAST(ListView, self);
	return thiz->AddRow();
}

LTK_API void WINAPI LtkListView_SetCellText(HLTK self, UINT row, UINT col, LPCWSTR text)
{
	ListView* thiz = LTK_CAST(ListView, self);
	thiz->SetCellText(row, col, text);
}

LTK_API LPCWSTR WINAPI LtkListView_GetCellText(HLTK self, UINT row, UINT col)
{
	ListView* thiz = LTK_CAST(ListView, self);
	return thiz->GetCellText(row, col);
}

LTK_API int WINAPI LtkListView_GetSelectedRow(HLTK self)
{
	ListView* thiz = LTK_CAST(ListView, self);
	return thiz->GetSelectedRow();
}

LTK_API HLTK WINAPI LtkListView_GetHeaderCtrl(HLTK self)
{
	ListView* thiz = LTK_CAST(ListView, self);
	return (HLTK)thiz->GetHeaderCtrl();
}

LTK_API void WINAPI LtkListView_UpdateColumnWidth(HLTK self)
{
	ListView* thiz = LTK_CAST(ListView, self);
	thiz->UpdateColumnWidth();
}

//////////////////////////////////////////////////////////////////////////
// Splitter
//////////////////////////////////////////////////////////////////////////

LTK_API HLTK WINAPI LtkSplitter_New_(UINT orientation, LPCSTR source, int line)
{
	auto obj = new Splitter((Orientation)orientation);
	obj->SetSourceLine(source, line);
	return (HLTK)obj;
}

LTK_API UINT WINAPI LtkSplitter_AddClient(HLTK self, HLTK widget)
{
	Splitter* thiz = LTK_CAST(Splitter, self);
	return thiz->AddClient(LTK_CAST(Widget, widget));
}

LTK_API void WINAPI LtkSplitter_SetClientSize(HLTK self, UINT idx, float size)
{
	Splitter* thiz = LTK_CAST(Splitter, self);
	thiz->SetClientSize(idx, size);
}

//////////////////////////////////////////////////////////////////////////
// TreeView
//////////////////////////////////////////////////////////////////////////

LTK_API HLTK WINAPI LtkTreeView_New_(LPCSTR source, int line)
{
	auto obj = new TreeView();
	obj->SetSourceLine(source, line);
	return (HLTK)obj;
}

LTK_API HTREENODE WINAPI LtkTreeView_GetRootNode(HLTK self)
{
	LTK_CHECK_TYPE(self, TreeView, thiz);
	return (HTREENODE)thiz->GetRootNode();
}

LTK_API HTREENODE WINAPI LtkTreeView_GetSelectedNode(HLTK self)
{
	LTK_CHECK_TYPE(self, TreeView, thiz);
	return (HTREENODE)thiz->GetSelectedNode();
}

//////////////////////////////////////////////////////////////////////////
// TreeNode
//////////////////////////////////////////////////////////////////////////

#define LTK_HTREENODE(tree, node) LtkCheckTreeNode(tree, node, __FUNCTION__)


TreeNode* LtkCheckTreeNode(TreeView* tree, HTREENODE node, LPCSTR func_name)
{
	if (g_bApiCheck) {
		if (!tree->CheckNode(node)) {
			char buf[256];
			StringCbPrintfA(buf, sizeof(buf), "[%s]中句柄无效: 0x%08x", func_name, node);
			MessageBoxA(NULL, buf, NULL, 0);
			__debugbreak();
		}
	}
	return reinterpret_cast<TreeNode*>(node);
}

//LTK_API HTREENODE WINAPI LtkTreeView_NewNode(HLTK self)
//{
//	auto obj = new TreeNode();
//	LTK_CAST(TreeView, self)->RegisterNode((HTREENODE)obj);
//	return (HTREENODE)obj;
//}

LTK_API HTREENODE WINAPI LtkTreeView_AddChildNode(HLTK self, HTREENODE parent)
{
	auto thiz = LTK_CAST(TreeView, self);
	auto node = new TreeNode;
	LTK_HTREENODE(thiz, parent)->AddChild(node);
	return reinterpret_cast<HTREENODE>(node);
}

LTK_API void WINAPI LtkTreeView_SetNodeText(HLTK self, HTREENODE node, LPCSTR text)
{
	auto thiz = LTK_CAST(TreeView, self);
	LTK_HTREENODE(thiz, node)->SetText(LtkA2W(text).c_str());
}

LTK_API void WINAPI LtkTreeView_SetNodeUserdata(HLTK self, HTREENODE node, void* userdata)
{
	auto thiz = LTK_CAST(TreeView, self);
	LTK_HTREENODE(thiz, node)->SetUserData(userdata);
}

LTK_API LPCSTR WINAPI LtkTreeView_GetNodeText(HLTK self, HTREENODE node)
{
	auto thiz = LTK_CAST(TreeView, self);
	static std::string strText = LtkW2A(LTK_HTREENODE(thiz, node)->GetText());
	return strText.c_str();
}

//////////////////////////////////////////////////////////////////////////
// TextEdit
//////////////////////////////////////////////////////////////////////////

LTK_API BOOL WINAPI LtkIsTextEdit(HLTK o)
{
	Object *obj = (Object *)o;
	if (!Object::CheckValid(obj)) return FALSE;
	return obj->Is(TextEdit::TypeIdClass()) ? TRUE : FALSE;
}

LTK_API HLTK WINAPI LtkTextEdit_New_(LPCSTR source, int line)
{
	auto obj = new TextEdit();
	obj->SetSourceLine(source, line);
	return (HLTK)obj;
}

//////////////////////////////////////////////////////////////////////////
// MenuBar
//////////////////////////////////////////////////////////////////////////


LTK_API HLTK WINAPI LtkMenuBar_New_(LPCSTR source, int line)
{
	auto obj = new MenuBar();
	obj->SetSourceLine(source, line);
	return (HLTK)obj;
}

LTK_API UINT WINAPI LtkMenuBar_AddItem(HLTK self, LPCSTR text)
{
	MenuBar* thiz = LTK_CAST(MenuBar, self);
	return thiz->AddItem(LtkA2W(text).c_str());
}

LTK_API void WINAPI LtkMenuBar_SetPopupMenu(HLTK self, UINT idx, HLTK popup)
{
	MenuBar* thiz = LTK_CAST(MenuBar, self);
	thiz->SetPopupMenu(idx, LTK_CAST(PopupMenu, popup));
}

//////////////////////////////////////////////////////////////////////////
// PopupMenu
//////////////////////////////////////////////////////////////////////////

LTK_API HLTK WINAPI LtkPopupMenu_New_(LPCSTR source, int line)
{
	auto obj = new PopupMenu();
	obj->SetSourceLine(source, line);
	return (HLTK)obj;
}

LTK_API void WINAPI LtkPopupMenu_AddItem(HLTK self, LPCSTR text, LPCSTR name)
{
	PopupMenu* thiz = LTK_CAST(PopupMenu, self);
	thiz->AddItem(LtkA2W(text).c_str(), name);
}

LTK_API void WINAPI LtkPopupMenu_AddSeparator(HLTK self)
{
	PopupMenu* thiz = LTK_CAST(PopupMenu, self);
	thiz->AddSeparator();
}

LTK_API void WINAPI LtkPopupMenu_SetWidth(HLTK self, float width)
{
	PopupMenu* thiz = LTK_CAST(PopupMenu, self);
	thiz->SetWidth(width);
}

LTK_API void WINAPI LtkPopupMenu_SetSubMenu(HLTK self, UINT idx, HLTK popup)
{
	PopupMenu* thiz = LTK_CAST(PopupMenu, self);
	thiz->SetSubMenu(idx, LTK_CAST(PopupMenu, popup));
}