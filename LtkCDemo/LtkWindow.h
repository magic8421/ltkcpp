#pragma once
#include "LtkWidget.h"

class LtkWindow : public LtkObject
{
public:
	void Create()
	{
		if (!m_bWeak) LtkDelete(m_hltk);
		m_hltk = LtkWindow_New();
		LtkRegisterCallback(m_hltk, LTK_WINDOW_CLOSE, (LtkCallback)LtkWindow::OnWindowClose, this);
		LtkRegisterCallback(m_hltk, LTK_WINDOW_DESTROY, (LtkCallback)LtkWindow::OnWindowDestroy, this);
	}
	void CreateHwnd(HWND parent, LtkRect* rc)
	{
		LtkWindow_Create(m_hltk, parent, rc);
	}
	void CreateHwndCenter(HWND parent, float width, float height)
	{
		LtkWindow_CreateCenter(m_hltk, parent, width, height);
	}
	void SetCaption(LPCSTR text)
	{
		LtkWindow_SetCaption(m_hltk, text);
	}
	void SetCentralWidget(LtkWidget* wdgt)
	{
		LtkWindow_SetCentralWidget(m_hltk, wdgt->GetHandle());
		wdgt->SetWeak(true);
	}

protected:
	virtual void OnClose(BOOL* proceed) {}
	virtual void OnDestroy() {}

private:
	static int CALLBACK OnWindowClose(void* ud, BOOL* proceed)
	{
		static_cast<LtkWindow*>(ud)->OnClose(proceed);
		return 0;
	}
	static int CALLBACK OnWindowDestroy(void* ud)
	{
		static_cast<LtkWindow*>(ud)->OnDestroy();
		return 0;
	}

};