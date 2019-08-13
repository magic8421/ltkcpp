#pragma once
#include "stdafx.h"
#include "RTTI.h"

namespace ltk {

enum Events
{
    eDestroy = 1,

    eSpriteFirst,
    ePaint,

    // type: MouseEvent
    eMouseFirst,
    eMouseMove,
    eMouseEnter,
    eMouseLeave,
    eMouseWheel,
    eLBtnDown,
    eLBtnUp,
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
    eSpriteLast, 

};

struct Event
{
    UINT id = 0;
};

struct MouseEvent : public Event
{
    UINT message;
    float x;
    float y;
    UINT flag; // ctrl shift atl
    float delta; // wheel
};

struct DelegateMouseEvent : public Event
{
    MouseEvent *data;
    bool bHandled;
};

struct KeyEvent : public Event
{
    DWORD keyCode;
    DWORD flag;
};

struct PaintEvent : public Event
{
    ID2D1RenderTarget *target;
};

struct SizeEvent : public Event
{
    float width;
    float height;
};

struct ImeEvent : public Event
{
    LPCWSTR text;
};

class Sprite;

struct FocusEvent : public Event
{
    Sprite *oldFocus;
};


}
