//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Sprite.h"

namespace ltk { 

class Button;
class Label;
class MenuBar;

class LTK_CPP_API WindowLayout : public Sprite
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
	void OnMinBtnClicked();
	void OnMaxBtnClicked();
	void OnCloseBtnClicked();

    Sprite *m_client = nullptr;
    Button *m_minBtn = nullptr;
    Button *m_maxBtn = nullptr;
    Button *m_closeBtn = nullptr;
	MenuBar *m_menu = nullptr;
    
    Label *m_caption = nullptr;
};

} // namespace ltk