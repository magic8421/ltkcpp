//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "Sprite.h"
#include "Common.h"

namespace ltk {

class Label : public Widget
{
public:
    Label();
    virtual ~Label();
    virtual bool OnPaint(PaintEvent *ev) override;
    virtual void OnThemeChanged() override;


    LPCWSTR GetText();
    void SetText(LPCWSTR text);

    void SetTextFormat(LPCSTR style);
    void SetTextColor(LPCSTR style);

private:
	std::wstring m_text;
	IDWriteTextFormat *m_textFormat = nullptr;
	D2D1_COLOR_F m_color;

    LPCSTR m_szTextFormat = nullptr;
    LPCSTR m_szTextColor = nullptr;
};

} // namespace ltk
