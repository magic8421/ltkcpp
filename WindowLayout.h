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

class LTK_CPP_API WindowLayout : public Widget
{
public:
    WindowLayout();
    virtual ~WindowLayout();

    void SetCentralWidget(Ptr<Widget> sp);
    Ptr<Widget> GetClientWidget();
	
    void WindowLayout::SetMenuBar(Ptr<MenuBar> menu);
    Ptr<MenuBar> GetMenuBar();

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

    Ptr<Widget> m_client;
    Ptr<Button> m_minBtn;
    Ptr<Button> m_maxBtn;
    Ptr<Button> m_closeBtn;
	Ptr<MenuBar> m_menu;
    
    Ptr<Label> m_caption;
};

} // namespace ltk