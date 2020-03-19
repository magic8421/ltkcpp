#pragma once

#include "resource.h"
#include "LtkInterface.h"

class MainWindow : public ILtkWindowListener
{
public:
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();
	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject);

	STDMETHOD_(BOOL, OnClose)();
	STDMETHOD_(void, OnDestroy)();

	void Create();
};