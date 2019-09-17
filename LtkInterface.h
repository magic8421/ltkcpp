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

typedef struct _LtkObject
{
	int dummy;
} LtkObject;

typedef struct _LtkWindow
{
	int dummy;
} LtkWindow;

typedef struct _LtkRect
{
	float x;
	float y;
	float w;
	float h;
} LtkRect;

typedef void (CALLBACK * LtkEventCallback) (void* userdata, UINT event_id, void* sender, void* arg);

#define LTK_WINDOW_DESTROY		100


LTK_API LtkError WINAPI LtkInitialize();
LTK_API void WINAPI LtkUninitialize();
LTK_API void WINAPI LtkRunApp();

LTK_API void WINAPI LtkObject_RegisterCallback(LtkObject* self, UINT event_id, LtkEventCallback cb, void* userdata);

LTK_API LtkWindow* WINAPI LtkCreateWindow(LtkWindow* parent, LtkRect* rc);
LTK_API void WINAPI LtkWindow_SetBackground(LtkWindow* self, LPCSTR name);
LTK_API void WINAPI LtkWindow_UpdateTheme(LtkWindow* self);

#ifdef __cplusplus
} // extern "C"
#endif
