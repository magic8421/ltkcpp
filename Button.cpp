//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Button.h"
#include "Label.h"
#include "ltk.h"
#include "StyleManager.h"
#include "ImageSprite.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

Button::Button()
{
    m_background = StyleManager::Instance()->GetBackground("default_button");
}

Button::~Button()
{
}

void Button::SetBackgroundStyle(LPCSTR style)
{
    m_background = StyleManager::Instance()->GetBackground(style);
}

bool Button::OnPaint(PaintEvent *ev)
{
    Update();
    //LTK_LOG("OnPaint target: 0x%08x", ev->target);

    Window *wnd = this->GetWindow();
    auto rc = this->GetClientRect();
    if (m_background) {
        float blend;
        switch (GetState()) {
        case AbstractBackground::Normal2Hover:
        case AbstractBackground::Hover2Normal:
            blend = GetBlend();
            break;
        default:
            blend = 1.0f;
        }
        m_background->Draw(wnd, ev->target, rc, GetState(), blend);
    }
    return true;
}

void Button::SetText(LPCWSTR text)
{
    m_text = text;
    this->Invalidate();
}



} // namespace ltk

