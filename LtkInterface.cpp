#include "stdafx.h"
#include "LtkInterface.h"
#include "ltk.h"
#include "Object.h"
#include "Window.h"
#include "Button.h"
#include "BoxLayout.h"
#include "ListView.h"
#include "Splitter.h"
#include "TreeView.h"
#include "TextEdit.h"
#include "MenuBar.h"
#include "StyleManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

using namespace ltk;

LTK_API UINT WINAPI LtkInitialize(LPCSTR version)
{
	if (strcmp(version, LTK_VERSION) != 0) {
		::MessageBox(0, L"Ltk界面库版本不匹配", 0, 0);
		::TerminateProcess(::GetCurrentProcess(), 1);
	}
	ltk::LtkInitialize();
	return 0;
}

LTK_API void WINAPI LtkUninitialize()
{
	ltk::LtkUninitialize();
}

LTK_API void WINAPI LtkRunMessageLoop()
{
	MSG msg;
	BOOL bRet;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if (bRet == -1) {
			// ???
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

class LtkFacotry : public Object, public ILtkFactory
{
public:
	STDMETHOD_(ULONG, AddRef)() { return Object::AddRef(); }

	STDMETHOD_(ULONG, Release)() { return Object::Release(); }

	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject) { return E_NOTIMPL; }

	void WINAPI CreateWindow(ILtkWindow** ppWnd) override {
		*ppWnd = new Window();
	}
	STDMETHOD_(void, CreateSplitter)(LTK_ORIENTATION o, ILtkSplitter **ppOut) override {
		*ppOut = new Splitter((ltk::Orientation)o);
	}
	STDMETHOD_(void, CreateButton)(ILtkButton ** ppOut) override {
		*ppOut = new Button;
	}
	STDMETHOD_(void, CreateMenuBar)(ILtkMenuBar ** ppOut) override {
		*ppOut = new MenuBar;
	}
	STDMETHOD_(void, CreatePopupMenu)(ILtkPopupMenu ** ppOut) override {
		*ppOut = new PopupMenu;
	}
	STDMETHOD_(void, CreateListView)(ILtkListView** ppOut) override {
		*ppOut = new ListView;
	}
};

LTK_API void WINAPI LtkGetFactory(ILtkFactory** ppFactory)
{
	*ppFactory = new LtkFacotry;
}
