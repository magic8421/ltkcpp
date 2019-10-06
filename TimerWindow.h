#pragma once
#include "Window.h"
#include "ListView.h"
#include "Label.h"

class TimerWindow : public ltk::Window
{
public:
	TimerWindow();
	~TimerWindow();

	void OnTimer();
	void OnAddBtnClicked();

	virtual void OnDestroy() override;

private:
	ltk::ListView *m_listView = nullptr;
	ltk::Label *m_lableTime = nullptr;
	ltk::Timer m_timer1;
};


