//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Sprite.h"

namespace ltk {

struct BoxLayoutParam {
    Sprite *item = nullptr;
    float size = 20.0f;
    float growFactor = 0.0f;
};

class BoxLayout : public Sprite
{
public:
    enum Mode {
        Horizontal, Vertical
    };
    explicit BoxLayout(Mode m);
    virtual ~BoxLayout();

    virtual bool OnSize(SizeEvent *ev) override;

    void SetMargin(float m);

    void SetLeftMargin(float m);
    void SetTopMargin(float m);
    void SetRightMargin(float m);
    void SetBottomMargin(float m);

    void SetSpacing(float spacing);

    void AddLayoutItem(Sprite *sp, float preferedSize, float growFactor = 0.0f);

    void InsertLayoutItem(UINT before, Sprite *item, float preferedSize, float growFactor);

    void AddSpaceItem(float preferedSize, float growFactor);

    void DoLayout();

private:
    std::vector<BoxLayoutParam> m_params; // item: owner
    Mode m_mode;
    float m_marginLeft = 0.0f;
    float m_marginRight = 0.0f;
    float m_marginTop = 0.0f;
    float m_marginBottom = 0.0f;
    float m_spacing = 5.0f;
};

} // namespace ltk
