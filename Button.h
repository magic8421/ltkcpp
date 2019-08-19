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

    void SetBackgroundStyle(LPCSTR style);
    void SetText(LPCWSTR text);


    virtual bool OnPaint(PaintEvent *ev) override;

private:
    std::wstring m_text;
    AbstractBackground *m_background = nullptr;
};

} // namespace
