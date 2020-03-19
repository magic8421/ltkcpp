#pragma once

#include "resource.h"
#include "LtkInterface.h"

class MainWindow
{
public:
	MainWindow() {}
	~MainWindow();

	void Create();

	BOOL OnMenuClick(LtkEvent* ev);
	BOOL OnWindowClose(LtkEvent* ev);
	BOOL OnWindowDestroy(LtkEvent* ev);


	LTK_CALLBACK_BEGIN(MainWindow, WindowHandler)
		LTK_HANDLE_MENU_CLICK(OnMenuClick)
		LTK_HANDLE_WINDOW_CLOSE(OnWindowClose)
		LTK_HANDLE_WINDOW_DESTROY(OnWindowDestroy)
	LTK_CALLBACK_END()

private:
	LtkObject* m_window = nullptr;
};