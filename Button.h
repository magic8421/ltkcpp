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

struct ButtonPrivate;

class Button : public AbstractButton
{
public:
    Button();
    virtual ~Button();

    void SetText(LPCWSTR text);
	void RecreateLayout();
	SizeF GetPreferredSize();

	void SetBackground(LPCSTR);
	void SetTextFormat(LPCSTR);
	void SetTextColor(LPCSTR);

protected:
	virtual bool OnSize(SizeEvent *ev) override;
	virtual bool OnPaint(PaintEvent *ev) override;
    virtual void OnThemeChanged() override;

protected:
	Button(ButtonPrivate *pd);

private:
	LTK_DECLARE_PRIVATE(Button);
};

} // namespace

