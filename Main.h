//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "Window.h"
#include "TimerManager.h"

class DemoWindow : public ltk::Window
{
public:
	DemoWindow();
	~DemoWindow();

	void BuildDemoWindow();
	void BuildSplitterTest();

	void OnTimer();
	void OnOnceTimer();

	void OnPixelThemeClicked();
	void OnDarkThemeClicked();
	void OnLightThemeClicked();
	void OnExitClicked();

	virtual void OnClose(bool &proceed) override;

private:
	ltk::Timer *m_timer = nullptr;
	ltk::Timer *m_onceTimer = nullptr;
};

