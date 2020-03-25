#pragma once

#include <unknwn.h>

#ifdef LTK_EXPORTS
#define LTK_API __declspec(dllexport)
#else
#define LTK_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LtkRect {
	float x;
	float y;
	float width;
	float height;
} LtkRect;

typedef struct LtkSize {
	float width;
	float height;
} LtkSize;

typedef struct LtkMargin {
	float left;
	float top;
	float right;
	float bottom;
} LtkMargin;

interface ILtkFactory;

#define LTK_VERSION "20200322"

LTK_API UINT WINAPI LtkInitialize(LPCSTR version);
LTK_API void WINAPI LtkUninitialize();
LTK_API void WINAPI LtkRunMessageLoop();
LTK_API void WINAPI LtkGetFactory(ILtkFactory** ppFactory);

#ifdef __cplusplus
} // extern "C"
#endif

#ifndef LTK_DECLARE_INTERFACE
#define LTK_DECLARE_INTERFACE(x) DECLSPEC_UUID(x) DECLSPEC_NOVTABLE
#endif


#undef CreateWindow

interface ILtkWidget;
interface ILtkSplitter;
interface ILtkButton;
interface ILtkPopupMenu;
interface ILtkMenuBar;
interface ILtkWindowListener;
interface ILtkActionListener;
interface ILtkListView;
interface ILtkTreeView;
interface ILtkTreeNode;

interface LTK_DECLARE_INTERFACE("F5A12F11-D3EE-41C8-8712-2699D2EEAD87")
	ILtkWindow : public IUnknown
{
	STDMETHOD(Create)(HWND hParent, LtkRect * rect) PURE;
	STDMETHOD(CreateCentered)(HWND hParent, LtkSize * size) PURE;
	STDMETHOD_(void, Dispose)() PURE;
	STDMETHOD_(void, UpdateTheme)() PURE;
	STDMETHOD_(HWND, GetHWND)() PURE;
	STDMETHOD_(void, SetCentralWidget)(ILtkWidget * w) PURE;
	STDMETHOD_(void, SetMenuBar)(ILtkMenuBar * mb) PURE;
	STDMETHOD_(void, GetMenuBar)(ILtkMenuBar ** ) PURE;
	STDMETHOD_(void, SetCaption)(LPCWSTR text) PURE;
	STDMETHOD_(void, SetWindowListener)(ILtkWindowListener * listener) PURE;
	STDMETHOD_(void, SetActionListener)(ILtkActionListener* listener) PURE;
};

interface LTK_DECLARE_INTERFACE("F617B2F6-EA75-41E7-AB0F-595DF6EF3B61")
	ILtkWindowListener : public IUnknown
{
	STDMETHOD_(BOOL, OnClose)(ILtkWindow *sender) { return FALSE; }
	STDMETHOD_(void, OnDestroy)(ILtkWindow *sender) {}
};

typedef enum LTK_ORIENTATION
{
	LTK_HORIZONTAL = 1,
	LTK_VERTICAL = 2
} LTK_ORIENTATION;

interface LTK_DECLARE_INTERFACE("45F1AC62-D035-4223-A3EB-08961DF3A16E") 
	ILtkFactory : public IUnknown
{
	STDMETHOD_(void, CreateWindow)(ILtkWindow** ppOut) PURE;
	STDMETHOD_(void, CreateSplitter)(LTK_ORIENTATION o, ILtkSplitter** ppOut) PURE;
	STDMETHOD_(void, CreateButton)(ILtkButton** ppOut) PURE;
	STDMETHOD_(void, CreateMenuBar)(ILtkMenuBar** ppOut) PURE;
	STDMETHOD_(void, CreatePopupMenu)(ILtkPopupMenu** ppOut) PURE;
	STDMETHOD_(void, CreateListView)(ILtkListView** ppOut) PURE;
	STDMETHOD_(void, CreateTreeView)(ILtkTreeView** ppOut) PURE;
	STDMETHOD_(void, CreateTreeNode)(ILtkTreeNode** ppOut) PURE;
};

interface LTK_DECLARE_INTERFACE("A0B263F7-0B6D-43A6-8A77-A6BF1838D927")
	ILtkWidget : public IUnknown
{
	STDMETHOD_(void, SetVisible)(BOOL) PURE;
	STDMETHOD_(BOOL, GetVisible)() PURE;
	STDMETHOD_(void, SetRect)(LtkRect * rect) PURE;
	STDMETHOD_(void, SetRect)(float x, float y, float width, float height) PURE;
	STDMETHOD(AddChild)(ILtkWidget * w) PURE;
};

interface LTK_DECLARE_INTERFACE("F4241511-42F2-4E64-8FC7-6B9433D6D18A")
	ILtkSplitter : public ILtkWidget
{
	STDMETHOD_(void, AddClient)(ILtkWidget * w) PURE;
	STDMETHOD(SetClientSize)(UINT idx, float size) PURE;
};

interface LTK_DECLARE_INTERFACE("67D14A90-8C1E-4696-B9E6-34B461ABB72E")
	ILtkButton : public ILtkWidget
{
	STDMETHOD_(void, SetText)(LPCWSTR) PURE;
};

interface LTK_DECLARE_INTERFACE("24DB015C-87D9-40F3-ABF8-30AE2E9DB96A")
	ILtkMenuBar : public ILtkWidget
{
	STDMETHOD_(void, AddItem)(LPCWSTR text) PURE;
	STDMETHOD_(void, SetPopupMenu)(UINT idx, ILtkPopupMenu *menu) PURE;
};

interface LTK_DECLARE_INTERFACE("8DAA2BFB-B41D-4B9C-BA68-CDF37F503421")
	ILtkPopupMenu : public ILtkWidget
{
	STDMETHOD_(void, AddItem)(LPCWSTR text, LPCSTR name) PURE;
	STDMETHOD_(void, AddSeparator)() PURE;
	STDMETHOD_(void, SetWidth)(float) PURE;
	STDMETHOD_(void, SetSubMenu)(UINT idx, ILtkPopupMenu *popup) PURE;
};

interface LTK_DECLARE_INTERFACE("5BCFA56C-3332-4088-90CB-5070F3F55B2A")
	ILtkListView : public ILtkWidget
{
	STDMETHOD_(void, AddColumn)(LPCWSTR text, float size) PURE;
	STDMETHOD_(UINT, AddRow)() PURE;
	STDMETHOD(SetCellText)(UINT row, UINT col, LPCWSTR text) PURE;

};

interface LTK_DECLARE_INTERFACE("51B6E813-AE89-4394-86C4-2EE96EE1150F")
	ILtkTreeView : public ILtkWidget
{
	STDMETHOD_(ILtkTreeNode*, GetRootNode)() PURE;
};

interface LTK_DECLARE_INTERFACE("187888DE-0539-46CE-A499-A765A7BC9205")
	ILtkTreeNode : public IUnknown
{
	STDMETHOD_(void, SetText)(LPCWSTR) PURE;
	STDMETHOD_(void, AddChildNode)(ILtkTreeNode *) PURE;
};

interface ILtkAction : public IUnknown // TODO 
{

};

interface LTK_DECLARE_INTERFACE("4DD8353D-2D90-489E-A9C3-49B8341279B8")
	ILtkActionListener : public IUnknown
{
	STDMETHOD_(void, OnClick)(IUnknown *sender, LPCSTR name) PURE;
};
