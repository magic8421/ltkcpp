#pragma once
#include "stdafx.h"
#include "RTTI.h"

namespace ltk {

enum Events
{
    eDestroy = 1,

    eWidgetFirst,
    ePaint,

    // type: MouseEvent
    eMouseFirst,
    eMouseMove,
    eMouseLeave,
    eMouseWheel,
    eLBtnDown,
    eLBtnUp,
	eLBtnDbClick,
    eRBtnDown,
    eRBtnUp,
    eMouseLast,

    eKeyDown,
    eKeyUp,
    eCharInput,
    eImeInput,
    eSetFocus,
    eKillFocus,
    eSizeChanged,
    eWidgetLast,

};

struct Event
{
    UINT id = 0;
};

struct MouseEvent : public Event
{
    float x = 0.f;
    float y = 0.f;
    UINT flag = 0; // ctrl shift atl
    float delta = 0.f; // wheel
};

struct DelegateMouseEvent : public Event
{
    MouseEvent *data = nullptr;
    bool bHandled = false;
};

struct KeyEvent : public Event
{
    DWORD keyCode = 0;
    DWORD flag = 0;
};

struct PaintEvent : public Event
{
    ID2D1RenderTarget *target = nullptr;
};

struct SizeEvent : public Event
{
    float width = 10.f;
    float height = 10.f;
};

struct ImeEvent : public Event
{
    LPCWSTR text = nullptr;
};

class Widget;

struct FocusEvent : public Event
{
    Widget *oldFocus = nullptr;
};


}
