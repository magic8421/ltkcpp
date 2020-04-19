#pragma once
#include "LtkWidget.h"

class LtkButton : public LtkWidget
{
public:
	void Create()
	{
		if (!m_bWeak) LtkDelete(m_hltk);
		m_hltk = LtkButton_New();
	}
	void SetText(LPCSTR text)
	{
		LtkButton_SetText(m_hltk, text);
	}
};