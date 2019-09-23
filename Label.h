//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "Sprite.h"
#include "Common.h"
#include "ImmutableString.h"

namespace ltk {

class LabelPrivate;

class Label : public Sprite
{
public:
    Label();
    virtual ~Label();

    LPCWSTR GetText();
    void SetText(LPCWSTR text);

	void SetTextFormat(LPCSTR name);
	void SetTextColor(LPCSTR name);

protected:
	virtual bool OnPaint(PaintEvent *ev) override;
	virtual void OnThemeChanged() override;
    
private:
	LTK_DECLARE_PRIVATE(Label);

protected:
	Label(LabelPrivate *q);
};

} // namespace ltk
