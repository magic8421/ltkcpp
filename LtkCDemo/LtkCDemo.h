#pragma once

#include "resource.h"
#include "LtkInterface.h"

#define SAFE_RELEASE(p) if (p) { p->Release(); p = NULL; }

class RefCounted {
public:
	virtual ~RefCounted() {}

	STDMETHOD_(ULONG, AddRef)() {
		InterlockedIncrement(&m_cRef);
		return m_cRef;
	}
	STDMETHOD_(ULONG, Release)() {
		ULONG ulRefCount = InterlockedDecrement(&m_cRef);
		if (0 == m_cRef) {
			delete this;
		}
		return ulRefCount;
	}
private:
	volatile ULONG m_cRef = 1;
};

class MainWindow : public RefCounted, public ILtkWindowListener
{
public:
	virtual ~MainWindow();

	STDMETHOD_(ULONG, AddRef)() { return RefCounted::AddRef(); }
	STDMETHOD_(ULONG, Release)() { return RefCounted::Release(); }
	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject);

	STDMETHOD_(BOOL, OnClose)(ILtkWindow* sender);
	STDMETHOD_(void, OnDestroy)(ILtkWindow* sender);

	void Create();

private:
	ILtkWindow* m_window = nullptr;
};