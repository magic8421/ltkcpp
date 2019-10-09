#include "stdafx.h"
#include "LtkInterface.h"
#include "ltk.h"
#include "Object.h"
#include "Window.h"
#include "Button.h"
#include "BoxLayout.h"

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

LTK_API void WINAPI LtkRunApp()
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


LTK_API LtkObject * WINAPI LtkGetEventSender()
{
	return (LtkObject *)Object::GetEventSender();
}

//////////////////////////////////////////////////////////////////////////
// Window
//////////////////////////////////////////////////////////////////////////


LTK_API LtkWindow* WINAPI LtkWindow_New()
{
	return (LtkWindow*)new Window;
}

LTK_API void WINAPI LtkCreateWindow(
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


LTK_API void WINAPI LtkCreateWindowCenter(
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

//////////////////////////////////////////////////////////////////////////
// BoxLayout
//////////////////////////////////////////////////////////////////////////

LTK_API LtkBoxLayout* WINAPI LtkBoxLayout_New(UINT orientation)
{
	return (LtkBoxLayout*)new BoxLayout((ltk::Orientation)orientation);
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

LTK_API LtkButton* WINAPI LtkButton_New()
{
	return (LtkButton*)new Button;
}

LTK_API void WINAPI LtkButton_SetText(LtkButton* self, LPCWSTR text)
{
	Button* thiz = (Button*)self;
	thiz->SetText(text);
}
