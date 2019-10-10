#pragma once

#ifdef LTK_EXPORTS
#define LTK_API __declspec(dllexport)
#else
#define LTK_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	enum LtkError
	{
		LTK_OK = 0,
		LTK_FAILED,
		LTK_NO_IMPLEMENT
	};

	typedef struct _LtkObject {
		int dummy;
	} LtkObject;

	typedef struct _LtkRect {
		float x;
		float y;
		float w;
		float h;
	} LtkRect;

	typedef void (CALLBACK *LtkCallback)();

	// void CALLBACK OnWindowDestory(void* userdata)
#define LTK_WINDOW_DESTROY		100

	// void CALLBACK OnWindowClose(void* userdata, BOOL *pProceed);
#define LTK_WINDOW_CLOSE		101


	LTK_API UINT WINAPI LtkInitialize();
	LTK_API void WINAPI LtkUninitialize();
	LTK_API void WINAPI LtkRunMessageLoop();

	LTK_API void WINAPI LtkFree(LtkObject *);

	LTK_API void WINAPI LtkObject_RegisterCallback(
		LtkObject * obj, UINT event_id, LtkCallback cb, void* userdata);
	LTK_API LtkObject* WINAPI LtkCallbackInvoker();

	typedef struct _LtkWindow {
		int dummy;
	} LtkWindow; // 基类：LtkObject

	typedef struct _LtkSprite {
		int dummy;
	} LtkSprite; // 基类：LtkObject

#define LTK_SPRITE(o) LtkIsSprite(o) ? (LtkSprite*)o : NULL
	LTK_API BOOL WINAPI LtkIsSprite(LtkObject* o);

#define LtkWindow_New() LtkWindow_New_(__FILE__, __LINE__)
	LTK_API LtkWindow* WINAPI LtkWindow_New_(LPCSTR source, int line);
	LTK_API void WINAPI LtkWindow_Create(LtkWindow*self, LtkWindow* parent, LtkRect* rc);
	LTK_API void WINAPI LtkWindow_CreateCenter(LtkWindow*self, LtkWindow* parent, float width, float height);
	LTK_API void WINAPI LtkWindow_SetBackground(LtkWindow* self, LPCSTR name);
	LTK_API void WINAPI LtkWindow_UpdateTheme(LtkWindow* self);
	LTK_API void WINAPI LtkWindow_SetClientSprite(LtkWindow* self, LtkSprite* sp);

	typedef struct _LtkBoxLayout {
		int dummy;
	} LtkBoxLayout; // 基类：LtkSprite

#define LTK_HORIZONTAL  1
#define LTK_VERTICAL    2

#define LtkBoxLayout_New(o) LtkBoxLayout_New_(o, __FILE__, __LINE__)
	LTK_API LtkBoxLayout* WINAPI LtkBoxLayout_New_(UINT orientation, LPCSTR source, int line);
	LTK_API void WINAPI LtkBoxLayout_AddLayoutItem(LtkBoxLayout* self, LtkSprite *sp, float preferedSize, float growFactor);
	LTK_API void WINAPI LtkBoxLayout_AddSpaceItem(LtkBoxLayout* self, float preferedSize, float growFactor);
	LTK_API void WINAPI LtkBoxLayout_SetSpacing(LtkBoxLayout* self, float spacing);

	typedef struct _LtkButton {
		int dummy;
	} LtkButton; // 基类：LtkSprite

	// void CALLBACK OnButtonClicked(void* userdata)
#define LTK_BUTTON_CLICKED		201

#define LTK_BUTTON(o) LtkIsButton(o) ? (LtkButton*)o : NULL

#define LtkButton_New() LtkButton_New_(__FILE__, __LINE__)
	LTK_API LtkObject* WINAPI LtkButton_New_(LPCSTR source, int line);
	LTK_API BOOL WINAPI LtkIsButton(LtkObject* o);
	LTK_API void WINAPI LtkButton_SetText(LtkButton* self, LPCWSTR);

	typedef struct _LtkListView {
		int dummy;
	} LtkListView; // 基类：LtkSprite

	// void CALLBACK OnListViewSelectChange(void* userdata, int row, int oldRow)
#define LTK_LISTVIEW_SELECT_CHANGE		301

	// void CALLBACK OnListViewDbClick(void* userdata, int row) // TODO NOT IMPLEMENTED
#define LTK_LISTVIEW_DBCLICK			302

#define LTK_LISTVIEW(o) LtkIsListView(o) ? (LtkListView*)o : NULL

#define LtkListView_New() LtkListView_New_( __FILE__, __LINE__)
	LTK_API LtkObject* WINAPI LtkListView_New_(LPCSTR source, int line);
	LTK_API BOOL WINAPI LtkIsListView(LtkObject* o);
	LTK_API UINT WINAPI LtkListView_AddRow(LtkListView* self);
	LTK_API void WINAPI LtkListView_SetCellText(LtkListView* self, UINT row, UINT col, LPCWSTR text);
	LTK_API LPCWSTR WINAPI LtkListView_GetCellText(LtkListView* self, UINT row, UINT col);
	LTK_API int WINAPI LtkListView_GetSelectedRow(LtkListView* self);

	typedef struct _LtkSplitter {
		int dummy;
	} LtkSplitter; // 基类：LtkSprite

#define LTK_SPLITTER(o) LtkIsSplitter(o) ? (LtkSplitter*)o : NULL

#define LtkSplitter_New(o) LtkSplitter_New_(o,  __FILE__, __LINE__)
LTK_API LtkObject* WINAPI LtkSplitter_New_(UINT orientation, LPCSTR source, int line);
LTK_API BOOL WINAPI LtkIsSplitter(LtkObject* o);
LTK_API void WINAPI LtkSplitter_Resize(LtkSplitter* self, UINT n);
LTK_API LtkSprite* WINAPI LtkSplitter_SetClientAt(LtkSplitter* self, UINT idx, LtkSprite* sp);
LTK_API LtkSplitter* WINAPI LtkSplitter_GetClientAt(LtkSplitter* self, UINT idx);
LTK_API	UINT WINAPI LtkSplitter_GetClientCount(LtkSplitter* self);
LTK_API void WINAPI LtkSplitter_SetClientSize(LtkSplitter* self, UINT idx, float size);
LTK_API float WINAPI LtkSplitter_GetClientSize(LtkSplitter* self, UINT idx);

typedef struct _LtkTreeView {
	int dummy;
} LtkTreeView; // 基类：LtkSprite

#ifdef __cplusplus
} // extern "C"
#endif
