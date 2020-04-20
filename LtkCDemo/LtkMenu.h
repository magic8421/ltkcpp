#pragma once
#include "LtkWidget.h"

class LtkPopupMenu : public LtkWidget
{
public:
	void Create()
	{
		if (!m_bWeak) LtkDelete(m_hltk);
		m_hltk = LtkPopupMenu_New();
	}
	void AddItem(LPCSTR text, LPCSTR name)
	{
		LtkPopupMenu_AddItem(m_hltk, text, name);
	}
	void AddSeparator()
	{
		LtkPopupMenu_AddSeparator(m_hltk);
	}
};

class LtkMenuBar : public LtkWidget
{
public:
	void Create()
	{
		if (!m_bWeak) LtkDelete(m_hltk);
		m_hltk = LtkMenuBar_New();
	}
	UINT AddItem(LPCSTR text)
	{
		return LtkMenuBar_AddItem(m_hltk, text);
	}
	void SetPopupMenu(UINT idx, LtkPopupMenu* popup)
	{
		LtkMenuBar_SetPopupMenu(m_hltk, idx, popup->GetHandle());
	}
};