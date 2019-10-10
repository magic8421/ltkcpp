#include "stdafx.h"
#include "LtkInterface.h"
#include "ltk.h"
#include "Object.h"
#include "Window.h"
#include "Button.h"
#include "BoxLayout.h"
#include "ListView.h"
#include "Splitter.h"

using namespace ltk;

LTK_API UINT WINAPI LtkInitialize()
{
	ltk::LtkInitialize();
	return LTK_OK;
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

LTK_API void WINAPI LtkObject_RegisterCallback(
	LtkObject * obj, UINT event_id, LtkCallback cb, void* userdata)
{
	Object* thiz = (Object*)obj;
	thiz->RegisterCallback(event_id, cb, userdata);
}

LTK_API LtkObject * WINAPI LtkCallbackInvoker()
{
	return (LtkObject *)Object::GetEventSender();
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


LTK_API LtkWindow* WINAPI LtkWindow_New_(LPCSTR source, int line)
{
	auto obj = new Window;
	obj->SetSourceLine(source, line);
	return (LtkWindow*)obj;
}

LTK_API void WINAPI LtkWindow_Create(
	LtkWindow* self, LtkWindow* parent, LtkRect *rc)
{
	Window* p2 = nullptr;
	if (parent) {
		p2 = (Window*)parent;
	}
	Gdiplus::RectF rcf(rc->x, rc->y, rc->w, rc->h);
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

LTK_API void WINAPI LtkWindow_SetBackground(LtkWindow* wnd, LPCSTR name)
{
	Window* thiz = (Window*)wnd;
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


//////////////////////////////////////////////////////////////////////////
// BoxLayout
//////////////////////////////////////////////////////////////////////////

LTK_API LtkBoxLayout* WINAPI LtkBoxLayout_New_(UINT orientation, LPCSTR source, int line)
{
	auto obj = new BoxLayout((ltk::Orientation)orientation);
	obj->SetSourceLine(source, line);
	return (LtkBoxLayout*)obj;
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