#pragma once
#include "Sprite.h"
#include "ltk.h"

namespace ltk {

class IconSprite : public Sprite
{
public:
    IconSprite() {}
    ~IconSprite() {}

    virtual bool OnPaint(PaintEvent *ev) override;
    void SetIcon(const RectF &atlas, float scale, UINT idx);
    void SetIcon(const IconInfo &v);

private:
    IconInfo m_info;
};

class TextureSprite : public Sprite
{
public:
    TextureSprite() {}
    ~TextureSprite() {}

    virtual bool OnPaint(PaintEvent *ev) override;
    void SetTexture(const RectF &atlas, const Margin &margin, float scale, UINT idx);

private:
    TextureInfo m_info;
};

class ImageSprite : public Sprite
{
public:
    ImageSprite() {}
    ~ImageSprite();

    virtual bool OnPaint(PaintEvent *ev) override;
    virtual void RecreateResouce(ID2D1RenderTarget *target) override;
    bool SetImage(const wchar_t *path);

private:
    ID2D1Bitmap *m_bitmap = nullptr;
    std::wstring m_path;
};

}
