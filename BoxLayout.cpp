//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BoxLayout.h"
#include "Sprite.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

BoxLayout::BoxLayout(Mode m) :  m_mode(m)
{
}


BoxLayout::~BoxLayout()
{
    //for (size_t i = 0; i < m_params.size(); i++) {
    //    if (m_params[i].item) {
    //        m_params[i].item->Release();
    //    }
    //}
}

void BoxLayout::AddLayoutItem(Sprite *item, float preferedSize, float growFactor)
{
    Sprite::AddChild(item);
    //item->AddRef();
    BoxLayoutParam param;
    param.item = item;
    param.size = preferedSize;
    param.growFactor = growFactor;
    // TODO check if duplicate.
    m_params.push_back(param);
}
void BoxLayout::InsertLayoutItem(UINT before, Sprite *item, float preferedSize, float growFactor)
{
    Sprite::AddChild(item);
    //item->AddRef();
    BoxLayoutParam param;
    param.item = item;
    param.size = preferedSize;
    param.growFactor = growFactor;
    // TODO check if duplicate.
    m_params.insert(m_params.begin() + before, param);
}

void BoxLayout::AddSpaceItem(float preferedSize, float growFactor)
{
    BoxLayoutParam param;
    param.size = preferedSize;
    param.growFactor = growFactor;
    m_params.push_back(param);
}

bool BoxLayout::OnSize(SizeEvent *ev)
{
    // TODO rename this function
    float sum_size = 0.0f;
    float sum_factor = 0.0f;
    for (size_t i = 0; i < m_params.size(); i++) {
        sum_size += m_params[i].size;
        sum_factor += m_params[i].growFactor;
    }
    //if (m_mode == Horizontal) {
    //    sum_size += m_marginLeft + m_marginRight;
    //}
    //else {
    //    sum_size += m_marginTop + m_marginBottom;
    //}
    sum_size += m_spacing * (m_params.size() - 1);

    float remain;
    if (m_mode == Horizontal) {
        remain = ev->width - sum_size;
    }
    else {
        remain = ev->height - sum_size;
    }
    remain = max(0.0f, remain);

    float x = 0; //m_marginLeft;
    float y = 0; //m_marginTop;
    for (size_t i = 0; i < m_params.size(); i++) {
        RectF rc2;
        float size;
        if (sum_factor > 0.0f) {
            size = m_params[i].size + remain * (m_params[i].growFactor / sum_factor);
        }
        else {
            size = m_params[i].size;
        }
        if (m_mode == Horizontal) {
            rc2.X = x;
            rc2.Y = y;
            rc2.Width = size;
            rc2.Height = ev->height;// -m_marginTop - m_marginBottom;
            x += size + m_spacing;
        }
        else {
            rc2.X = x;
            rc2.Y = y;
            rc2.Width = ev->width;// -m_marginLeft - m_marginRight;
            rc2.Height = size;
            y += size + m_spacing;
        }
        if (m_params[i].item) {
            //rc2.X = (float)(int)(rc2.X + 0.5f);
            //rc2.Y = (float)(int)(rc2.Y + 0.5f);
            //rc2.Width = (float)(int)(rc2.Width + 0.5f);
            //rc2.Height = (float)(int)(rc2.Height + 0.5f);
            m_params[i].item->SetRect(rc2);
        }
    }
    return true;
}

/*
void BoxLayout::SetMargin(float margin)
{
    margin = max(0.0f, margin);
    m_marginLeft = margin;
    m_marginRight = margin;
    m_marginTop = margin;
    m_marginBottom = margin;
}

void BoxLayout::SetLeftMargin(float m)
{
    m = max(0.0f, m);
    m_marginLeft = m;
}

void BoxLayout::SetTopMargin(float m)
{
    m = max(0.0f, m);
    m_marginTop = m;
}

void BoxLayout::SetRightMargin(float m)
{
    m = max(0.0f, m);
    m_marginRight = m;
}

void BoxLayout::SetBottomMargin(float m)
{
    m = max(0.0f, m);
    m_marginBottom = m;
}
*/

void BoxLayout::SetSpacing(float spacing)
{
    m_spacing = spacing;
}

void BoxLayout::DoLayout()
{
    RectF rc = this->GetRect();
    SizeEvent ev;
    ev.id = eSizeChanged;
    ev.width = rc.Width;
    ev.height = rc.Height;
    this->OnEvent(&ev);
}

} // namespace ltk

