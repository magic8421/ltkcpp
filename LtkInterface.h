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

// void OnWindowDestory()
#define LTK_WINDOW_DESTROY		100

// void OnWindowClose(BOOL *pProceed);
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
} LtkWindow;

LTK_API LtkWindow* WINAPI LtkWindow_New();
LTK_API void WINAPI LtkCreateWindow(LtkWindow*self, LtkWindow* parent, LtkRect* rc);
LTK_API void WINAPI LtkCreateWindowCenter(LtkWindow*self, LtkWindow* parent, float width, float height);
LTK_API void WINAPI LtkWindow_SetBackground(LtkWindow* self, LPCSTR name);
LTK_API void WINAPI LtkWindow_UpdateTheme(LtkWindow* self);

typedef struct _LtkSprite {
	int dummy;
} LtkSprite;

typedef struct _LtkBoxLayout {
	int dummy;
} LtkBoxLayout;

#define LTK_HORIZONTAL  1
#define LTK_VERTICAL    2
LTK_API LtkBoxLayout* WINAPI LtkBoxLayout_New(UINT orientation);
LTK_API void WINAPI LtkBoxLayout_AddLayoutItem(LtkBoxLayout* self, LtkSprite *sp, float preferedSize, float growFactor);
LTK_API void WINAPI LtkBoxLayout_AddSpaceItem(LtkBoxLayout* self, float preferedSize, float growFactor);
LTK_API void WINAPI LtkBoxLayout_SetSpacing(LtkBoxLayout* self, float spacing);

typedef struct _LtkButton {
	int dummy;
} LtkButton;

// void OnButtonClicked()
#define LTK_BUTTON_CLICKED		201

LTK_API LtkButton* WINAPI LtkButton_New();
LTK_API void WINAPI LtkButton_SetText(LPCWSTR);

typedef struct _LtkListView {
	int dummy;
} LtkListView;

LTK_API LtkListView* WINAPI LtkListView_New();
LTK_API void WINAPI LtkListView_AddItem(LtkListView* self, LPCWSTR text);
LTK_API void WINAPI LtkListView_SetSubItemText(LtkListView* self, UINT row, UINT col, LPCWSTR text);

#ifdef __cplusplus
} // extern "C"
#endif
