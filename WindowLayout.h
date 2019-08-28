//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Sprite.h"
#include "Delegate.h"

namespace ltk { 

class Button;
class Label;
class MenuBar;

class WindowLayout : public Sprite
{
public:
    WindowLayout();
    virtual ~WindowLayout();

    Sprite *SetClientSprite(Sprite *sp);
    Sprite *GetClientSprite();
	
	MenuBar *SetMenuBar(MenuBar *menu);
	MenuBar *GetMenuBar();

    void DoLayout();
    void SetWindow(Window *wnd);
    RectF GetCaptionRect();
    LPCWSTR GetCaptionText();
    void SetCaptionText(LPCWSTR text);

    virtual bool OnSize(SizeEvent *ev) override;

private:
    Sprite *m_client = nullptr;
    Button *m_minBtn = nullptr;
    Button *m_maxBtn = nullptr;
    Button *m_closeBtn = nullptr;
	MenuBar *m_menu = nullptr;
    
    Cookie m_minTrack = 0;
    Cookie m_maxTrack = 0;
    Cookie m_closeTrack = 0;
    Label *m_caption = nullptr;
};

} // namespace ltk