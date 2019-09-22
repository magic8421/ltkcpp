#include "stdafx.h"
#include "LtkInterface.h"
#include "ltk.h"
#include "Object.h"
#include "Window.h"

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

LTK_API void WINAPI LtkFree(HLTK h)
{
	Object *pobj = (Object *)h;
	delete pobj;
}

LTK_API void WINAPI LtkObject_RegisterCallback(
	HLTK obj, UINT event_id, LtkCallback cb, void* userdata)
{
	Object* thiz = (Object*)obj;
	thiz->RegisterCallback(event_id, cb, userdata);
}


LTK_API HLTK WINAPI LtkGetEventSender()
{
	return (HLTK)Object::GetEventSender();
}

LTK_API HLTK WINAPI LtkCreateWindow(HLTK parent, LtkRect *rc)
{
	Window* p2 = nullptr;
	if (parent) {
		p2 = (Window*)parent;
	}
	Gdiplus::RectF rcf(rc->x, rc->y, rc->w, rc->h);
	auto wnd = new Window;
	wnd->Create(p2, rcf);
	return (HLTK)wnd;
}


LTK_API HLTK WINAPI LtkCreateWindowCenter(HLTK parent, float width, float height)
{
	Gdiplus::SizeF size(width, height);
	auto wnd = new Window;
	wnd->Create(nullptr, size);
	return (HLTK)wnd;
}

LTK_API void WINAPI LtkWindow_SetBackground(HLTK wnd, LPCSTR name)
{
	Window* thiz = ((RTTI*)wnd)->As<Window>();
	thiz->SetBackground(name);
}

LTK_API void WINAPI LtkWindow_UpdateTheme(HLTK wnd)
{
	Window* thiz = ((RTTI*)wnd)->As<Window>();
	thiz->UpdateTheme();
}

