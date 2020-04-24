#pragma once
#include "LtkWidget.h"

class LtkSplitter : public LtkWidget
{
public:
	LtkSplitter(UINT orientation) : LtkWidget(LtkSplitter_New(orientation))
	{}
	UINT AddClient(LtkWidget* wdgt)
	{
		return LtkSplitter_AddClient(m_hltk, wdgt->GetHandle());
	}
	void SetClientSize(UINT idx, float size)
	{
		LtkSplitter_SetClientSize(m_hltk, idx, size);
	}
};