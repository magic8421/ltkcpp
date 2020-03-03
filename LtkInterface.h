#pragma once

#ifdef LTK_EXPORTS
#define LTK_API __declspec(dllexport)
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
	float w;
	float h;
} LtkRect;

LTK_API UINT WINAPI LtkInitialize();
LTK_API void WINAPI LtkUninitialize();
LTK_API void WINAPI LtkRunMessageLoop();

LTK_DECLARE_TYPE(LtkObject);

LTK_API void WINAPI LtkFree(LtkObject *);

typedef struct _LtkEvent {
	UINT id;
	LtkObject* sender;
} LtkEvent;

typedef void (CALLBACK *LtkCallback)(void* userdata, LtkEvent* ev);

LTK_API void WINAPI LtkObject_SetName(LtkObject* o, LPCSTR name);
LTK_API LPCSTR WINAPI LtkObject_GetName(LtkObject* o);

LTK_API void WINAPI LtkObject_AddListener(LtkObject* o, void* userdata, LtkCallback callback);
LTK_API void WINAPI LtkObject_RemoveListener(LtkObject* o, void* userdata, LtkCallback callback);


LTK_DECLARE_TYPE(LtkWindow); // 基类：LtkObject

LTK_DECLARE_TYPE(LtkSprite); // 基类：LtkObject

LTK_DECLARE_TYPE(LtkMenuBar); // 基类：LtkSprite
LTK_DECLARE_TYPE(LtkPopupMenu); // 基类：LtkSprite

#define LTK_SPRITE(o) LtkIsSprite(o) ? (LtkSprite*)o : NULL
LTK_API BOOL WINAPI LtkIsSprite(LtkObject* o);

// void CALLBACK OnWindowDestory(void* userdata)
#define LTK_WINDOW_DESTROY		100

// void CALLBACK OnWindowClose(void* userdata, BOOL *pProceed);
#define LTK_WINDOW_CLOSE		101

#define LTK_WINDOW(o) LtkIsWindow(o) ? (LtkWindow*)o : NULL
LTK_API BOOL WINAPI LtkIsWindow(LtkObject* o);

#define LtkWindow_New() LtkWindow_New_(__FILE__, __LINE__)
LTK_API LtkObject* WINAPI LtkWindow_New_(LPCSTR source, int line);

LTK_API void WINAPI LtkWindow_Create(LtkWindow*self, LtkWindow* parent, LtkRect* rc);
LTK_API void WINAPI LtkWindow_CreateCenter(LtkWindow*self, LtkWindow* parent, float width, float height);
LTK_API void WINAPI LtkWindow_SetCaption(LtkWindow*self, LPCWSTR text);
LTK_API void WINAPI LtkWindow_SetBackground(LtkWindow* self, LPCSTR name);
LTK_API void WINAPI LtkWindow_UpdateTheme(LtkWindow* self);
LTK_API void WINAPI LtkWindow_SetClientSprite(LtkWindow* self, LtkSprite* sp);
LTK_API void WINAPI LtkWindow_SetMenu(LtkWindow* self, LtkMenuBar* menu);

LTK_DECLARE_TYPE(LtkBoxLayout); // 基类：LtkSprite

#define LTK_HORIZONTAL  1
#define LTK_VERTICAL    2

#define LTK_BOXLAYOUT(o) LtkIsBoxLayout(o) ? (LtkBoxLayout*)o
LTK_API BOOL WINAPI LtkIsBoxLayout(LtkObject* o);

#define LtkBoxLayout_New(o) LtkBoxLayout_New_(o, __FILE__, __LINE__)
LTK_API LtkObject* WINAPI LtkBoxLayout_New_(UINT orientation, LPCSTR source, int line);

LTK_API void WINAPI LtkBoxLayout_AddLayoutItem(LtkBoxLayout* self, LtkSprite *sp, float preferedSize, float growFactor);
LTK_API void WINAPI LtkBoxLayout_AddSpaceItem(LtkBoxLayout* self, float preferedSize, float growFactor);
LTK_API void WINAPI LtkBoxLayout_SetSpacing(LtkBoxLayout* self, float spacing);

LTK_DECLARE_TYPE(LtkButton); // 基类：LtkSprite

// void CALLBACK OnButtonClicked(void* userdata)
#define LTK_BUTTON_CLICKED		201

#define LTK_BUTTON(o) LtkIsButton(o) ? (LtkButton*)o : NULL
LTK_API BOOL WINAPI LtkIsButton(LtkObject* o);

#define LtkButton_New() LtkButton_New_(__FILE__, __LINE__)
LTK_API LtkObject* WINAPI LtkButton_New_(LPCSTR source, int line);

LTK_API void WINAPI LtkButton_SetText(LtkButton* self, LPCWSTR);

LTK_DECLARE_TYPE(LtkHeaderCtrl); // 基类：LtkSprite

#define LTK_HEADERCTRL(o) LtkIsHeaderCtrl(o) ? (LtkHeaderCtrl*)o : NULL
LTK_API BOOL WINAPI LtkIsHeaderCtrl(LtkObject* o);

#define LtkHeaderCtrl_New() LtkHeaderCtrl_New_(  __FILE__, __LINE__)
LTK_API LtkObject* WINAPI LtkHeaderCtrl_New_(LPCSTR source, int line);

LTK_API void WINAPI LtkHeaderCtrl_AddColumn(LtkHeaderCtrl* self, LPCWSTR text, float width);

LTK_DECLARE_TYPE(LtkListView); // 基类：LtkSprite

// void CALLBACK OnListViewSelectChange(void* userdata, int row, int oldRow)
#define LTK_LISTVIEW_SELECT_CHANGE		301

// void CALLBACK OnListViewDbClick(void* userdata, int row) // TODO NOT IMPLEMENTED
#define LTK_LISTVIEW_DBCLICK			302

#define LTK_LISTVIEW(o) LtkIsListView(o) ? (LtkListView*)o : NULL
LTK_API BOOL WINAPI LtkIsListView(LtkObject* o);

#define LtkListView_New() LtkListView_New_( __FILE__, __LINE__)
LTK_API LtkObject* WINAPI LtkListView_New_(LPCSTR source, int line);

LTK_API UINT WINAPI LtkListView_AddRow(LtkListView* self);
LTK_API void WINAPI LtkListView_SetCellText(LtkListView* self, UINT row, UINT col, LPCWSTR text);
LTK_API LPCWSTR WINAPI LtkListView_GetCellText(LtkListView* self, UINT row, UINT col);
LTK_API int WINAPI LtkListView_GetSelectedRow(LtkListView* self);
LTK_API LtkObject* WINAPI LtkListView_GetHeaderCtrl(LtkListView* self);
LTK_API void WINAPI LtkListView_UpdateColumnWidth(LtkListView* self);

LTK_DECLARE_TYPE(LtkSplitter); // 基类：LtkSprite

#define LTK_SPLITTER(o) LtkIsSplitter(o) ? (LtkSplitter*)o : NULL
LTK_API BOOL WINAPI LtkIsSplitter(LtkObject* o);

#define LtkSplitter_New(o) LtkSplitter_New_(o,  __FILE__, __LINE__)
LTK_API LtkObject* WINAPI LtkSplitter_New_(UINT orientation, LPCSTR source, int line);

LTK_API void WINAPI LtkSplitter_Resize(LtkSplitter* self, UINT n);
LTK_API LtkSprite* WINAPI LtkSplitter_SetClientAt(LtkSplitter* self, UINT idx, LtkSprite* sp);
LTK_API LtkSplitter* WINAPI LtkSplitter_GetClientAt(LtkSplitter* self, UINT idx);
LTK_API	UINT WINAPI LtkSplitter_GetClientCount(LtkSplitter* self);
LTK_API void WINAPI LtkSplitter_SetClientSize(LtkSplitter* self, UINT idx, float size);
LTK_API float WINAPI LtkSplitter_GetClientSize(LtkSplitter* self, UINT idx);

LTK_DECLARE_TYPE(LtkTreeView); // 基类：LtkSprite

// void CALLBACK OnTreeViewSelectChange(void* userdata, LtkTreeNode* node, LtkTreeNode* oldNode)
#define LTK_TREEVIEW_SELECT_CHANGE		401

#define LTK_TREEVIEW(o) LtkIsTreeView(o) ? (LtkTreeView*)o : NULL
LTK_API BOOL WINAPI LtkIsTreeView(LtkObject* o);

#define LtkTreeView_New() LtkTreeView_New_(  __FILE__, __LINE__)
LTK_API LtkObject* WINAPI LtkTreeView_New_(LPCSTR source, int line);

LTK_DECLARE_TYPE(LtkTreeNode); // 基类：LtkObject

LTK_API LtkObject* WINAPI LtkTreeView_GetRootNode(LtkTreeView* self);

#define LTK_TREENODE(o) LtkIsTreeNode(o) ? (LtkTreeNode*)o : NULL
LTK_API BOOL WINAPI LtkIsTreeNode(LtkObject* o);

#define LtkTreeNode_New() LtkTreeNode_New_(  __FILE__, __LINE__)
LTK_API LtkObject* WINAPI LtkTreeNode_New_(LPCSTR source, int line);

LTK_API void WINAPI LtkTreeNode_AddChild(LtkTreeNode* self, LtkTreeNode* node);
LTK_API void WINAPI LtkTreeNode_SetText(LtkTreeNode* self, LPCWSTR text);

LTK_DECLARE_TYPE(LtkTextEdit); // 基类：LtkSprite

#define LTK_TEXTEDIT(o) LtkIsTextEdit(o) ? (LtkTextEdit*)o : NULL
LTK_API BOOL WINAPI LtkIsTextEdit(LtkObject* o);

#define LtkTextEdit_New() LtkTextEdit_New_(  __FILE__, __LINE__)
LTK_API LtkObject* WINAPI LtkTextEdit_New_(LPCSTR source, int line);

#define LTK_MENU_CLICK 501

#define LTK_MENUBAR(o) LtkIsMenuBar(o) ? (LtkMenuBar*)o : NULL
LTK_API BOOL WINAPI LtkIsMenuBar(LtkObject* o);

#define LtkMenuBar_New() LtkMenuBar_New_(  __FILE__, __LINE__)
LTK_API LtkObject* WINAPI LtkMenuBar_New_(LPCSTR source, int line);

LTK_API void WINAPI LtkMenuBar_AddItem(LtkMenuBar* self, LPCWSTR text);
LTK_API void WINAPI LtkMenuBar_SetPopupMenu(LtkMenuBar* self, UINT idx, LtkPopupMenu* popup);

#define LTK_POPUPMENU(o) LtkIsPopupMenu(o) ? (LtkPopupMenu*)o : NULL
LTK_API BOOL WINAPI LtkIsPopupMenu(LtkObject* o);

#define LtkPopupMenu_New() LtkPopupMenu_New_(  __FILE__, __LINE__)
LTK_API LtkObject* WINAPI LtkPopupMenu_New_(LPCSTR source, int line);

LTK_API void WINAPI LtkPopupMenu_AddItem(LtkPopupMenu* self, LPCWSTR text, LPCSTR name);
LTK_API void WINAPI LtkPopupMenu_SetWidth(LtkPopupMenu* self, float width);
LTK_API void WINAPI LtkPopupMenu_SetSubMenu(LtkPopupMenu* self, UINT idx, LtkPopupMenu* popup);

#ifdef __cplusplus
} // extern "C"
#endif
