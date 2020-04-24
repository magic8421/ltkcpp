#pragma once
#include "LtkWidget.h"

class LtkButton : public LtkWidget
{
public:
	LtkButton() : LtkWidget(LtkButton_New())
	{}
	void SetText(LPCSTR text)
	{
		LtkButton_SetText(m_hltk, text);
	}
};