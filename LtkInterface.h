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

struct _HLTK {
	int dummy;
};
typedef struct _HLTK* HLTK;

typedef struct _LtkRect {
	float x;
	float y;
	float w;
	float h;
} LtkRect;

typedef void (CALLBACK *LtkCallback)();

// BOOL OnWindowDestory()
#define LTK_WINDOW_DESTROY		100

// BOOL OnWindowClose(BOOL *pProceed);
#define LTK_WINDOW_CLOSE		101


LTK_API UINT WINAPI LtkInitialize();
LTK_API void WINAPI LtkUninitialize();
LTK_API void WINAPI LtkRunApp();

LTK_API void WINAPI LtkFree(HLTK h);

LTK_API void WINAPI LtkObject_RegisterCallback(HLTK obj, UINT event_id, LtkCallback cb, void* userdata);
LTK_API HLTK WINAPI LtkGetEventSender();

LTK_API HLTK WINAPI LtkCreateWindow(HLTK parent, LtkRect* rc);
LTK_API HLTK WINAPI LtkCreateWindowCenter(HLTK parent, float width, float height);
LTK_API void WINAPI LtkWindow_SetBackground(HLTK self, LPCSTR name);
LTK_API void WINAPI LtkWindow_UpdateTheme(HLTK wnd);

#ifdef __cplusplus
} // extern "C"
#endif
