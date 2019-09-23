//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BoxLayout.h"
#include "BoxLayout_p.h"
#include "Sprite.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

BoxLayout::BoxLayout(Mode m) : Sprite(new BoxLayoutPrivate(this, m))
{
}


BoxLayout::BoxLayout(BoxLayoutPrivate *d)
{
}

BoxLayout::~BoxLayout()
{
}

BoxLayoutPrivate::BoxLayoutPrivate(BoxLayout *q, BoxLayout::Mode m):
	SpritePrivate(q), mode(m)
{
}

void BoxLayout::AddLayoutItem(Sprite *item, float preferedSize, float growFactor)
{
	LTK_D(BoxLayout);
	Sprite::AddChild(item);
    //item->AddRef();
    BoxLayoutParam param;
    param.item = item;
    param.size = preferedSize;
    param.growFactor = growFactor;
    // TODO check if duplicate.
	d->params.push_back(param);
}
void BoxLayout::InsertLayoutItem(UINT before, Sprite *item, float preferedSize, float growFactor)
{
	LTK_D(BoxLayout);
	Sprite::AddChild(item);
    //item->AddRef();
    BoxLayoutParam param;
    param.item = item;
    param.size = preferedSize;
    param.growFactor = growFactor;
    // TODO check if duplicate.
	d->params.insert(d->params.begin() + before, param);
}

void BoxLayout::AddSpaceItem(float preferedSize, float growFactor)
{
	LTK_D(BoxLayout);
	BoxLayoutParam param;
    param.size = preferedSize;
    param.growFactor = growFactor;
	d->params.push_back(param);
}

bool BoxLayout::OnSize(SizeEvent *ev)
{
	LTK_D(BoxLayout);
    // TODO rename this function
    float sum_size = 0.0f;
    float sum_factor = 0.0f;
	for (size_t i = 0; i < d->params.size(); i++) {
		sum_size += d->params[i].size;
		sum_factor += d->params[i].growFactor;
    }

	sum_size += d->spacing * (d->params.size() - 1);

    float remain;
	if (d->mode == Horizontal) {
        remain = ev->width - sum_size;
    }
    else {
        remain = ev->height - sum_size;
    }
    remain = max(0.0f, remain);

    float x = 0;
    float y = 0;
	for (size_t i = 0; i < d->params.size(); i++) {
        RectF rc2;
        float size;
        if (sum_factor > 0.0f) {
			size = d->params[i].size + remain * (d->params[i].growFactor / sum_factor);
        }
        else {
			size = d->params[i].size;
        }
		if (d->mode == Horizontal) {
            rc2.X = x;
            rc2.Y = y;
            rc2.Width = size;
            rc2.Height = ev->height;
			x += size + d->spacing;
        }
        else {
            rc2.X = x;
            rc2.Y = y;
            rc2.Width = ev->width;
            rc2.Height = size;
			y += size + d->spacing;
        }
		if (d->params[i].item) {
            //rc2.X = (float)(int)(rc2.X + 0.5f);
            //rc2.Y = (float)(int)(rc2.Y + 0.5f);
            //rc2.Width = (float)(int)(rc2.Width + 0.5f);
            //rc2.Height = (float)(int)(rc2.Height + 0.5f);
			d->params[i].item->SetRect(rc2);
        }
    }
    return true;
}

void BoxLayout::SetSpacing(float spacing)
{
	LTK_D(BoxLayout);
	d->spacing = spacing;
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

