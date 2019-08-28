//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "AbstractButton.h"
#include "StyleManager.h"
#include "Delegate.h"

namespace ltk {

class Button : public AbstractButton
{
public:
    Button();
    virtual ~Button();

    void SetText(LPCWSTR text);
	void RecreateLayout();
	SizeF GetPreferredSize();

	ImmutableString Background;
	ImmutableString TextFormat;
	ImmutableString TextColor;

protected:
	virtual bool OnSize(SizeEvent *ev) override;
	virtual bool OnPaint(PaintEvent *ev) override;
    virtual void OnThemeChanged() override;

private:
    std::wstring m_text;
    AbstractBackground *m_background = nullptr;
    IDWriteTextFormat *m_format = nullptr;
	IDWriteTextLayout *m_layout = nullptr;
	D2D1_COLOR_F m_textColor;
};

} // namespace
