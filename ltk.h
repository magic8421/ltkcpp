/*
** LTK -- a lightweight GUI toolkit based on Direct2D
**
** Copyright (C) 2015-2016 Chen Shi. All rights reserved.
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
** [ MIT license: http://www.opensource.org/licenses/mit-license.php ]
*/

#pragma once

#ifdef LTK_EXPORTS
#define LTK_API __declspec(dllexport)
#else
#define LTK_API __declspec(dllimport)
#endif

namespace ltk {
    void LtkInitialize();
    void LtkUninitialize();
    ID2D1Factory *GetD2DFactory();
    IWICImagingFactory *GetWICFactory();
    IDWriteFactory *GetDWriteFactory();
    void TranslateTransform(ID2D1RenderTarget *target, float dx, float dy);
    D2D1_RECT_F D2D1RectF(const RectF &rc);
	HRESULT LoadBitmapFromFile(ID2D1RenderTarget * target, LPCWSTR path, ID2D1Bitmap ** bitmap);

    struct Margin {
        float left = 0.0f;
        float top = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
    };
    void DrawTextureNineInOne(ID2D1RenderTarget *target, ID2D1Bitmap *bitmap,
        const RectF &atlas, const Margin& margin, RectF dst, float opacity, float scale);

    void DrawRectSnapped(ID2D1RenderTarget *target, const RectF &rc, ID2D1Brush *brush);

    void MapCoordByDpi(float &x, float &y);
    void UnmapCoordByDpi(float &x, float &y);

    struct TextureInfo {
        UINT idx = 0;
        RectF atlas;
        Margin margin;
        float scale = 1.0f;
    };

    struct IconInfo {
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
