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

class MainWindow : public ILtkWindowListener, public ILtkActionListener,
	public RefCounted
{
public:
	virtual ~MainWindow();

	void Create();

	STDMETHOD_(ULONG, AddRef)() { return RefCounted::AddRef(); }
	STDMETHOD_(ULONG, Release)() { return RefCounted::Release(); }
	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject);

	//STDMETHOD_(BOOL, OnClose)(ILtkWindow* sender);
	STDMETHOD_(void, OnDestroy)(ILtkWindow* sender);
	STDMETHOD_(void, OnClick)(IUnknown* sender, LPCSTR name);


protected:
	void BuildMenu(ILtkMenuBar **ppMenu);
	void FillListView(ILtkListView* listview);
	void FillTreeView(ILtkFactory *factory, ILtkTreeNode* parent, int depth);

private:
	ILtkWindow* m_window = nullptr;
};