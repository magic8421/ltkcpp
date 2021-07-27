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
    Ptr<Widget> item;
    float size = 20.0f;
    float growFactor = 0.0f;
};

class LTK_CPP_API BoxLayout : public Widget
{
public:
    RTTI_DECLARATIONS(BoxLayout, Widget);
	explicit BoxLayout(Orientation);
    explicit BoxLayout(UINT o) : BoxLayout((Orientation)o) {} // for dukglue
    virtual ~BoxLayout();

    static void BoxLayout::Init();

    static Ptr<Widget> CreateHBox();
    static Ptr<Widget> CreateVBox();

	virtual bool OnSize(SizeEvent *ev) override;

    void SetSpacing(float spacing);

    void AddLayoutItem(Ptr<Widget> item, float preferedSize, float growFactor = 0.0f);

    void InsertLayoutItem(UINT before, Ptr<Widget> item, float preferedSize, float growFactor);

    void AddSpaceItem(float preferedSize, float growFactor);

    void DoLayout();

    virtual void OnChildAttribute(Object* child, LPCSTR name, LPCSTR value) override;


private:
	bool AlreadyHas(const Ptr<Widget> &item);

	std::vector<BoxLayoutParam> m_params; // item: owner
	Orientation m_mode;
    float m_spacing = 5.0f;
};

} // namespace ltk
