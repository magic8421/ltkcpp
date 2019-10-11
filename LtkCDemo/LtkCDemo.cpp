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

	LtkObject* wnd = LtkWindow_New();
	//LtkWindow_SetBackground(wnd, "window_bg");
	LtkObject_RegisterCallback(
		(LtkObject*)wnd, LTK_WINDOW_DESTROY, (LtkCallback)MyEventCallback, NULL);

	LtkObject* splitter_h = LtkSplitter_New(LTK_HORIZONTAL);
	LtkWindow_SetClientSprite(LTK_WINDOW(wnd), LTK_SPRITE(splitter_h));

	LtkSplitter_Resize(LTK_SPLITTER(splitter_h), 2);

	LtkObject* btn = LtkButton_New();
	LtkSplitter_SetClientAt(LTK_SPLITTER(splitter_h), 0, LTK_SPRITE(btn));
	LtkSplitter_SetClientSize(LTK_SPLITTER(splitter_h), 0, 200);

	LtkObject* splitter_v = LtkSplitter_New(LTK_VERTICAL);
	LtkSplitter_Resize(LTK_SPLITTER(splitter_v), 2);
	LtkSplitter_SetClientAt(LTK_SPLITTER(splitter_h), 1, LTK_SPRITE(splitter_v));

	btn = LtkButton_New();
	LtkSplitter_SetClientAt(LTK_SPLITTER(splitter_v), 0, LTK_SPRITE(btn));
	LtkSplitter_SetClientSize(LTK_SPLITTER(splitter_v), 0, 350);
	btn = LtkButton_New();
	LtkSplitter_SetClientAt(LTK_SPLITTER(splitter_v), 1, LTK_SPRITE(btn));


	LtkWindow_CreateCenter(LTK_WINDOW(wnd), NULL, 800, 600);
	LtkWindow_SetCaption(LTK_WINDOW(wnd), L"Ltk测试窗口");
	LtkWindow_UpdateTheme(LTK_WINDOW(wnd));

	LtkRunMessageLoop();

	LtkFree((LtkObject *)wnd);
	LtkUninitialize();
	return 0;
}


