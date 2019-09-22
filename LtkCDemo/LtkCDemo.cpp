// LtkCDemo.cpp : 定义应用程序的入口点。
//

#include "pch.h"
#include "framework.h"
#include "LtkCDemo.h"
#include "LtkInterface.h"

BOOL CALLBACK OnWindowClose(void* userdata, BOOL *pProceed)
{
	*pProceed = TRUE;
	HLTK obj = LtkGetEventSender();
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

	HLTK wnd = LtkCreateWindowCenter(NULL, 500, 400);
	LtkWindow_SetBackground(wnd, "window_bg");
	LtkWindow_UpdateTheme(wnd);
	LtkObject_RegisterCallback(wnd, LTK_WINDOW_DESTROY,
		(LtkCallback)OnWindowDestroy, NULL);
	LtkObject_RegisterCallback(wnd, LTK_WINDOW_CLOSE,
		(LtkCallback)OnWindowClose, NULL);

	LtkRunApp();
	LtkFree(wnd);
	LtkUninitialize();
	return 0;
}


