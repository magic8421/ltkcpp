//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "AbstractButton.h"
#include "StyleManager.h"

namespace ltk {

class LTK_CPP_API Button : public AbstractButton
{
public:
	RTTI_DECLARATIONS(Button, AbstractButton);

    Button();
    virtual ~Button();

    void SetText(LPCWSTR text);
	void RecreateLayout();
	SizeF GetPreferredSize();

	void SetBackground(LPCSTR style);
	void SetTextFormat(LPCSTR style);
	void SetTextColor(LPCSTR style);

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

	LPCSTR m_szBackground = nullptr;
	LPCSTR m_szTextFormat = nullptr;
	LPCSTR m_szTextColor = nullptr;
};

} // namespace
