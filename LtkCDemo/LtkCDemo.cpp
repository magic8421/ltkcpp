// LtkCDemo.cpp : 定义应用程序的入口点。
//

#include "pch.h"
#include "framework.h"
#include "LtkCDemo.h"
#include "LtkInterface.h"


void CALLBACK MyEventCallback(void* userdata, UINT event_id, void* sender, void* arg)
{
	switch (event_id) {
	case LTK_WINDOW_DESTROY:
		::PostQuitMessage(0);
		break;
	default:
		break;
	}
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	LtkInitialize();

	LtkRect rc = { 10.f, 10.f, 400.f, 400.f };
	LtkWindow* wnd = LtkCreateWindow(NULL, &rc);
	LtkWindow_SetBackground(wnd, "window_bg");
	LtkWindow_UpdateTheme(wnd);
	LtkObject_RegisterCallback((LtkObject*)wnd, LTK_WINDOW_DESTROY, MyEventCallback, NULL);

	LtkRunApp();
	LtkUninitialize();
	return 0;
}


