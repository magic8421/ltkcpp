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

template<typename T>
T* ltk_cast(HLTK o)
{
	if (g_bApiCheck) {
		Object* obj = (Object*)o;
		if (!Object::CheckValid(obj)) {
			LTK_LOG("ltk C interface handle invalid: 0x%08x", o);
			__debugbreak();
		}
		if (!obj->Is(T::TypeIdClass())) {
			LTK_LOG("ltk C interface type mismatch, %s required, got %s",
				T::TypeNameClass(), obj->TypeNameInstance());
			__debugbreak();
		}
	}
	return (T *) o;
}

#define LTK_CHECK_TYPE_OR_RETURN(hltk, klass, name) \
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
	LTK_CHECK_TYPE_OR_RETURN(obj, Object, pobj);
	delete pobj;
}

LTK_API void WINAPI LtkDeleteLater(HLTK obj)
{
	if (!obj) return;
	LTK_CHECK_TYPE_OR_RETURN(obj, Object, pobj);
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
	LTK_CHECK_TYPE_OR_RETURN(o, Object, pobj);
	pobj->SetName(name);
}

LTK_API LPCSTR WINAPI LtkGetName(HLTK o)
{
	LTK_CHECK_TYPE_OR_RETURN(o, Object, pobj);
	return pobj->GetName();
}

LTK_API void WINAPI LtkRegisterCallback(HLTK obj, UINT event_id, LtkCallback cb, void* userdata)
{
	LTK_CHECK_TYPE_OR_RETURN(obj, Object, pobj);
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
	LTK_CHECK_TYPE_OR_RETURN(self, Window, thiz);
	thiz->Create(parent, rcf);
}

LTK_API void WINAPI LtkWindow_CreateCenter(HLTK self, HWND parent, float width, float height)
{
	Gdiplus::SizeF size(width, height);
	LTK_CHECK_TYPE_OR_RETURN(self, Window, thiz);
	thiz->Create(parent, size);
}

LTK_API void WINAPI LtkWindow_SetCaption(HLTK self, LPCSTR text)
{
	LTK_CHECK_TYPE_OR_RETURN(self, Window, thiz);
	thiz->SetCaption(LtkA2W(text).c_str());
}

LTK_API void WINAPI LtkWindow_SetBackground(HLTK self, LPCSTR name)
{
	LTK_CHECK_TYPE_OR_RETURN(self, Window, thiz);
	thiz->SetBackground(name);
}

LTK_API void WINAPI LtkWindow_UpdateTheme(HLTK self)
{
	LTK_CHECK_TYPE_OR_RETURN(self, Window, thiz);
	thiz->UpdateTheme();
}

LTK_API void WINAPI LtkWindow_SetCentralWidget(HLTK self, HLTK widget)
{
	LTK_CHECK_TYPE_OR_RETURN(self, Window, thiz);
	LTK_CHECK_TYPE_OR_RETURN(widget, Widget, w);
	thiz->SetCentralWidget(w);
}

LTK_API void WINAPI LtkWindow_SetMenu(HLTK self, HLTK menu_bar)
{
	LTK_CHECK_TYPE_OR_RETURN(self, Window, thiz);
	LTK_CHECK_TYPE_OR_RETURN(menu_bar, MenuBar, mb);
	thiz->SetMenu(mb);
}

LTK_API HWND WINAPI LtkWindow_GetHWND(HLTK self)
{
	LTK_CHECK_TYPE_OR_RETURN(self, Window, thiz);
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
	LTK_CHECK_TYPE_OR_RETURN(self, BoxLayout, thiz);
	LTK_CHECK_TYPE_OR_RETURN(widget, Widget, w);
	thiz->AddLayoutItem(w, preferedSize, growFactor);
}

LTK_API void WINAPI LtkBoxLayout_AddSpaceItem(HLTK self, float preferedSize, float growFactor)
{
	LTK_CHECK_TYPE_OR_RETURN(self, BoxLayout, thiz);
	thiz->AddSpaceItem(preferedSize, growFactor);
}

LTK_API void WINAPI LtkBoxLayout_SetSpacing(HLTK self, float spacing)
{
	LTK_CHECK_TYPE_OR_RETURN(self, BoxLayout, thiz);
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
	LTK_CHECK_TYPE_OR_RETURN(self, Button, thiz);
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
	HeaderCtrl* thiz = ltk_cast<HeaderCtrl>(self);
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
	LTK_CHECK_TYPE_OR_RETURN(self, ListView, thiz);
	auto header = thiz->GetHeaderCtrl();
	header->AddColumn(LtkA2W(text).c_str(), width);
}

LTK_API UINT WINAPI LtkListView_AddRow(HLTK self)
{
	ListView* thiz = ltk_cast<ListView>(self);
	return thiz->AddRow();
}

LTK_API void WINAPI LtkListView_SetCellText(HLTK self, UINT row, UINT col, LPCWSTR text)
{
	ListView* thiz = ltk_cast<ListView>(self);
	thiz->SetCellText(row, col, text);
}

LTK_API LPCWSTR WINAPI LtkListView_GetCellText(HLTK self, UINT row, UINT col)
{
	ListView* thiz = ltk_cast<ListView>(self);
	return thiz->GetCellText(row, col);
}

LTK_API int WINAPI LtkListView_GetSelectedRow(HLTK self)
{
	ListView* thiz = ltk_cast<ListView>(self);
	return thiz->GetSelectedRow();
}

LTK_API HLTK WINAPI LtkListView_GetHeaderCtrl(HLTK self)
{
	ListView* thiz = ltk_cast<ListView>(self);
	return (HLTK)thiz->GetHeaderCtrl();
}

LTK_API void WINAPI LtkListView_UpdateColumnWidth(HLTK self)
{
	ListView* thiz = ltk_cast<ListView>(self);
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
	Splitter* thiz = ltk_cast<Splitter>(self);
	return thiz->AddClient(ltk_cast<Widget>(widget));
}

LTK_API void WINAPI LtkSplitter_SetClientSize(HLTK self, UINT idx, float size)
{
	Splitter* thiz = ltk_cast<Splitter>(self);
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

LTK_API HLTK WINAPI LtkTreeView_GetRootNode(HLTK self)
{
	TreeView* thiz = ltk_cast<TreeView>(self);
	return (HLTK)thiz->GetRootNode();
}

//////////////////////////////////////////////////////////////////////////
// TreeNode
//////////////////////////////////////////////////////////////////////////


LTK_API HLTK WINAPI LtkTreeNode_New_(LPCSTR source, int line)
{
	auto obj = new TreeNode();
	//obj->SetSourceLine(source, line);
	return (HLTK)obj;
}

LTK_API void WINAPI LtkTreeNode_AddChild(HLTK self, HLTK node)
{
	TreeNode* thiz = (TreeNode*)self;
	thiz->AddChild((TreeNode*)node);
}

LTK_API void WINAPI LtkTreeNode_SetText(HLTK self, LPCSTR text)
{
	TreeNode* thiz = (TreeNode*)self;
	thiz->SetText(LtkA2W(text).c_str());
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
	MenuBar* thiz = ltk_cast<MenuBar>(self);
	return thiz->AddItem(LtkA2W(text).c_str());
}

LTK_API void WINAPI LtkMenuBar_SetPopupMenu(HLTK self, UINT idx, HLTK popup)
{
	MenuBar* thiz = ltk_cast<MenuBar>(self);
	thiz->SetPopupMenu(idx, ltk_cast<PopupMenu>(popup));
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
	PopupMenu* thiz = ltk_cast<PopupMenu>(self);
	thiz->AddItem(LtkA2W(text).c_str(), name);
}

LTK_API void WINAPI LtkPopupMenu_AddSeparator(HLTK self)
{
	PopupMenu* thiz = ltk_cast<PopupMenu>(self);
	thiz->AddSeparator();
}

LTK_API void WINAPI LtkPopupMenu_SetWidth(HLTK self, float width)
{
	PopupMenu* thiz = ltk_cast<PopupMenu>(self);
	thiz->SetWidth(width);
}

LTK_API void WINAPI LtkPopupMenu_SetSubMenu(HLTK self, UINT idx, HLTK popup)
{
	PopupMenu* thiz = ltk_cast<PopupMenu>(self);
	thiz->SetSubMenu(idx, ltk_cast<PopupMenu>(popup));
}