//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once



namespace ltk {
	void LtkInitialize();
	void LtkUninitialize();
	ID2D1Factory *GetD2DFactory();
    IWICImagingFactory *GetWICFactory();
    IDWriteFactory *GetDWriteFactory();
    void TranslateTransform(ID2D1RenderTarget *target, float dx, float dy);
    D2D1_RECT_F D2D1RectF(const RectF &rc);
	HRESULT LoadBitmapFromFile(ID2D1RenderTarget * target, LPCWSTR path, ID2D1Bitmap ** bitmap);
	DWORD TickCount();

    struct Margin {
        float left = 0.0f;
        float top = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
    };
    void DrawTextureNineInOne(ID2D1RenderTarget *target, ID2D1Bitmap *bitmap,
        const RectF &atlas, const Margin& margin, RectF dst, float opacity, float scale);

    void DrawRectSnapped(ID2D1RenderTarget *target, const RectF &rc, ID2D1Brush *brush);

    void DipCoordToScreen(float &x, float &y);
    void ScreenCoordToDip(float &x, float &y);
    RECT DipRectToScreen(const RectF &rc);

    struct TextureInfo {
        TextureInfo& SetAtals(float x, float y, float w, float h);
        TextureInfo& SetMargin(float left, float top, float right, float bottom);

        UINT idx = 0;
        RectF atlas;
        Margin margin;
        float scale = 1.0f;
    };

    struct IconInfo {
        IconInfo& SetAtlas(float x, float y, float w, float h);
        IconInfo& SetScale(float s);

        UINT idx = 0;
        RectF atlas;
        bool bIconOnTop = false;
        float scale = 1.0f;
    };

    inline float Round45(float value) 
    {
        return (float)(int)(value + 0.5f);
    }

    inline D2D1_RECT_F D2D1RectF(const RectF &rc)
    {
        D2D1_RECT_F rc2;
        rc2.left = rc.X;
        rc2.top = rc.Y;
        rc2.right = rc.X + rc.Width;
        rc2.bottom = rc.Y + rc.Height;
        return rc2;
    }

    enum Orientation {
        Horizontal = 1,
        Vertical = 2
    };
}
