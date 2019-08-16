//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageSprite.h"
#include "StyleManager.h"
#include "ltk.h"
#include "Window.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

bool IconSprite::OnPaint(PaintEvent *ev)
{
    RectF rc = this->GetClientRect();
    RectF rc3;
    rc3.X = (rc.Width - m_info.atlas.Width * m_info.scale) / 2.0f;
    rc3.Y = (rc.Height - m_info.atlas.Height * m_info.scale) / 2.0f;
    rc3.Width = m_info.atlas.Width * m_info.scale;
    rc3.Height = m_info.atlas.Height * m_info.scale;
    auto bitmap = this->GetWindow()->GetAtlasBitmap();
    ev->target->DrawBitmap(bitmap, D2D1RectF(rc3), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
        D2D1RectF(m_info.atlas));
    return true;
}

void IconSprite::SetIcon(const RectF &atlas, float scale, UINT idx)
{
    m_info.atlas = atlas;
    m_info.scale = scale;
    m_info.idx = idx;
    this->Invalidate();
}

void IconSprite::SetIcon(const IconInfo &v)
{
    m_info = v;
    this->Invalidate();
}

bool TextureSprite::OnPaint(PaintEvent *ev)
{
    RectF rc = this->GetClientRect();
    DrawTextureNineInOne(
        ev->target,
        this->GetWindow()->GetAtlasBitmap(),
        m_info.atlas,
        m_info.margin,
        rc,
        1.0f,
        m_info.scale);
    return true;
}

void TextureSprite::SetTexture(const RectF &atlas, const Margin &margin, float scale, UINT idx)
{
    m_info.atlas = atlas;
    m_info.margin = margin;
    m_info.scale = scale;
    this->Invalidate();
}

ImageSprite::~ImageSprite()
{
    RELEASE_AND_INVALIDATE(ID2D1Bitmap, m_bitmap);
}

bool ImageSprite::OnPaint(PaintEvent *ev)
{
    RectF rc = this->GetClientRect();
    if (m_bitmap) {
    }
    return true;
}

bool ImageSprite::SetImage(const wchar_t *path)
{

    return false;
}

void ImageSprite::RecreateResouce(ID2D1RenderTarget *target)
{
    throw std::logic_error("The method or operation is not implemented.");
}


} //namespace ltk 