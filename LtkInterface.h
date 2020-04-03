#pragma once

#ifdef LTK_EXPORTS
#define LTK_API
#else
#define LTK_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define LTK_DECLARE_TYPE(type) \
 typedef struct _##type { \
	int dummy; \
} type

typedef struct _LtkRect {
	float x;
	float y;
	float width;
	float height;
} LtkRect;

LTK_API UINT WINAPI LtkInitialize();
LTK_API void WINAPI LtkUninitialize();
LTK_API void WINAPI LtkRunMessageLoop();

struct _HLTK {
	int dummy;
};
typedef _HLTK* HLTK;

LTK_API void WINAPI LtkFree(HLTK obj);

typedef struct _LtkEvent {
	UINT id;
	HLTK sender;
} LtkEvent;

typedef void (CALLBACK *LtkCallback)();

LTK_API LPCSTR WINAPI LtkInternString(LPCSTR str);

LTK_API void WINAPI LtkSetName(HLTK o, LPCSTR name);
LTK_API LPCSTR WINAPI LtkGetName(HLTK o);

LTK_API void WINAPI LtkRegisterCallback(HLTK obj, UINT event_id, LtkCallback cb, void* userdata);
LTK_API void WINAPI LtkUnregisterCallback(HLTK obj, UINT event_id, LtkCallback cb, void* userdata);
LTK_API void WINAPI LtkUnregisterCallbackByUserdata(HLTK obj, UINT event_id, void* userdata);

LTK_API HLTK WINAPI LtkGetEventSender();

LTK_API void* WINAPI LtkGetProp(LPCSTR name);
LTK_API void WINAPI LtkSetProp(LPCSTR name, void *data);


#define LTK_CREATE			1
#define LTK_DESTROY			2
#define LTK_PAINT			3
#define LTK_SIZE			4

#define LTK_MOUSE_FIRST			5
#define LTK_MOUSE_MOVE			6
#define LTK_MOUSE_LEAVE			7
#define LTK_MOUSE_WHEEL			8
#define LTK_LBUTTON_DOWN		9
#define LTK_LBUTTON_UP			10
#define LTK_LBUTTON_DBCLICK		11
#define LTK_RBUTTON_DOWN		12
#define LTK_RBUTTON_UP			13
#define LTK_MOUSE_LAST			14

#define LTK_KEY_DOWN			15
#define LTK_KEY_UP				16
#define LTK_CHAR				17
#define LTK_IME_INPUT			18

#define LTK_SET_FOCUS			19
#define LTK_KILL_FOCUS			20

#define LTK_RECREATE_RESOURCE	21
#define LTK_THEME_CHANGED		22

typedef struct _LtkMouseEvent
{
	LtkEvent hdr;
	float x;
	float y;
	UINT flag;		// ctrl shift atl
	float delta;	// wheel
} LtkMouseEvent;

typedef struct _LtkDelegateMouseEvent
{
	LtkEvent hdr;
	LtkMouseEvent* data;
	BOOL bHandled;
} LtkDelegateMouseEvent;

typedef struct _LtkKeyEvent
{
	LtkEvent hdr;
	DWORD keyCode;
	DWORD flag;
} LtkKeyEvent;

struct ID2D1RenderTarget;

typedef struct _LtkPaintEvent
{
	LtkEvent hdr;
	ID2D1RenderTarget* target;
} LtkPaintEvent;

typedef struct _LtkSizeEvent
{
	LtkEvent hdr;
	float width;
	float height;
} LtkSizeEvent;

typedef struct _LtkImeEvent
{
	LtkEvent hdr;
	LPCWSTR text;
} LtkImeEvent;

typedef struct _LtkFocusEvent
{
	LtkEvent hdr;
	HLTK oldFocus;
} LtkFocusEvent;

typedef struct _LtkRecreateResource {
	LtkEvent hdr;
	ID2D1RenderTarget* target;
} LtkRecreateResource;


// LtkMenuBar 基类：LtkWidget
// LtkPopupMenu 基类：LtkWidget


// (void *userdata, BOOL *bHandled)
#define LTK_WINDOW_DESTROY		101
// (void *userdata, BOOL *pProceed, BOOL *bHandled)
#define LTK_WINDOW_CLOSE		102


#define LtkWindow_New() LtkWindow_New_(__FILE__, __LINE__)
LTK_API HLTK WINAPI LtkWindow_New_(LPCSTR source, int line);

LTK_API void WINAPI LtkWindow_Create(HLTK self, HWND parent, LtkRect* rc);
LTK_API void WINAPI LtkWindow_CreateCenter(HLTK self, HWND parent, float width, float height);
LTK_API void WINAPI LtkWindow_SetCaption(HLTK self, LPCWSTR text);
LTK_API void WINAPI LtkWindow_SetBackground(HLTK self, LPCSTR name);
LTK_API void WINAPI LtkWindow_UpdateTheme(HLTK self);
LTK_API void WINAPI LtkWindow_SetCentralWidget(HLTK self, HLTK widget);
LTK_API void WINAPI LtkWindow_SetMenu(HLTK self, HLTK menu_bar);
LTK_API HWND WINAPI LtkWindow_GetHWND(HLTK self);

// LtkBoxLayout 基类：LtkWidget

#define LTK_HORIZONTAL  1
#define LTK_VERTICAL    2

#define LtkBoxLayout_New(o) LtkBoxLayout_New_(o, __FILE__, __LINE__)
LTK_API HLTK WINAPI LtkBoxLayout_New_(UINT orientation, LPCSTR source, int line);

LTK_API void WINAPI LtkBoxLayout_AddLayoutItem(HLTK self, HLTK widget, float preferedSize, float growFactor);
LTK_API void WINAPI LtkBoxLayout_AddSpaceItem(HLTK self, float preferedSize, float growFactor);
LTK_API void WINAPI LtkBoxLayout_SetSpacing(HLTK self, float spacing);

// LtkButton 基类：LtkWidget

// void CALLBACK OnButtonClicked(void* userdata)
#define LTK_BUTTON_CLICKED		201

#define LtkButton_New() LtkButton_New_(__FILE__, __LINE__)
LTK_API HLTK WINAPI LtkButton_New_(LPCSTR source, int line);

LTK_API void WINAPI LtkButton_SetText(HLTK self, LPCWSTR);

// LtkHeaderCtrl 基类：LtkWidget

#define LtkHeaderCtrl_New() LtkHeaderCtrl_New_(  __FILE__, __LINE__)
LTK_API HLTK WINAPI LtkHeaderCtrl_New_(LPCSTR source, int line);

LTK_API void WINAPI LtkHeaderCtrl_AddColumn(HLTK self, LPCWSTR text, float width);



// LtkListView 基类：LtkWidget

// void CALLBACK OnListViewSelectChange(void* userdata, int row, int oldRow)
#define LTK_LISTVIEW_SELECT_CHANGE		301

// void CALLBACK OnListViewDbClick(void* userdata, int row) // TODO NOT IMPLEMENTED
#define LTK_LISTVIEW_DBCLICK			302


#define LtkListView_New() LtkListView_New_( __FILE__, __LINE__)
LTK_API HLTK WINAPI LtkListView_New_(LPCSTR source, int line);

LTK_API UINT WINAPI LtkListView_AddRow(HLTK self);
LTK_API void WINAPI LtkListView_SetCellText(HLTK self, UINT row, UINT col, LPCWSTR text);
LTK_API LPCWSTR WINAPI LtkListView_GetCellText(HLTK self, UINT row, UINT col);
LTK_API int WINAPI LtkListView_GetSelectedRow(HLTK self);
LTK_API HLTK WINAPI LtkListView_GetHeaderCtrl(HLTK self);
LTK_API void WINAPI LtkListView_UpdateColumnWidth(HLTK self);

// LtkSplitter 基类：LtkWidget

#define LtkSplitter_New(o) LtkSplitter_New_(o,  __FILE__, __LINE__)
LTK_API HLTK WINAPI LtkSplitter_New_(UINT orientation, LPCSTR source, int line);

LTK_API void WINAPI LtkSplitter_Resize(HLTK self, UINT n);
LTK_API HLTK WINAPI LtkSplitter_SetClientAt(HLTK self, UINT idx, HLTK widget);
//LTK_API LtkSplitter* WINAPI LtkSplitter_GetClientAt(LtkSplitter* self, UINT idx);
//LTK_API	UINT WINAPI LtkSplitter_GetClientCount(LtkSplitter* self);
LTK_API void WINAPI LtkSplitter_SetClientSize(HLTK self, UINT idx, float size);
//LTK_API float WINAPI LtkSplitter_GetClientSize(LtkSplitter* self, UINT idx);

// LtkTreeView 基类：LtkWidget

#define LtkTreeView_New() LtkTreeView_New_(  __FILE__, __LINE__)
LTK_API HLTK WINAPI LtkTreeView_New_(LPCSTR source, int line);

// LtkTreeNode 基类：LtkObject

LTK_API HLTK WINAPI LtkTreeView_GetRootNode(HLTK self);

#define LTK_TREEVIEW_FIRST 500
#define LTK_TREEVIEW_SELECT_CHANGE (LTK_TREEVIEW_FIRST + 1)

#define LtkTreeNode_New() LtkTreeNode_New_(  __FILE__, __LINE__)
LTK_API HLTK WINAPI LtkTreeNode_New_(LPCSTR source, int line);

LTK_API void WINAPI LtkTreeNode_AddChild(HLTK self, HLTK node);
LTK_API void WINAPI LtkTreeNode_SetText(HLTK self, LPCWSTR text);

LTK_DECLARE_TYPE(LtkTextEdit); // 基类：LtkWidget

#define LTK_TEXTEDIT(o) LtkIsTextEdit(o) ? (LtkTextEdit*)o : NULL
LTK_API BOOL WINAPI LtkIsTextEdit(HLTK o);

#define LtkTextEdit_New() LtkTextEdit_New_(  __FILE__, __LINE__)
LTK_API HLTK WINAPI LtkTextEdit_New_(LPCSTR source, int line);

LTK_DECLARE_TYPE(LtkMenuItem);

#define LTK_MENU_CLICK 601

#define LtkMenuBar_New() LtkMenuBar_New_(  __FILE__, __LINE__)
LTK_API HLTK WINAPI LtkMenuBar_New_(LPCSTR source, int line);

LTK_API void WINAPI LtkMenuBar_AddItem(HLTK self, LPCWSTR text);
LTK_API void WINAPI LtkMenuBar_SetPopupMenu(HLTK self, UINT idx, HLTK popup);

#define LtkPopupMenu_New() LtkPopupMenu_New_(  __FILE__, __LINE__)
LTK_API HLTK WINAPI LtkPopupMenu_New_(LPCSTR source, int line);

LTK_API void WINAPI LtkPopupMenu_AddItem(HLTK self, LPCWSTR text, LPCSTR name);
LTK_API void WINAPI LtkPopupMenu_AddSeparator(HLTK self);
LTK_API void WINAPI LtkPopupMenu_SetWidth(HLTK self, float width);
LTK_API void WINAPI LtkPopupMenu_SetSubMenu(HLTK self, UINT idx, HLTK popup);

#ifdef __cplusplus
} // extern "C"
#endif
