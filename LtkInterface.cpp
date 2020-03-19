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

LTK_API void WINAPI LtkFree(LtkObject *obj)
{
	Object *pobj = (Object *)obj;
	delete pobj;
}

LTK_API LPCSTR WINAPI LtkInternString(LPCSTR str)
{
	return StyleManager::Instance()->InternString(str);
}

LTK_API void WINAPI LtkObject_SetName(LtkObject* o, LPCSTR name)
{
	((Object*)(o))->SetName(name);
}

LTK_API LPCSTR WINAPI LtkObject_GetName(LtkObject* o)
{
	return ((Object*)(o))->GetName();
}

LTK_API void WINAPI LtkObject_AddNotifyCallback(LtkObject* o, void* userdata, LtkCallback callback)
{
	Object* pobj = (Object*)o;
	pobj->AddNotifyCallback(userdata, callback);
}

LTK_API void WINAPI LtkObject_RemoveNotifyCallback(LtkObject* o, void* userdata, LtkCallback callback)
{
	Object* pobj = (Object*)o;
	pobj->RemoveNotifyCallback(userdata, callback);
}



//////////////////////////////////////////////////////////////////////////
// Sprite
//////////////////////////////////////////////////////////////////////////

LTK_API BOOL WINAPI LtkIsSprite(LtkObject* o)
{
	Object *obj = (Object *)o;
	if (!Object::CheckValid(obj)) return FALSE;
	return obj->Is(Sprite::TypeIdClass()) ? TRUE : FALSE;
}


//////////////////////////////////////////////////////////////////////////
// Window
//////////////////////////////////////////////////////////////////////////


LTK_API LtkObject* WINAPI LtkWindow_New_(LPCSTR source, int line)
{
	auto obj = new Window;
	obj->SetSourceLine(source, line);
	return (LtkObject*)obj;
}

LTK_API BOOL WINAPI LtkIsWindow(LtkObject* o)
{
	Object *obj = (Object *)o;
	if (!Object::CheckValid(obj)) return FALSE;
	return obj->Is(Window::TypeIdClass()) ? TRUE : FALSE;
}

LTK_API void WINAPI LtkWindow_Create(
	LtkWindow* self, LtkWindow* parent, LtkRect *rc)
{
	Window* p2 = nullptr;
	if (parent) {
		p2 = (Window*)parent;
	}
	Gdiplus::RectF rcf(rc->x, rc->y, rc->width, rc->height);
	Window *thiz = (Window *)self;
	thiz->Create(p2, rcf);
}


LTK_API void WINAPI LtkWindow_CreateCenter(
	LtkWindow*self, LtkWindow* parent, float width, float height)
{
	Window* p2 = nullptr;
	if (parent) {
		p2 = (Window*)parent;
	}
	Gdiplus::SizeF size(width, height);
	Window *thiz = (Window *)self;
	thiz->Create(p2, size);
}

LTK_API void WINAPI LtkWindow_SetCaption(LtkWindow*self, LPCWSTR text)
{
	Window* thiz = (Window*)self;
	thiz->SetCaption(text);
}

LTK_API void WINAPI LtkWindow_SetBackground(LtkWindow* self, LPCSTR name)
{
	Window* thiz = (Window*)self;
	thiz->SetBackground(name);
}

LTK_API void WINAPI LtkWindow_UpdateTheme(LtkWindow* wnd)
{
	Window* thiz = (Window*)wnd;
	thiz->UpdateTheme();
}

LTK_API void WINAPI LtkWindow_SetClientSprite(LtkWindow* self, LtkSprite* sp)
{
	Window* thiz = (Window*)self;
	thiz->SetClientSprite((Sprite*)sp);
}

LTK_API void WINAPI LtkWindow_SetMenu(LtkWindow* self, LtkMenuBar* menu)
{
	Window* thiz = (Window*)self;
	thiz->SetMenu((MenuBar*)menu);
}

LTK_API HWND WINAPI LtkWindow_GetHWND(LtkWindow* self)
{
	Window* thiz = (Window*)self;
	return thiz->Handle();
}


//////////////////////////////////////////////////////////////////////////
// BoxLayout
//////////////////////////////////////////////////////////////////////////

LTK_API LtkObject* WINAPI LtkBoxLayout_New_(UINT orientation, LPCSTR source, int line)
{
	auto obj = new BoxLayout((ltk::Orientation)orientation);
	obj->SetSourceLine(source, line);
	return (LtkObject*)obj;
}

LTK_API BOOL WINAPI LtkIsBoxLayout(LtkObject* o)
{
	Object *obj = (Object *)o;
	if (!Object::CheckValid(obj)) return FALSE;
	return obj->Is(BoxLayout::TypeIdClass()) ? TRUE : FALSE;
}

LTK_API void WINAPI LtkBoxLayout_AddLayoutItem(
	LtkBoxLayout* self, LtkSprite *sp, float preferedSize, float growFactor)
{
	BoxLayout* thiz = (BoxLayout *)self;
	thiz->AddLayoutItem((Sprite*)sp, preferedSize, growFactor);
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

LTK_API LtkObject* WINAPI LtkButton_New_(LPCSTR source, int line)
{
	auto obj = new Button;
	obj->SetSourceLine(source, line);
	return (LtkObject*)obj;
}

LTK_API BOOL WINAPI LtkIsButton(LtkObject* o)
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

LTK_API BOOL WINAPI LtkIsHeaderCtrl(LtkObject* o)
{
	Object *obj = (Object *)o;
	if (!Object::CheckValid(obj)) return FALSE;
	return obj->Is(HeaderCtrl::TypeIdClass()) ? TRUE : FALSE;
}

LTK_API LtkObject* WINAPI LtkHeaderCtrl_New_(LPCSTR source, int line)
{
	auto obj = new HeaderCtrl();
	obj->SetSourceLine(source, line);
	return (LtkObject*)obj;
}

LTK_API void WINAPI LtkHeaderCtrl_AddColumn(LtkHeaderCtrl* self, LPCWSTR text, float width)
{
	HeaderCtrl* thiz = (HeaderCtrl*)self;
	thiz->AddColumn(text, width);
}

//////////////////////////////////////////////////////////////////////////
// ListView
//////////////////////////////////////////////////////////////////////////

LTK_API LtkObject* WINAPI LtkListView_New_(LPCSTR source, int line)
{
	return (LtkObject*)new ListView;
}

LTK_API BOOL WINAPI LtkIsListView(LtkObject* o)
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

LTK_API LtkObject* WINAPI LtkListView_GetHeaderCtrl(LtkListView* self)
{
	ListView* thiz = (ListView *)self;
	return (LtkObject*)thiz->GetHeaderCtrl();
}

LTK_API void WINAPI LtkListView_UpdateColumnWidth(LtkListView* self)
{
	ListView* thiz = (ListView *)self;
	thiz->UpdateColumnWidth();
}

//////////////////////////////////////////////////////////////////////////
// Splitter
//////////////////////////////////////////////////////////////////////////

LTK_API LtkObject* WINAPI LtkSplitter_New_(UINT orientation, LPCSTR source, int line)
{
	auto obj = new Splitter((Orientation)orientation);
	obj->SetSourceLine(source, line);
	return (LtkObject*)obj;
}

LTK_API BOOL WINAPI LtkIsSplitter(LtkObject *o)
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
	return (LtkSprite*)thiz->SetClientAt(idx, (Sprite*)sp);
}

LTK_API void WINAPI LtkSplitter_SetClientSize(LtkSplitter* self, UINT idx, float size)
{
	Splitter* thiz = (Splitter*)self;
	thiz->SetClientSize(idx, size);
}

//////////////////////////////////////////////////////////////////////////
// TreeView
//////////////////////////////////////////////////////////////////////////

LTK_API BOOL WINAPI LtkIsTreeView(LtkObject* o)
{
	Object *obj = (Object *)o;
	if (!Object::CheckValid(obj)) return FALSE;
	return obj->Is(TreeView::TypeIdClass()) ? TRUE : FALSE;
}

LTK_API LtkObject* WINAPI LtkTreeView_New_(LPCSTR source, int line)
{
	auto obj = new TreeView();
	obj->SetSourceLine(source, line);
	return (LtkObject*)obj;
}

LTK_API LtkObject* WINAPI LtkTreeView_GetRootNode(LtkTreeView* self)
{
	TreeView* thiz = (TreeView*)self;
	return (LtkObject*)thiz->GetRootNode();
}

//////////////////////////////////////////////////////////////////////////
// TreeNode
//////////////////////////////////////////////////////////////////////////

LTK_API BOOL WINAPI LtkIsTreeNode(LtkObject* o)
{
	Object *obj = (Object *)o;
	if (!Object::CheckValid(obj)) return FALSE;
	return obj->Is(TreeNode::TypeIdClass()) ? TRUE : FALSE;
}

LTK_API LtkObject* WINAPI LtkTreeNode_New_(LPCSTR source, int line)
{
	auto obj = new TreeNode();
	obj->SetSourceLine(source, line);
	return (LtkObject*)obj;
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

LTK_API BOOL WINAPI LtkIsTextEdit(LtkObject* o)
{
	Object *obj = (Object *)o;
	if (!Object::CheckValid(obj)) return FALSE;
	return obj->Is(TextEdit::TypeIdClass()) ? TRUE : FALSE;
}

LTK_API LtkObject* WINAPI LtkTextEdit_New_(LPCSTR source, int line)
{
	auto obj = new TextEdit();
	obj->SetSourceLine(source, line);
	return (LtkObject*)obj;
}

//////////////////////////////////////////////////////////////////////////
// MenuBar
//////////////////////////////////////////////////////////////////////////

LTK_API BOOL WINAPI LtkIsMenuBar(LtkObject* o)
{
	Object *obj = (Object *)o;
	if (!Object::CheckValid(obj)) return FALSE;
	return obj->Is(MenuBar::TypeIdClass()) ? TRUE : FALSE;
}

LTK_API LtkObject* WINAPI LtkMenuBar_New_(LPCSTR source, int line)
{
	auto obj = new MenuBar();
	obj->SetSourceLine(source, line);
	return (LtkObject*)obj;
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

LTK_API BOOL WINAPI LtkIsPopupMenu(LtkObject* o)
{
	Object *obj = (Object *)o;
	if (!Object::CheckValid(obj)) return FALSE;
	return obj->Is(PopupMenu::TypeIdClass()) ? TRUE : FALSE;
}

LTK_API LtkObject* WINAPI LtkPopupMenu_New_(LPCSTR source, int line)
{
	auto obj = new PopupMenu();
	obj->SetSourceLine(source, line);
	return (LtkObject*)obj;
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