#pragma once
#include "LtkInterface.h"
#include "..\Delegate.hpp"
using namespace delegate2;

class LtkObject
{
public:
	explicit LtkObject(HLTK hltk) : m_hltk(hltk) 
	{
		LtkRegisterCallback(m_hltk, LTK_DELETE_EVENT, (LtkCallback)LtkObject::OnDelete, this);
	}

	virtual ~LtkObject() 
	{
		this->DeleteDelegate();
		LtkDelete(m_hltk);
	}
	HLTK GetHandle()
	{
		return m_hltk;
	}
	/*
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
	*/
	void DeleteLater()
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

	MuticastDelegate<> DeleteDelegate;

protected:
	void PostInit()
	{
	}

private:
	static int CALLBACK OnDelete(void* userdata)
	{
		LtkObject* self = (LtkObject*)userdata;
		self->m_hltk = NULL;
		delete self;
		return 0;
	}
protected:
	HLTK m_hltk = NULL;
};

