#pragma once
#include "LtkWidget.h"

class LtkSplitter : public LtkWidget
{
public:
	void Create(UINT orientation)
	{
		if (!m_bWeak) LtkDelete(m_hltk);
		m_hltk = LtkSplitter_New(orientation);
	}
	UINT AddClient(LtkWidget* wdgt)
	{
		wdgt->SetWeak(true);
		return LtkSplitter_AddClient(m_hltk, wdgt->GetHandle());
	}
	void SetClientSize(UINT idx, float size)
	{
		LtkSplitter_SetClientSize(m_hltk, idx, size);
	}
};