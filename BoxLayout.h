//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "ltk.h"
#include "Sprite.h"

namespace ltk {

struct BoxLayoutParam {
    Widget *item = nullptr;
    float size = 20.0f;
    float growFactor = 0.0f;
};

class BoxLayout : public Widget
{
public:
	explicit BoxLayout(Orientation);
    virtual ~BoxLayout();

	virtual bool OnSize(SizeEvent *ev) override;

    void SetSpacing(float spacing);

    void AddLayoutItem(Widget *sp, float preferedSize, float growFactor = 0.0f);

    void InsertLayoutItem(UINT before, Widget *item, float preferedSize, float growFactor);

    void AddSpaceItem(float preferedSize, float growFactor);

    void DoLayout();

private:
	bool AlreadyHas(Widget *item);

	std::vector<BoxLayoutParam> m_params; // item: owner
	Orientation m_mode;
    float m_spacing = 5.0f;
};

} // namespace ltk
