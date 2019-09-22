// LtkCDemo.cpp : 定义应用程序的入口点。
//

#include "pch.h"
#include "framework.h"
#include "LtkCDemo.h"
#include "LtkInterface.h"

BOOL CALLBACK OnWindowClose(void* userdata, BOOL *pProceed)
{
	*pProceed = TRUE;
	return TRUE;
}

BOOL CALLBACK OnWindowDestroy(void* userdata)
{
	PostQuitMessage(0);
	return TRUE;
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
	LtkObject_RegisterCallback((LtkObject*)wnd, LTK_WINDOW_DESTROY,
		(LtkCallback)OnWindowDestroy, NULL);
	LtkObject_RegisterCallback((LtkObject*)wnd, LTK_WINDOW_CLOSE, 
		(LtkCallback)OnWindowClose, NULL);

	LtkRunApp();
	LtkUninitialize();
	return 0;
}


