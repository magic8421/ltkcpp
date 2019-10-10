// LtkCDemo.cpp : 定义应用程序的入口点。
//

#include "pch.h"
#include "framework.h"
#include "LtkCDemo.h"
#include "LtkInterface.h"


void CALLBACK MyEventCallback(void* userdata)
{
	::PostQuitMessage(0);
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	LtkInitialize();

	LtkWindow* wnd = LtkWindow_New();
	LtkWindow_CreateCenter(wnd, NULL, 500, 400);
	//LtkWindow_SetBackground(wnd, "window_bg");
	LtkObject* splitter_h = LtkSplitter_New(LTK_HORIZONTAL);
	LtkSplitter_Resize(LTK_SPLITTER(splitter_h), 3);

	LtkObject* btn = LtkButton_New();
	LtkSplitter_SetClientAt(LTK_SPLITTER(splitter_h), 0, LTK_SPRITE(btn));
	btn = LtkButton_New();
	LtkSplitter_SetClientAt(LTK_SPLITTER(splitter_h), 1, LTK_SPRITE(btn));
	btn = LtkButton_New();
	LtkSplitter_SetClientAt(LTK_SPLITTER(splitter_h), 2, LTK_SPRITE(btn));

	LtkWindow_SetClientSprite(wnd, LTK_SPRITE(splitter_h));

	LtkWindow_UpdateTheme(wnd);
	LtkObject_RegisterCallback(
		(LtkObject*)wnd, LTK_WINDOW_DESTROY, (LtkCallback)MyEventCallback, NULL);

	LtkRunMessageLoop();

	LtkFree((LtkObject *)wnd);
	LtkUninitialize();
	return 0;
}


