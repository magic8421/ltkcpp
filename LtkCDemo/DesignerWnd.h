#pragma once
#include "LtkInterface.h"
#include "LtkWindow.h"
#include "WidgetTreeView.h"

class DesignerWnd : public LtkWindow
{
public:
	void Create();
	void BuildMenu();

	virtual void OnDestroy() override;
	virtual void OnClose(BOOL* proceed) override;

private:
	WidgetTreeView *m_wdgtTree = nullptr;
};

