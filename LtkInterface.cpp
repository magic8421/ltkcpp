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

using namespace ltk;

static BOOL sApiCheck = TRUE;

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
	Object *pobj = (Object *)obj;
	delete pobj;
}

LTK_API LPCSTR WINAPI LtkInternString(LPCSTR str)
{
	return StyleManager::Instance()->InternString(str);
}

LTK_API void WINAPI LtkSetName(HLTK o, LPCSTR name)
{
	((Object*)(o))->SetName(name);
}

LTK_API LPCSTR WINAPI LtkGetName(HLTK o)
{
	return ((Object*)(o))->GetName();
}

LTK_API void WINAPI LtkRegisterCallback(HLTK obj, UINT event_id, LtkCallback cb, void* userdata)
{
	Object* pobj = (Object*)obj;
	pobj->RegisterCallback(event_id, cb, userdata);
}

LTK_API void WINAPI LtkUnregisterCallback(HLTK obj, UINT event_id, LtkCallback cb, void* userdata)
{
	Object* pobj = (Object*)obj;
	// TODO
}

LTK_API HLTK WINAPI LtkGetEventSender()
{
	return (HLTK)Object::GetDelegateInvoker();
}



//////////////////////////////////////////////////////////////////////////
// Widget
//////////////////////////////////////////////////////////////////////////

LTK_API BOOL WINAPI LtkIsSprite(HLTK o)
{
	Object *obj = (Object *)o;
	if (!Object::CheckValid(obj)) return FALSE;
	return obj->Is(Widget::TypeIdClass()) ? TRUE : FALSE;
}


//////////////////////////////////////////////////////////////////////////
// Window
//////////////////////////////////////////////////////////////////////////


LTK_API HLTK WINAPI LtkWindow_New_(LPCSTR source, int line)
{
	auto obj = new Window;
	obj->SetSourceLine(source, line);
	return (HLTK)obj;
}

template<typename T>
T* LtkCheckType(HLTK o)
{
	if (sApiCheck) {
		Object* obj = (Object*)o;
		if (!Object::CheckValid(obj)) __debugbreak();
		if (!obj->Is(T::TypeIdClass())) __debugbreak();
	}
	return (T *) o;
}

LTK_API void WINAPI LtkWindow_Create(HLTK self, HWND parent, LtkRect* rc)
{
	Gdiplus::RectF rcf(rc->x, rc->y, rc->width, rc->height);
	Window *thiz = LtkCheckType<Window>(self);
	thiz->Create(0, rcf);
}

LTK_API void WINAPI LtkWindow_CreateCenter(HLTK self, HWND parent, float width, float height)
{
	Gdiplus::SizeF size(width, height);
	Window *thiz = LtkCheckType<Window>(self);
	thiz->Create(0, size);
}

LTK_API void WINAPI LtkWindow_SetCaption(HLTK self, LPCWSTR text)
{
	Window* thiz = LtkCheckType<Window>(self);
	thiz->SetCaption(text);
}

LTK_API void WINAPI LtkWindow_SetBackground(HLTK self, LPCSTR name)
{
	Window* thiz = LtkCheckType<Window>(self);
	thiz->SetBackground(name);
}

LTK_API void WINAPI LtkWindow_UpdateTheme(HLTK self)
{
	Window* thiz = LtkCheckType<Window>(self);
	thiz->UpdateTheme();
}

LTK_API void WINAPI LtkWindow_SetCentralWidget(HLTK self, HLTK widget)
{
	Window* thiz = LtkCheckType<Window>(self);
	Widget* w = LtkCheckType<Widget>(widget);
	thiz->SetCentralWidget(w);
}

LTK_API void WINAPI LtkWindow_SetMenu(HLTK self, HLTK menu_bar)
{
	Window* thiz = LtkCheckType<Window>(self);
	MenuBar* mb = LtkCheckType<MenuBar>(menu_bar);
	thiz->SetMenu(mb);
}

LTK_API HWND WINAPI LtkWindow_GetHWND(HLTK self)
{
	Window* thiz = LtkCheckType<Window>(self);
	return thiz->GetHWND();
}

typedef void(CALLBACK *WindowCloseCallback)(void *userdata, BOOL *pProceed, BOOL *bHandled);
typedef void(CALLBACK *WindowDestroyCallback)(void *userdata, BOOL *bHandled);

void Window::DoInvokeCallback(UINT event_id, LtkCallback cb, void* userdata, 
	va_list args, BOOL *bHandled)
{
	switch (event_id)
	{
	case LTK_WINDOW_CLOSE:
	{
		BOOL *pProceed = va_arg(args, BOOL *);
		return ((WindowCloseCallback)cb)(userdata, pProceed, bHandled);
	}
	case LTK_WINDOW_DESTROY:
	{
		return ((WindowDestroyCallback)cb)(userdata, bHandled);
	}
	}
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

LTK_API BOOL WINAPI LtkIsBoxLayout(HLTK o)
{
	auto obj = (Object *)o;
	if (!Object::CheckValid(obj)) return FALSE;
	return obj->Is(BoxLayout::TypeIdClass()) ? TRUE : FALSE;
}

LTK_API void WINAPI LtkBoxLayout_AddLayoutItem(
	LtkBoxLayout* self, LtkSprite *sp, float preferedSize, float growFactor)
{
	BoxLayout* thiz = (BoxLayout *)self;
	thiz->AddLayoutItem((Widget*)sp, preferedSize, growFactor);
}

LTK_API void WINAPI LtkBoxLayout_AddSpaceItem(LtkBoxLayout* self, float preferedSize, float growFactor)
{
	BoxLayout* thiz = (BoxLayout *)self;
	thiz->AddSpaceItem(preferedSize, growFactor);
}

LTK_API void WINAPI LtkBoxLayout_SetSpacing(LtkBoxLayout* self, float spacing)
{
	BoxLayout* thiz = (BoxLayout *)self;
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

LTK_API BOOL WINAPI LtkIsButton(HLTK o)
{
	Object *obj = (Object *)o;
	if (!Object::CheckValid(obj)) return FALSE;
	return obj->Is(Button::TypeIdClass()) ? TRUE : FALSE;
}

LTK_API void WINAPI LtkButton_SetText(LtkButton* self, LPCWSTR text)
{
	Button* thiz = (Button*)self;
	thiz->SetText(text);
}

//////////////////////////////////////////////////////////////////////////
// HeaderCtrl
//////////////////////////////////////////////////////////////////////////

LTK_API BOOL WINAPI LtkIsHeaderCtrl(HLTK o)
{
	Object *obj = (Object *)o;
	if (!Object::CheckValid(obj)) return FALSE;
	return obj->Is(HeaderCtrl::TypeIdClass()) ? TRUE : FALSE;
}

LTK_API HLTK WINAPI LtkHeaderCtrl_New_(LPCSTR source, int line)
{
	auto obj = new HeaderCtrl();
	obj->SetSourceLine(source, line);
	return (HLTK)obj;
}

LTK_API void WINAPI LtkHeaderCtrl_AddColumn(LtkHeaderCtrl* self, LPCWSTR text, float width)
{
	HeaderCtrl* thiz = (HeaderCtrl*)self;
	thiz->AddColumn(text, width);
}

//////////////////////////////////////////////////////////////////////////
// ListView
//////////////////////////////////////////////////////////////////////////

LTK_API HLTK WINAPI LtkListView_New_(LPCSTR source, int line)
{
	return (HLTK)new ListView;
}

LTK_API BOOL WINAPI LtkIsListView(HLTK o)
{
	Object *obj = (Object *)o;
	if (!Object::CheckValid(obj)) return FALSE;
	return obj->Is(ListView::TypeIdClass()) ? TRUE : FALSE;
}

LTK_API UINT WINAPI LtkListView_AddRow(LtkListView* self)
{
	ListView* thiz = (ListView *)self;
	return thiz->AddRow();
}

LTK_API void WINAPI LtkListView_SetCellText(LtkListView* self, UINT row, UINT col, LPCWSTR text)
{
	ListView* thiz = (ListView *)self;
	thiz->SetCellText(row, col, text);
}

LTK_API LPCWSTR WINAPI LtkListView_GetCellText(LtkListView* self, UINT row, UINT col)
{
	ListView* thiz = (ListView *)self;
	return thiz->GetCellText(row, col);
}

LTK_API int WINAPI LtkListView_GetSelectedRow(LtkListView* self)
{
	ListView* thiz = (ListView *)self;
	return thiz->GetSelectedRow();
}

LTK_API HLTK WINAPI LtkListView_GetHeaderCtrl(LtkListView* self)
{
	ListView* thiz = (ListView *)self;
	return (HLTK)thiz->GetHeaderCtrl();
}

LTK_API void WINAPI LtkListView_UpdateColumnWidth(LtkListView* self)
{
	ListView* thiz = (ListView *)self;
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

LTK_API BOOL WINAPI LtkIsSplitter(HLTK o)
{
	Object *obj = (Object *)o;
	if (!Object::CheckValid(obj)) return FALSE;
	return obj->Is(Splitter::TypeIdClass()) ? TRUE : FALSE;
}

LTK_API void WINAPI LtkSplitter_Resize(LtkSplitter* self, UINT n)
{
	Splitter* thiz = (Splitter*)self;
	thiz->Resize(n);
}

LTK_API LtkSprite* WINAPI LtkSplitter_SetClientAt(LtkSplitter* self, UINT idx, LtkSprite* sp)
{
	Splitter* thiz = (Splitter*)self;
	return (LtkSprite*)thiz->SetClientAt(idx, (Widget*)sp);
}

LTK_API void WINAPI LtkSplitter_SetClientSize(LtkSplitter* self, UINT idx, float size)
{
	Splitter* thiz = (Splitter*)self;
	thiz->SetClientSize(idx, size);
}

//////////////////////////////////////////////////////////////////////////
// TreeView
//////////////////////////////////////////////////////////////////////////

LTK_API BOOL WINAPI LtkIsTreeView(HLTK o)
{
	Object *obj = (Object *)o;
	if (!Object::CheckValid(obj)) return FALSE;
	return obj->Is(TreeView::TypeIdClass()) ? TRUE : FALSE;
}

LTK_API HLTK WINAPI LtkTreeView_New_(LPCSTR source, int line)
{
	auto obj = new TreeView();
	obj->SetSourceLine(source, line);
	return (HLTK)obj;
}

LTK_API HLTK WINAPI LtkTreeView_GetRootNode(LtkTreeView* self)
{
	TreeView* thiz = (TreeView*)self;
	return (HLTK)thiz->GetRootNode();
}

//////////////////////////////////////////////////////////////////////////
// TreeNode
//////////////////////////////////////////////////////////////////////////

LTK_API BOOL WINAPI LtkIsTreeNode(HLTK o)
{
	Object *obj = (Object *)o;
	if (!Object::CheckValid(obj)) return FALSE;
	return obj->Is(TreeNode::TypeIdClass()) ? TRUE : FALSE;
}

LTK_API HLTK WINAPI LtkTreeNode_New_(LPCSTR source, int line)
{
	auto obj = new TreeNode();
	obj->SetSourceLine(source, line);
	return (HLTK)obj;
}

LTK_API void WINAPI LtkTreeNode_AddChild(LtkTreeNode* self, LtkTreeNode* node)
{
	TreeNode* thiz = (TreeNode*)self;
	thiz->AddChild((TreeNode*)node);
}

LTK_API void WINAPI LtkTreeNode_SetText(LtkTreeNode* self, LPCWSTR text)
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

LTK_API BOOL WINAPI LtkIsMenuBar(HLTK o)
{
	Object *obj = (Object *)o;
	if (!Object::CheckValid(obj)) return FALSE;
	return obj->Is(MenuBar::TypeIdClass()) ? TRUE : FALSE;
}

LTK_API HLTK WINAPI LtkMenuBar_New_(LPCSTR source, int line)
{
	auto obj = new MenuBar();
	obj->SetSourceLine(source, line);
	return (HLTK)obj;
}

LTK_API void WINAPI LtkMenuBar_AddItem(LtkMenuBar* self, LPCWSTR text)
{
	MenuBar* thiz = (MenuBar*)self;
	thiz->AddItem(text);
}

LTK_API void WINAPI LtkMenuBar_SetPopupMenu(LtkMenuBar* self, UINT idx, LtkPopupMenu* popup)
{
	MenuBar* thiz = (MenuBar*)self;
	thiz->SetPopupMenu(idx, (PopupMenu*)popup);
}

//////////////////////////////////////////////////////////////////////////
// PopupMenu
//////////////////////////////////////////////////////////////////////////

LTK_API BOOL WINAPI LtkIsPopupMenu(HLTK o)
{
	Object *obj = (Object *)o;
	if (!Object::CheckValid(obj)) return FALSE;
	return obj->Is(PopupMenu::TypeIdClass()) ? TRUE : FALSE;
}

LTK_API HLTK WINAPI LtkPopupMenu_New_(LPCSTR source, int line)
{
	auto obj = new PopupMenu();
	obj->SetSourceLine(source, line);
	return (HLTK)obj;
}

LTK_API void WINAPI LtkPopupMenu_AddItem(LtkPopupMenu* self, LPCWSTR text, LPCSTR name)
{
	PopupMenu* thiz = (PopupMenu*)self;
	thiz->AddItem(text, name);
}

LTK_API void WINAPI LtkPopupMenu_AddSeparator(LtkPopupMenu* self)
{
	PopupMenu* thiz = (PopupMenu*)self;
	thiz->AddSeparator();
}

LTK_API void WINAPI LtkPopupMenu_SetWidth(LtkPopupMenu* self, float width)
{
	PopupMenu* thiz = (PopupMenu*)self;
	thiz->SetWidth(width);
}

LTK_API void WINAPI LtkPopupMenu_SetSubMenu(LtkPopupMenu* self, UINT idx, LtkPopupMenu* popup)
{
	PopupMenu* thiz = (PopupMenu*)self;
	thiz->SetSubMenu(idx, (PopupMenu*)popup);
}