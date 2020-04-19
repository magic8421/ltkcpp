#pragma once
#include "LtkInterface.h"

class LtkObject
{
public:
	LtkObject() 
	{}

	explicit LtkObject(HLTK hltk) : m_hltk(hltk) {}

	~LtkObject() 
	{
		if (!m_bWeak) {
			LtkDelete(m_hltk);
		}
	}
	LtkObject(LtkObject&& rhs)
	{
		m_hltk = rhs.m_hltk;
		rhs.m_hltk = NULL;
		m_bWeak = rhs.m_bWeak;
	}
	void SetWeak(bool b)
	{
		m_bWeak = b;
	}
	HLTK GetHandle()
	{
		return m_hltk;
	}
	void Attach(HLTK hltk) 
	{
		LtkDelete(m_hltk);
		m_hltk = hltk;
	}
	HLTK Dettach()
	{
		auto tmp = m_hltk;
		m_hltk = NULL;
		return tmp;
	}
	void Destroy()
	{
		LtkDelete(m_hltk);
		m_hltk = NULL;
	}
	void DestroyLater()
	{
		LtkDeleteLater(m_hltk);
	}
	void SetName(LPCSTR name)
	{
		LtkSetName(m_hltk, name);
	}
	LPCSTR GetName()
	{
		return LtkGetName(m_hltk);
	}
	void RegisterCallback(UINT event_id, LtkCallback cb, void* userdata)
	{
		LtkRegisterCallback(m_hltk, event_id, cb, userdata);
	}

protected:
	HLTK m_hltk = NULL;
	bool m_bWeak = false;
};

