//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Sprite.h"

namespace ltk {

class BoxLayoutPrivate;

class BoxLayout : public Sprite
{
public:
    enum Mode {
        Horizontal, Vertical
    };
    explicit BoxLayout(Mode m);
    virtual ~BoxLayout();

    virtual bool OnSize(SizeEvent *ev) override;

    void SetSpacing(float spacing);

    void AddLayoutItem(Sprite *sp, float preferedSize, float growFactor = 0.0f);

    void InsertLayoutItem(UINT before, Sprite *item, float preferedSize, float growFactor);

    void AddSpaceItem(float preferedSize, float growFactor);

    void DoLayout();

private:
	LTK_DECLARE_PRIVATE(BoxLayout);

protected:
	BoxLayout(BoxLayoutPrivate *d);
};

} // namespace ltk
