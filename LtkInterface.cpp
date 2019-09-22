#include "stdafx.h"
#include "LtkInterface.h"
#include "ltk.h"
#include "Object.h"
#include "Window.h"

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

LTK_API void WINAPI LtkObject_RegisterCallback(LtkObject* self, UINT event_id, LtkCallback cb, void* userdata)
{
	ltk::Object* thiz = (ltk::Object*)self;
	thiz->RegisterCallback(event_id, cb, userdata);
}

LTK_API LtkWindow* WINAPI LtkCreateWindow(LtkWindow *parent, LtkRect *rc)
{
	ltk::Window* p2 = nullptr;
	if (parent) {
		p2 = (ltk::Window*)parent;
	}
	Gdiplus::RectF rcf(rc->x, rc->y, rc->w, rc->h);
	auto wnd = new ltk::Window;
	wnd->Create(p2, rcf);
	return (LtkWindow*)wnd;
}

LTK_API void WINAPI LtkWindow_SetBackground(LtkWindow* self, LPCSTR name)
{
	ltk::Window* thiz = (ltk::Window*)self;
	thiz->SetBackground(name);
}

LTK_API void WINAPI LtkWindow_UpdateTheme(LtkWindow* self)
{
	ltk::Window* thiz = (ltk::Window*)self;
	thiz->UpdateTheme();
}

