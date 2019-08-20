//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "Sprite.h"

namespace ltk {

struct LabelStyle;

class Label : public Sprite
{
public:
    Label();
    virtual ~Label();
    virtual bool OnPaint(PaintEvent *ev) override;
    virtual void OnThemeChanged() override;


    LPCWSTR GetText();
    void SetText(LPCWSTR text);
    

private:
    IDWriteTextFormat *m_textFormat = nullptr;
    std::wstring m_text;
    LabelStyle *m_style = nullptr;
};

} // namespace ltk
