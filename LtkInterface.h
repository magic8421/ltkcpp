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
LTK_API void WINAPI LtkRunApp();

LTK_API void WINAPI LtkFree(LtkObject *);

LTK_API void WINAPI LtkObject_RegisterCallback(
	LtkObject * obj, UINT event_id, LtkCallback cb, void* userdata);
LTK_API LtkObject* WINAPI LtkGetEventSender();

typedef struct _LtkWindow {
	int dummy;
} LtkWindow; // 基类：LtkObject

typedef struct _LtkSprite {
	int dummy;
} LtkSprite; // 基类：LtkObject

LTK_API LtkWindow* WINAPI LtkWindow_New();
LTK_API void WINAPI LtkCreateWindow(LtkWindow*self, LtkWindow* parent, LtkRect* rc);
LTK_API void WINAPI LtkCreateWindowCenter(LtkWindow*self, LtkWindow* parent, float width, float height);
LTK_API void WINAPI LtkWindow_SetBackground(LtkWindow* self, LPCSTR name);
LTK_API void WINAPI LtkWindow_UpdateTheme(LtkWindow* self);
LTK_API void WINAPI LtkWindow_SetClientSprite(LtkWindow* self, LtkSprite* sp);

typedef struct _LtkBoxLayout {
	int dummy;
} LtkBoxLayout; // 基类：LtkSprite

#define LTK_HORIZONTAL  1
#define LTK_VERTICAL    2

LTK_API LtkBoxLayout* WINAPI LtkBoxLayout_New(UINT orientation);
LTK_API void WINAPI LtkBoxLayout_AddLayoutItem(LtkBoxLayout* self, LtkSprite *sp, float preferedSize, float growFactor);
LTK_API void WINAPI LtkBoxLayout_AddSpaceItem(LtkBoxLayout* self, float preferedSize, float growFactor);
LTK_API void WINAPI LtkBoxLayout_SetSpacing(LtkBoxLayout* self, float spacing);

typedef struct _LtkButton {
	int dummy;
} LtkButton; // 基类：LtkSprite

// void CALLBACK OnButtonClicked(void* userdata)
#define LTK_BUTTON_CLICKED		201

LTK_API LtkButton* WINAPI LtkButton_New();
LTK_API void WINAPI LtkButton_SetText(LtkButton* self, LPCWSTR);

typedef struct _LtkListView {
	int dummy;
} LtkListView; // 基类：LtkSprite

// void CALLBACK OnListViewSelectChange(void* userdata, int row, int oldRow)
#define LTK_LISTVIEW_SELECT_CHANGE		301

// void CALLBACK OnListViewDbClick(void* userdata, int row) // TODO NOT IMPLEMENTED
#define LTK_LISTVIEW_DBCLICK			302

LTK_API LtkListView* WINAPI LtkListView_New();
LTK_API UINT WINAPI LtkListView_AddRow(LtkListView* self);
LTK_API void WINAPI LtkListView_SetCellText(LtkListView* self, UINT row, UINT col, LPCWSTR text);
LTK_API LPCWSTR WINAPI LtkListView_GetCellText(LtkListView* self, UINT row, UINT col);
LTK_API int WINAPI LtkListView_GetSelectedRow(LtkListView* self);

typedef struct _LtkSplitter {
	int dummy;
} LtkSplitter; // 基类：LtkSprite

LTK_API LtkSplitter* WINAPI LtkSplitter_New(UINT orientation);
LTK_API void WINAPI LtkSplitter_Resize(LtkSplitter* self, UINT n);
LTK_API LtkSprite* WINAPI LtkSplitter_SetClientAt(LtkSplitter* self, UINT idx, LtkSprite* sp);
LTK_API LtkSplitter* WINAPI LtkSplitter_GetClientAt(LtkSplitter* self, UINT idx);
LTK_API	UINT WINAPI LtkSplitter_GetClientCount(LtkSplitter* self);
LTK_API void WINAPI LtkSplitter_SetClientSize(LtkSplitter* self, UINT idx, float size);
LTK_API float WINAPI LtkSplitter_GetClientSize(LtkSplitter* self, UINT idx);


#ifdef __cplusplus
} // extern "C"
#endif
