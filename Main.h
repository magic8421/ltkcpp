#pragma once
#include "Window.h"
#include "TimerManager.h"

class DemoWindow : public ltk::Window
{
public:
	DemoWindow();
	~DemoWindow();

	void Create();
	void BuildDemoWindow();

	void OnTimer();
	void OnOnceTimer();

	void OnPixelThemeClicked();
	void OnDarkThemeClicked();
	void OnLightThemeClicked();

private:
	ltk::Timer *m_timer = nullptr;
	ltk::Timer *m_onceTimer = nullptr;
};

