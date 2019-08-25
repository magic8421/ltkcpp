//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once

namespace ltk {

class ShadowFrame
{
public:
    enum Mode {
        eLeft, eTop, eRight, eBottom
    };
    explicit ShadowFrame(Mode m);
    ~ShadowFrame();

    static void Init();
    static void Free();
    void Create();
    void Destroy();
    void Update(HWND hParent, HDWP &hdwp, bool bRedraw);
    void OnDraw(const RECT &rc);
    void DrawShadow(Gdiplus::Graphics &g, Gdiplus::Rect rc);

    static void RegisterWndClass();
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
    HWND m_hwnd = 0;
    Mode m_mode = eLeft;
    static Gdiplus::Bitmap *m_bitmap;
};

}
