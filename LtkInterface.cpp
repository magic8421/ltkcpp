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

static BOOL sApiCheck = TRUE;

LTK_API void WINAPI LtkEnalbeApiCheck(BOOL b)
{
	sApiCheck = b;
}

template<typename T>
T* ltk_cast(HLTK o)
{
	if (sApiCheck) {
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

LTK_API void WINAPI LtkFree(HLTK obj)
{
	if (!obj) return;
	Object *pobj = ltk_cast<Object>(obj);
	delete pobj;
}

LTK_API LPCSTR WINAPI LtkInternString(LPCSTR str)
{
	return Object::InternString(str);
}

LTK_API void WINAPI LtkSetName(HLTK o, LPCSTR name)
{
	ltk_cast<Object>(o)->SetName(name);
}

LTK_API LPCSTR WINAPI LtkGetName(HLTK o)
{
	return ltk_cast<Object>(o)->GetName();
}

LTK_API void WINAPI LtkRegisterCallback(HLTK obj, UINT event_id, LtkCallback cb, void* userdata)
{
	Object* pobj = ltk_cast<Object>(obj);
	pobj->RegisterCallback(event_id, cb, userdata);
}

LTK_API void WINAPI LtkUnregisterCallback(HLTK obj, UINT event_id, LtkCallback cb, void* userdata)
{
	Object* pobj = ltk_cast<Object>(obj);
	// TODO
}

LTK_API HLTK WINAPI LtkGetEventSender()
{
	return (HLTK)Object::GetDelegateInvoker();
}

LTK_API HLTK LtkBuildFromXml(LPCSTR path)
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

LTK_API void WINAPI LtkWindow_Create(HLTK self, HWND parent, LtkRect* rc)
{
	Gdiplus::RectF rcf(rc->x, rc->y, rc->width, rc->height);
	Window *thiz = ltk_cast<Window>(self);
	thiz->Create(0, rcf);
}

LTK_API void WINAPI LtkWindow_CreateCenter(HLTK self, HWND parent, float width, float height)
{
	Gdiplus::SizeF size(width, height);
	Window *thiz = ltk_cast<Window>(self);
	thiz->Create(0, size);
}

LTK_API void WINAPI LtkWindow_SetCaption(HLTK self, LPCWSTR text)
{
	Window* thiz = ltk_cast<Window>(self);
	thiz->SetCaption(text);
}

LTK_API void WINAPI LtkWindow_SetBackground(HLTK self, LPCSTR name)
{
	Window* thiz = ltk_cast<Window>(self);
	thiz->SetBackground(name);
}

LTK_API void WINAPI LtkWindow_UpdateTheme(HLTK self)
{
	Window* thiz = ltk_cast<Window>(self);
	thiz->UpdateTheme();
}

LTK_API void WINAPI LtkWindow_SetCentralWidget(HLTK self, HLTK widget)
{
	Window* thiz = ltk_cast<Window>(self);
	Widget* w = ltk_cast<Widget>(widget);
	thiz->SetCentralWidget(w);
}

LTK_API void WINAPI LtkWindow_SetMenu(HLTK self, HLTK menu_bar)
{
	Window* thiz = ltk_cast<Window>(self);
	MenuBar* mb = ltk_cast<MenuBar>(menu_bar);
	thiz->SetMenu(mb);
}

LTK_API HWND WINAPI LtkWindow_GetHWND(HLTK self)
{
	Window* thiz = ltk_cast<Window>(self);
	return thiz->GetHWND();
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
	BoxLayout* thiz = ltk_cast<BoxLayout>(self);
	thiz->AddLayoutItem(ltk_cast<Widget>(widget), preferedSize, growFactor);
}

LTK_API void WINAPI LtkBoxLayout_AddSpaceItem(HLTK self, float preferedSize, float growFactor)
{
	BoxLayout* thiz = ltk_cast<BoxLayout>(self);
	thiz->AddSpaceItem(preferedSize, growFactor);
}

LTK_API void WINAPI LtkBoxLayout_SetSpacing(HLTK self, float spacing)
{
	BoxLayout* thiz = ltk_cast<BoxLayout>(self);
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

LTK_API void WINAPI LtkButton_SetText(HLTK self, LPCWSTR text)
{
	Button* thiz = ltk_cast<Button>(self);
	thiz->SetText(text);
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

LTK_API void WINAPI LtkHeaderCtrl_AddColumn(HLTK self, LPCWSTR text, float width)
{
	HeaderCtrl* thiz = ltk_cast<HeaderCtrl>(self);
	thiz->AddColumn(text, width);
}

//////////////////////////////////////////////////////////////////////////
// ListView
//////////////////////////////////////////////////////////////////////////

LTK_API HLTK WINAPI LtkListView_New_(LPCSTR source, int line)
{
	return (HLTK)new ListView;
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
	obj->SetSourceLine(source, line);
	return (HLTK)obj;
}

LTK_API void WINAPI LtkTreeNode_AddChild(HLTK self, HLTK node)
{
	TreeNode* thiz = (TreeNode*)self;
	thiz->AddChild((TreeNode*)node);
}

LTK_API void WINAPI LtkTreeNode_SetText(HLTK self, LPCWSTR text)
{
	TreeNode* thiz = (TreeNode*)self;
	thiz->SetText(text);
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

LTK_API void WINAPI LtkMenuBar_AddItem(HLTK self, LPCWSTR text)
{
	MenuBar* thiz = ltk_cast<MenuBar>(self);
	thiz->AddItem(text);
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

LTK_API void WINAPI LtkPopupMenu_AddItem(HLTK self, LPCWSTR text, LPCSTR name)
{
	PopupMenu* thiz = ltk_cast<PopupMenu>(self);
	thiz->AddItem(text, name);
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