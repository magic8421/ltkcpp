//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "Common.h"
#include "Window.h"
#include "TimerManager.h"

class DemoWindow : public ltk::Window
{
public:
	DemoWindow();
	~DemoWindow();

	void BuildDemoWindow();
	void BuildSplitterTest();
	void BuildSplitterTest2();

	void OnPixelThemeClicked();
	void OnDarkThemeClicked();
	void OnLightThemeClicked();
	void OnExitClicked();

	virtual void OnClose(BOOL* proceed) override;

	void OnListViewSelectChange(int row, int oldRow);

	void InternStringTest();

private:
	UINT m_timer = 0;
	UINT m_onceTimer = 0;
	DISALLOW_COPY_AND_ASSIGN(DemoWindow);
};

